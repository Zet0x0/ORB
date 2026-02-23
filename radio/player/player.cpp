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

    // TODO: remove when done testing
    setStation(
        Station("BigFM", "https://stream.bigfm.de/hiphop/mp3-128/radiode",
                "https://static.wixstatic.com/media/"
                "d08c94_434ec752494241e6a53fbd10e2783a87~mv2.jpg/v1/fill/"
                "w_256,h_256,al_c,q_80,usm_0.66_1.00_0.01,enc_avif,quality_"
                "auto/d08c94_434ec752494241e6a53fbd10e2783a87~mv2.jpg"));

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
            setState(State::Stopped);
        }

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

void Player::setStation(const Station &newStation) {
    if (m_station == newStation) {
        return;
    }

    m_station = newStation;

    emit stationChanged();
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

void Player::play() const {
    commandAsync({QStringLiteral("loadfile"), m_station.streamUrl()});
}

void Player::stop() const {
    commandAsync({QStringLiteral("stop")}, AsyncCommandId::Stop);
}
