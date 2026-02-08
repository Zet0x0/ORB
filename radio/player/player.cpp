#include "player.h"
#include "mpvproperties.h"

Player::Player(QObject *parent)
    : QObject(parent), m_workerThread(new QThread(this)) {
    m_mpvController = new MpvController;

    connect(m_workerThread, &QThread::finished, m_mpvController,
            &QObject::deleteLater, Qt::QueuedConnection);

    m_mpvController->moveToThread(m_workerThread);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_mpvController, &MpvController::init,
                              Qt::BlockingQueuedConnection);

    setupConnections();
    setupObservations();
}

void Player::setupConnections() {
    connect(m_mpvController, &MpvController::propertyChanged, this,
            &Player::onPropertyChanged, Qt::QueuedConnection);

    connect(m_mpvController, &MpvController::asyncReply, this,
            &Player::onAsyncReply, Qt::QueuedConnection);

    connect(m_mpvController, &MpvController::endFile, this, &Player::onEndFile,
            Qt::QueuedConnection);
    connect(m_mpvController, &MpvController::fileStarted, this,
            &Player::onFileStarted, Qt::QueuedConnection);
    connect(m_mpvController, &MpvController::fileLoaded, this,
            &Player::onFileLoaded, Qt::QueuedConnection);
}

void Player::setupObservations() {
    observeProperty(MpvProperties::NowPlaying, MPV_FORMAT_STRING);
    observeProperty(MpvProperties::Elapsed, MPV_FORMAT_DOUBLE);
}

void Player::observeProperty(const QString &property, mpv_format format,
                             uint64_t id) {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::observeProperty,
                              Qt::QueuedConnection, property, format, id);
}

void Player::commandAsync(const QStringList &params,
                          Player::AsyncCommandId id) {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::commandAsync,
                              Qt::QueuedConnection, params,
                              static_cast<int>(id));
}

void Player::setNowPlaying(QString newNowPlaying) {
    newNowPlaying = newNowPlaying.trimmed();

    if (m_nowPlaying == newNowPlaying) {
        return;
    }

    m_nowPlaying = newNowPlaying;

    emit nowPlayingChanged();
}

void Player::setState(const Player::State &newState) {
    if (m_state == newState) {
        return;
    }

    m_state = newState;

    emit stateChanged();
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

void Player::onPropertyChanged(const QString &property, const QVariant &value) {
    if (property == MpvProperties::NowPlaying) {
        setNowPlaying(value.toString());
    } else if (property == MpvProperties::Elapsed) {
        setElapsed(formatTime(value.toDouble()));
    }
}

void Player::onAsyncReply(const QVariant &data, mpv_event event) {
    switch (static_cast<AsyncCommandId>(event.reply_userdata)) {
    case AsyncCommandId::None: {
        break;
    }
    case AsyncCommandId::Stop: {
        if (event.error > -1) {
            setState(Player::State::Stopped);
        }

        break;
    }
    }
}

void Player::onEndFile(QString reason) {
    if (reason == QStringLiteral("eof") || reason == QStringLiteral("error")) {
        // TODO: some error handling here
    }

    setState(Player::State::Stopped);
}

void Player::onFileStarted() {
    setState(Player::State::Loading);
}

void Player::onFileLoaded() {
    setState(Player::State::Playing);
}

Player::~Player() {
    m_workerThread->quit();
    m_workerThread->wait();

    mpv_terminate_destroy(m_mpvController->mpv());

    m_workerThread->deleteLater();
}

QString Player::nowPlaying() const {
    return m_nowPlaying;
}

Player::State Player::state() const {
    return m_state;
}

QString Player::elapsed() const {
    return m_elapsed;
}

// TODO: take the stream url from the station object instead
void Player::play() {
    commandAsync(
        {QStringLiteral("loadfile"),
         QStringLiteral("https://stream.bigfm.de/hiphop/mp3-128/radiode")});
}

void Player::stop() {
    commandAsync({QStringLiteral("stop")}, Player::AsyncCommandId::Stop);
}
