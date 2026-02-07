#include "player.h"
#include "mpvproperties.h"

void Player::setupObservations() {
    connect(m_mpvController, &MpvController::propertyChanged, this,
            &Player::onPropertyChanged, Qt::QueuedConnection);

    observeProperty(MpvProperties::NowPlaying, MPV_FORMAT_STRING);
    observeProperty(MpvProperties::Elapsed, MPV_FORMAT_DOUBLE);
}

void Player::observeProperty(const QString &property, mpv_format format,
                             uint64_t id) {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::observeProperty,
                              Qt::QueuedConnection, property, format, id);
}

void Player::setNowPlaying(QString newNowPlaying) {
    newNowPlaying = newNowPlaying.trimmed();

    if (m_nowPlaying == newNowPlaying) {
        return;
    }

    m_nowPlaying = newNowPlaying;

    emit nowPlayingChanged();
}

// taken from MpvQt examples & slightly modified
QString Player::formatTime(const double &time) {
    const int totalNumberOfSeconds = static_cast<int>(time);

    const int seconds = totalNumberOfSeconds % 60;
    const int minutes = (totalNumberOfSeconds / 60) % 60;
    const int hours = totalNumberOfSeconds / 60 / 60;

    return QStringLiteral("%0:%1:%2")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void Player::setElapsed(const QString &newElapsed) {
    if (m_elapsed == newElapsed) {
        return;
    }

    m_elapsed = newElapsed;

    emit elapsedChanged();
}

Player::Player(QObject *parent)
    : QObject(parent), m_workerThread(new QThread(this)) {
    m_mpvController = new MpvController;

    connect(m_workerThread, &QThread::finished, m_mpvController,
            &QObject::deleteLater);

    m_mpvController->moveToThread(m_workerThread);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_mpvController, &MpvController::init,
                              Qt::BlockingQueuedConnection);

    m_mpvController->commandAsync(QStringList{
        "loadfile", "https://stream.bigfm.de/hiphop/mp3-128/radiode"});

    connect(this, &Player::nowPlayingChanged, this, [this] {
        qDebug() << "nowplaying:" << m_nowPlaying;
    });
    connect(this, &Player::elapsedChanged, this, [this] {
        qDebug() << "elapsed:" << m_elapsed;
    });

    setupObservations();
}

Player::~Player() {
    m_workerThread->quit();
    m_workerThread->wait();

    mpv_terminate_destroy(m_mpvController->mpv());

    m_workerThread->deleteLater();
}

Player *Player::instance() {
    static Player *instance = new Player;

    return instance;
}

QString Player::nowPlaying() const {
    return m_nowPlaying;
}

QString Player::elapsed() const {
    return m_elapsed;
}

void Player::onPropertyChanged(const QString &property, const QVariant &value) {
    if (property == MpvProperties::NowPlaying) {
        setNowPlaying(value.toString());
    } else if (property == MpvProperties::Elapsed) {
        setElapsed(formatTime(value.toDouble()));
    }
}
