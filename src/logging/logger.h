#pragma once

#include "../common/singleton.h"
#include <QAbstractTableModel>
#include <QDateTime>
#include <QFile>
#include <QList>
#include <QMutex>
#include <QTextStream>
#include <QThread>
#include <QUrl>

class Logger : public QAbstractTableModel, public Singleton<Logger> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(QUrl directoryUrl READ directoryUrl CONSTANT FINAL)

    friend class Singleton<Logger>;

public:
    enum Level { Debug, Info, Warning, Critical, Fatal };
    Q_ENUM(Level)

    enum Column {
        TimeColumn,
        LevelColumn,
        CategoryColumn,
        MessageColumn,
        ColumnCount
    };
    Q_ENUM(Column)

    // Raw values, for sorting and copying all of logs
    enum Role { TimestampRole = Qt::UserRole, LevelRole, LineTextRole };
    Q_ENUM(Role)

private:
    struct Entry {
        QDateTime timestamp;
        Level level = Info;
        QString category;
        QString message;
    };

    QList<Entry> m_entries;
    int m_maxEntries = 2000;

    QtMessageHandler m_previousHandler = nullptr;
    QThread *m_guiThread = nullptr;

    bool m_appendingEntry = false; // GUI thread only

    QMutex m_fileMutex;
    QFile m_logFile;
    QTextStream m_logStream;

    explicit Logger(QObject *parent = nullptr);

    static QString directoryPath();

    static QString levelName(Level level);
    static QString formatLine(const QDateTime &timestamp, Level level,
                              const QString &category, const QString &message);

    static Level levelFromMsgType(QtMsgType type);
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &message);

    void openLogFile();
    void writeToFile(const QString &line);

    void appendEntry(Entry entry);

public:
    static void install();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    QUrl directoryUrl() const;

    void setMaxEntries(int newMaxEntries);
    void setMaxFiles(int maxFiles);

    // uses QLoggingCategory::setFilterRules
    static void setFilterRules(const QString &rules);

signals:
    void countChanged();
};
