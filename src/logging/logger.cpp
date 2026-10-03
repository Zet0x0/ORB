#include "logger.h"
#include "logcategories.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJSEngine>
#include <QLoggingCategory>
#include <QMetaObject>
#include <QScopeGuard>
#include <QStandardPaths>
#include <QStringList>
#include <QThread>
#include <QVariant>
#include <utility>

void Logger::install() {
    Logger *self = Logger::instance();

    QJSEngine::setObjectOwnership(self, QJSEngine::CppOwnership);

    self->m_guiThread = QThread::currentThread();
    self->m_previousHandler = qInstallMessageHandler(&Logger::messageHandler);

    self->openLogFile();
}

int Logger::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_entries.size();
}

int Logger::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant Logger::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 ||
        index.row() >= m_entries.size()) {
        return QVariant();
    }

    const Entry &entry = m_entries.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case TimeColumn:
            return entry.timestamp.toString(QStringLiteral("HH:mm:ss.zzz"));

        case LevelColumn:
            return levelName(entry.level);

        case CategoryColumn:
            return entry.category;

        case MessageColumn:
            return entry.message;

        default:
            return QVariant();
        }

    case TimestampRole:
        return entry.timestamp;

    case LevelRole:
        return entry.level;

    case LineTextRole:
        return formatLine(entry.timestamp, entry.level, entry.category,
                          entry.message);

    default:
        return QVariant();
    }
}

QVariant Logger::headerData(int section, Qt::Orientation orientation,
                            int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    switch (section) {
    case TimeColumn:
        return tr("Time");
    case LevelColumn:
        return tr("Level");
    case CategoryColumn:
        return tr("Category");
    case MessageColumn:
        return tr("Message");
    }

    return QVariant();
}

QHash<int, QByteArray> Logger::roleNames() const {
    static const QHash<int, QByteArray> roles{
        {Qt::DisplayRole, QByteArrayLiteral("display")},
        {LevelRole, QByteArrayLiteral("level")},
    };

    return roles;
}

int Logger::count() const {
    return m_entries.size();
}

QUrl Logger::directoryUrl() const {
    return QUrl::fromLocalFile(directoryPath());
}

void Logger::setMaxEntries(int newMaxEntries) {
    newMaxEntries = qMax(1, newMaxEntries);

    if (m_maxEntries == newMaxEntries) {
        return;
    }

    m_maxEntries = newMaxEntries;

    const int excess = m_entries.size() - m_maxEntries;

    if (excess <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, excess - 1);
    m_entries.remove(0, excess);
    endRemoveRows();

    emit countChanged();
}

void Logger::setMaxFiles(int maxFiles) {
    maxFiles = qMax(1, maxFiles);

    const QDir directory(directoryPath());

    QStringList existing =
        directory.entryList({QStringLiteral("*.log")}, QDir::Files, QDir::Name);

    // never touch the one being written to
    existing.removeOne(QFileInfo(m_logFile.fileName()).fileName());

    for (int i = 0; i < existing.size() - (maxFiles - 1); ++i) {
        QFile file(directory.filePath(existing.at(i)));

        if (!file.remove()) {
            qCWarning(lcLogging) << "Cannot remove old log file"
                                 << file.fileName() << file.errorString();

            continue;
        }

        qCDebug(lcLogging) << "Removed old log file" << file.fileName();
    }
}

void Logger::setFilterRules(const QString &rules) {
    // setFilterRules() only splits on newlines
    QLoggingCategory::setFilterRules(QString(rules).replace(u';', u'\n'));

    if (rules.isEmpty()) {
        return;
    }

    qCInfo(lcLogging) << "Applied category rules" << rules;

    for (const char *variable : {"QT_LOGGING_CONF", "QT_LOGGING_RULES"}) {
        if (qEnvironmentVariableIsSet(variable)) {
            qCWarning(lcLogging)
                << variable
                << "is set and takes precedence over applied category rules";
        }
    }
}

Logger::Logger(QObject *parent) : QAbstractTableModel(parent) {}

QString Logger::directoryPath() {
    return QDir(QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation))
        .filePath(QStringLiteral("logs"));
}

QString Logger::levelName(Level level) {
    switch (level) {
    case Debug:
        return QStringLiteral("DEBUG");
    case Info:
        return QStringLiteral("INFO");
    case Warning:
        return QStringLiteral("WARNING");
    case Critical:
        return QStringLiteral("CRITICAL");
    case Fatal:
        return QStringLiteral("FATAL");
    }

    return QStringLiteral("INFO");
}

QString Logger::formatLine(const QDateTime &timestamp, Level level,
                           const QString &category, const QString &message) {
    const QString prefix =
        category.isEmpty() ? QString() : QStringLiteral("%0: ").arg(category);

    return QStringLiteral("[%0] [%1] %2%3")
        .arg(timestamp.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss.zzz")),
             levelName(level), prefix, message);
}

Logger::Level Logger::levelFromMsgType(QtMsgType type) {
    switch (type) {
    case QtDebugMsg:
        return Debug;
    case QtInfoMsg:
        return Info;
    case QtWarningMsg:
        return Warning;
    case QtCriticalMsg:
        return Critical;
    case QtFatalMsg:
        return Fatal;
    }

    return Info;
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context,
                            const QString &message) {
    Logger *self = Logger::instance();

    // Message to console first (Qt's default handler if none was installed)
    if (self->m_previousHandler != nullptr) {
        self->m_previousHandler(type, context, message);
    }

    Entry entry;

    entry.timestamp = QDateTime::currentDateTime();
    entry.level = levelFromMsgType(type);
    entry.category = QString::fromUtf8(
        context.category != nullptr ? context.category : "default");
    entry.message = message;

    self->writeToFile(formatLine(entry.timestamp, entry.level, entry.category,
                                 entry.message));

    // Logged something while a row was being added (maybe QML warning reacting
    // to it?), adding that one too could keep triggering itself
    if (QThread::currentThread() == self->m_guiThread &&
        self->m_appendingEntry) {
        return;
    }

    QMetaObject::invokeMethod(
        self,
        [self, entry = std::move(entry)]() mutable {
            self->appendEntry(std::move(entry));
        },
        Qt::QueuedConnection);
}

void Logger::openLogFile() {
    const QDir directory(directoryPath());

    QDir().mkpath(directory.absolutePath());

    const QDateTime currentDateTime = QDateTime::currentDateTime();

    const QString base =
        currentDateTime.toString(QStringLiteral("yyyy-MM-dd_HH-mm-ss"));

    QString path = directory.filePath(base + QStringLiteral(".log"));

    for (int n = 2; QFile::exists(path); ++n) {
        path = directory.filePath(QStringLiteral("%0_%1.log").arg(base).arg(n));
    }

    m_logFile.setFileName(path);

    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCWarning(lcLogging)
            << "Cannot open log file" << path << m_logFile.errorString();

        return;
    }

    m_logStream.setDevice(&m_logFile);

    const QString headerMessage =
        QStringLiteral("===== ORB %0 =====")
            .arg(QCoreApplication::applicationVersion());

    m_logStream << headerMessage << u'\n';
    m_logStream << QStringLiteral("Logging for PID %0 since %1")
                       .arg(QString::number(QCoreApplication::applicationPid()),
                            currentDateTime.toString(Qt::ISODate))
                << u'\n';
    m_logStream << QStringLiteral("=").repeated(headerMessage.size()) << u'\n';
    m_logStream << u'\n';
    m_logStream.flush();

    qCDebug(lcLogging) << "Writing to" << path;
}

void Logger::writeToFile(const QString &line) {
    const QMutexLocker locker(&m_fileMutex);

    if (m_logStream.device() == nullptr) {
        return;
    }

    m_logStream << line << u'\n';
    m_logStream.flush();
}

void Logger::appendEntry(Entry entry) {
    m_appendingEntry = true;

    const auto guard = qScopeGuard([this] {
        m_appendingEntry = false;
    });

    const bool isFull = m_entries.size() >= m_maxEntries;

    if (isFull) {
        beginRemoveRows(QModelIndex(), 0, 0);
        m_entries.removeFirst();
        endRemoveRows();
    }

    const int row = m_entries.size();

    beginInsertRows(QModelIndex(), row, row);
    m_entries.append(std::move(entry));
    endInsertRows();

    if (!isFull) {
        emit countChanged();
    }
}
