#include "player.h"
#include "../common/utilities.h"
#include "../settings/settings.h"
#include "mpvproperties.h"
#include <QCoreApplication>

namespace {
constexpr int RetryMaxDelaySeconds = 30;
constexpr int StabilityThresholdMs = 15000;
}

Player::Player(QObject *parent)
    : QObject(parent), m_mpvController(new MpvController),
      m_workerThread(new QThread(this)), m_retryTimer(new QTimer(this)),
      m_stabilityTimer(new QTimer(this)) {
    connect(m_workerThread, &QThread::finished, m_mpvController,
            &QObject::deleteLater, Qt::QueuedConnection);

    m_mpvController->moveToThread(m_workerThread);

    m_workerThread->start();

    QMetaObject::invokeMethod(m_mpvController, &MpvController::init,
                              Qt::BlockingQueuedConnection);

    m_retryTimer->setInterval(1000);
    connect(m_retryTimer, &QTimer::timeout, this, &Player::onRetryTick);

    m_stabilityTimer->setSingleShot(true);
    m_stabilityTimer->setInterval(StabilityThresholdMs);
    connect(m_stabilityTimer, &QTimer::timeout, this,
            &Player::onPlaybackStable);

    setupConnections();
    setupObservations();

    connect(qApp, &QCoreApplication::aboutToQuit, this, &Player::shutdown);
}

void Player::shutdown() {
    if (m_shutDown) {
        return;
    }

    m_shutDown = true;

    m_retryTimer->stop();
    m_stabilityTimer->stop();

    m_workerThread->quit();
    m_workerThread->wait();
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
    observePropertyAsync(MpvProperties::NowPlaying, MPV_FORMAT_STRING);
    observePropertyAsync(MpvProperties::Elapsed, MPV_FORMAT_DOUBLE);
}

void Player::observePropertyAsync(const QString &property, mpv_format format,
                                  AsyncReplyId id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::observeProperty,
                              Qt::QueuedConnection, property, format,
                              static_cast<uint64_t>(id));
}

void Player::getPropertyAsync(const QString &property, AsyncReplyId id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::getPropertyAsync,
                              Qt::QueuedConnection, property,
                              static_cast<int>(id));
}

void Player::commandAsync(const QStringList &params, AsyncReplyId id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::commandAsync,
                              Qt::QueuedConnection, params,
                              static_cast<int>(id));
}

void Player::setPropertyAsync(const QString &property, const QVariant &value,
                              AsyncReplyId id) const {
    QMetaObject::invokeMethod(m_mpvController, &MpvController::setPropertyAsync,
                              Qt::QueuedConnection, property, value,
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

QString Player::formatTime(double time) const {
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

void Player::sendStop(AsyncReplyId id) const {
    commandAsync({QStringLiteral("stop")}, id);
}

void Player::setError(const ErrorInfo &error) {
    if (m_error == error) {
        return;
    }

    m_error = error;

    emit errorChanged();
}

void Player::raiseError(const QString &title, const QString &message) {
    setError(ErrorInfo(title, message));
}

bool Player::shouldRetry() const {
    const PlayerSettings *settings = Settings::instance()->player();

    return settings->retryOnError() && m_retryAttempt < settings->maxRetries();
}

int Player::retryDelaySeconds() const {
    const int attempt = qMax(1, m_retryAttempt);
    const int delay = 1 << qMin(attempt - 1, 10);

    return qMin(delay, RetryMaxDelaySeconds);
}

void Player::cancelRetry() {
    m_stabilityTimer->stop();

    stopRetryCountdown();
    setRetryAttempt(0);
}

void Player::setRetryAttempt(int newRetryAttempt) {
    if (m_retryAttempt == newRetryAttempt) {
        return;
    }

    m_retryAttempt = newRetryAttempt;

    emit retryAttemptChanged();
}

void Player::setRetrySecondsRemaining(int newRetrySecondsRemaining) {
    if (m_retrySecondsRemaining == newRetrySecondsRemaining) {
        return;
    }

    m_retrySecondsRemaining = newRetrySecondsRemaining;

    emit retrySecondsRemainingChanged();
}

void Player::startRetryCountdown(int seconds) {
    setRetrySecondsRemaining(seconds);
    m_retryTimer->start();
}

void Player::stopRetryCountdown() {
    m_retryTimer->stop();

    setRetrySecondsRemaining(0);
}

void Player::onPropertyChanged(const QString &property, const QVariant &value) {
    if (property == MpvProperties::NowPlaying) {
        const bool alreadyResolving = m_pendingNowPlaying.has_value();
        m_pendingNowPlaying = value.toString();

        if (!alreadyResolving) {
            getPropertyAsync(MpvProperties::Filename,
                             AsyncReplyId::ResolvingNowPlaying);
        }
    } else if (property == MpvProperties::Elapsed) {
        setElapsed(formatTime(value.toDouble()));
    }
}

void Player::onAsyncReply(const QVariant &data, mpv_event event) {
    const AsyncReplyId id = static_cast<AsyncReplyId>(event.reply_userdata);
    const int error = event.error;
    const bool succeeded = error > -1;

    switch (id) {
    case AsyncReplyId::None:
        break;

    case AsyncReplyId::Stopping: {
        if (succeeded) {
            setState(State::Stopped);
        } else {
            raiseError(tr("Playback error"),
                       tr("Failed to stop playback (%0)")
                           .arg(MpvController::getError(error)));
        }

        break;
    }

    case AsyncReplyId::StoppingForStationChange: {
        const std::optional<PendingStationChange> pending =
            std::exchange(m_pendingStationChange, std::nullopt);

        if (succeeded) {
            setState(State::Stopped);

            if (pending) {
                setStation(pending->station, pending->shouldPlay);
            }
        } else {
            raiseError(tr("Playback error"),
                       tr("Failed to stop playback (%0)")
                           .arg(MpvController::getError(error)));
        }

        break;
    }

    case AsyncReplyId::ResolvingNowPlaying: {
        const QString pendingNowPlaying =
            std::exchange(m_pendingNowPlaying, std::nullopt)
                .value_or(QString());

        setNowPlaying(
            data.toString() == pendingNowPlaying
                ? QString()
                : Utilities::escapeControlCharacters(pendingNowPlaying));

        break;
    }

    case AsyncReplyId::SettingVolume: {
        if (!succeeded) {
            raiseError(tr("Audio error"),
                       tr("Failed to change the volume (%0)")
                           .arg(MpvController::getError(error)));

            m_volume = m_previousVolume;

            emit volumeChanged();
        }

        break;
    }

    case AsyncReplyId::SettingMuted: {
        if (!succeeded) {
            raiseError(tr("Audio error"),
                       tr("Failed to mute the audio (%0)")
                           .arg(MpvController::getError(error)));

            m_muted = m_previousMuted;

            emit mutedChanged();
        }

        break;
    }
    }
}

void Player::onEndFile(QString reason) {
    m_stabilityTimer->stop();

    const bool isPlaybackError =
        reason == QStringLiteral("error") || reason == QStringLiteral("eof");

    if (isPlaybackError && shouldRetry()) {
        setRetryAttempt(m_retryAttempt + 1);

        setState(State::Retrying);
        startRetryCountdown(retryDelaySeconds());

        return;
    }

    if (isPlaybackError) {
        raiseError(tr("Playback error"),
                   m_retryAttempt > 0
                       ? tr("Unable to play the station after %0 retries")
                             .arg(m_retryAttempt)
                       : tr("An error occurred trying to play the station"));
    }

    setRetryAttempt(0);

    setState(State::Stopped);
}

void Player::onFileStarted() {
    clearError();

    setState(State::Loading);
}

void Player::onFileLoaded() {
    setState(State::Playing);

    m_stabilityTimer->start();
}

void Player::onRetryTick() {
    if (m_state != State::Retrying) {
        m_retryTimer->stop();

        return;
    }

    const int remaining = m_retrySecondsRemaining - 1;

    if (remaining <= 0) {
        m_retryTimer->stop();
        setRetrySecondsRemaining(0);

        play();

        return;
    }

    setRetrySecondsRemaining(remaining);
}

void Player::onPlaybackStable() {
    setRetryAttempt(0);
}

Player::~Player() {
    shutdown();
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

int Player::volume() const {
    return m_volume;
}

bool Player::muted() const {
    return m_muted;
}

ErrorInfo Player::error() const {
    return m_error;
}

int Player::retryAttempt() const {
    return m_retryAttempt;
}

int Player::retrySecondsRemaining() const {
    return m_retrySecondsRemaining;
}

void Player::setStation(const Station &newStation, bool playImmediately) {
    cancelRetry();

    if (m_state == State::Retrying) {
        setState(State::Stopped);
    }

    if (m_state == State::Playing) {
        const bool alreadyStopping = m_pendingStationChange.has_value();
        m_pendingStationChange =
            PendingStationChange{newStation, playImmediately};

        if (!alreadyStopping) {
            sendStop(AsyncReplyId::StoppingForStationChange);
        }

        return;
    }

    if (m_station != newStation) {
        m_station = newStation;

        emit stationChanged();
    }

    if (playImmediately) {
        play();
    }
}

void Player::play() const {
    commandAsync({QStringLiteral("loadfile"), m_station.streamUrl()});
}

void Player::stop() {
    cancelRetry();

    sendStop(AsyncReplyId::Stopping);
}

void Player::setVolume(int newVolume) {
    newVolume = qBound(0, newVolume, 100);

    if (m_volume == newVolume) {
        return;
    }

    m_previousVolume = m_volume;
    m_volume = newVolume;
    setPropertyAsync(MpvProperties::Volume, m_volume,
                     AsyncReplyId::SettingVolume);

    emit volumeChanged();

    setMuted(false);
}

void Player::setMuted(bool newMuted) {
    if (m_muted == newMuted) {
        return;
    }

    m_previousMuted = m_muted;
    m_muted = newMuted;
    setPropertyAsync(MpvProperties::Mute, m_muted, AsyncReplyId::SettingMuted);

    emit mutedChanged();
}

void Player::clearError() {
    setError(ErrorInfo{});
}

void Player::retryNow() {
    if (m_state != State::Retrying) {
        return;
    }

    stopRetryCountdown();
    play();
}
