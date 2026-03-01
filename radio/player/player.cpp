#include "player.h"
#include "mpvproperties.h"

Player::Player(QObject *parent)
    : QObject(parent), m_mpvController(new MpvController),
      m_workerThread(new QThread(this)) {
    connect(m_workerThread, &QThread::finished, m_mpvController,
            &QObject::deleteLater, Qt::QueuedConnection);

    m_mpvController->moveToThread(m_workerThread);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_mpvController, &MpvController::init,
                              Qt::BlockingQueuedConnection);

    setupConnections();
    setupObservations();
}

void Player::setupConnections() const {
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

void Player::setupObservations() const {
    observeProperty(MpvProperties::NowPlaying, MPV_FORMAT_STRING);
    observeProperty(MpvProperties::Elapsed, MPV_FORMAT_DOUBLE);
}

void Player::observeProperty(const QString &property, mpv_format format,
                             uint64_t id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::observeProperty,
                              Qt::QueuedConnection, property, format, id);
}

void Player::getPropertyAsync(const QString &property,
                              AsyncCommandId id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::getPropertyAsync,
                              Qt::QueuedConnection, property,
                              static_cast<int>(id));
}

void Player::commandAsync(const QStringList &params, AsyncCommandId id) const {
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

void Player::setState(const State &newState) {
    if (m_state == newState) {
        return;
    }

    m_state = newState;

    emit stateChanged();
}

// taken from MpvQt examples & slightly modified
QString Player::formatTime(const double &time) const {
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

void Player::stop(AsyncCommandId id) const {
    commandAsync({QStringLiteral("stop")}, id);
}

void Player::onPropertyChanged(const QString &property, const QVariant &value) {
    if (property == MpvProperties::NowPlaying) {
        m_pendingNowPlaying = value.toString();

        getPropertyAsync(MpvProperties::Filename,
                         AsyncCommandId::GetFilenameAndFilterNowPlaying);
    } else if (property == MpvProperties::Elapsed) {
        setElapsed(formatTime(value.toDouble()));
    }
}

void Player::onAsyncReply(const QVariant &data, mpv_event event) {
    const AsyncCommandId id = static_cast<AsyncCommandId>(event.reply_userdata);

    if (id == AsyncCommandId::Stop || id == AsyncCommandId::StopAndSetStation) {
        if (event.error > -1) {
            setState(State::Stopped);
        }
    }

    switch (id) {
    case AsyncCommandId::None:
    case AsyncCommandId::Stop: {
        break;
    }

    case AsyncCommandId::StopAndSetStation: {
        setStation(m_pendingStation, m_pendingPlay);
        m_pendingStation = Station{};
        m_pendingPlay = false;

        break;
    }

    case AsyncCommandId::GetFilenameAndFilterNowPlaying: {
        setNowPlaying(data.toString() == m_pendingNowPlaying
                          ? QString()
                          : m_pendingNowPlaying);

        m_pendingNowPlaying.clear();

        break;
    }
    }
}

void Player::onEndFile(QString reason) {
    if (reason == QStringLiteral("eof") || reason == QStringLiteral("error")) {
        // TODO: remove this qCritical when some
        // proper error handling involving the UI
        // is done here
        qCritical() << "playback error";
    }

    setState(State::Stopped);
}

void Player::onFileStarted() {
    setState(State::Loading);
}

void Player::onFileLoaded() {
    setState(State::Playing);
}

Player::~Player() {
    m_workerThread->quit();
    m_workerThread->wait();

    mpv_terminate_destroy(m_mpvController->mpv());

    m_workerThread->deleteLater();
}

Station Player::station() const {
    return m_station;
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

void Player::setStation(const Station &newStation, const bool &play) {
    if (m_station == newStation) {
        return;
    }

    if (m_state == State::Playing) {
        m_pendingStation = newStation;
        m_pendingPlay = play;
        stop(AsyncCommandId::StopAndSetStation);

        return;
    }

    m_station = newStation;

    emit stationChanged();

    if (play) {
        this->play();
    }
}

void Player::play() const {
    commandAsync({QStringLiteral("loadfile"), m_station.streamUrl()});
}

void Player::stop() const {
    stop(AsyncCommandId::Stop);
}
