#include "player.h"
#include "../common/utilities.h"
#include "../logging/logcategories.h"
#include "../settings/playersettings.h"
#include "../settings/settings.h"
#include <QCoreApplication>
#include <utility>

namespace {
constexpr int RetryMaxDelaySeconds = 30;
constexpr int StabilityThresholdMs = 15000;
}

Player::Player(QObject *parent)
    : QObject(parent), m_mpv(new Mpv(this)), m_retryTimer(new QTimer(this)),
      m_stabilityTimer(new QTimer(this)) {
    m_retryTimer->setInterval(1000);
    connect(m_retryTimer, &QTimer::timeout, this, &Player::onRetryTick);

    m_stabilityTimer->setSingleShot(true);
    m_stabilityTimer->setInterval(StabilityThresholdMs);
    connect(m_stabilityTimer, &QTimer::timeout, this,
            &Player::onPlaybackStable);

    connect(m_mpv, &Mpv::fileStarted, this, &Player::onFileStarted);
    connect(m_mpv, &Mpv::fileLoaded, this, &Player::onFileLoaded);
    connect(m_mpv, &Mpv::fileEnded, this, &Player::onFileEnded);

    connect(m_mpv, &Mpv::titleChanged, this, [this](const QString &title) {
        setNowPlaying(Utilities::escapeControlCharacters(title));
    });
    connect(m_mpv, &Mpv::timePosChanged, this, [this](double seconds) {
        setElapsed(formatTime(seconds));
    });

    connect(qApp, &QCoreApplication::aboutToQuit, this, &Player::shutdown);
}

void Player::shutdown() {
    m_mpv->shutdown();

    m_retryTimer->stop();
    m_stabilityTimer->stop();
}

void Player::setNowPlaying(QString newNowPlaying) {
    newNowPlaying = newNowPlaying.trimmed();

    if (m_nowPlaying == newNowPlaying) {
        return;
    }

    m_nowPlaying = newNowPlaying;

    if (!m_nowPlaying.isEmpty()) {
        qCInfo(lcPlayer) << "Now playing" << m_nowPlaying;
    }

    emit nowPlayingChanged();
}

void Player::setState(const State &newState) {
    if (m_state == newState) {
        return;
    }

    qCDebug(lcPlayer) << "State changed from" << m_state << "to" << newState;

    m_state = newState;

    emit stateChanged();
}

QString Player::formatTime(double time) const {
    const int totalNumberOfSeconds = static_cast<int>(time);

    const int seconds = totalNumberOfSeconds % 60;
    const int minutes = (totalNumberOfSeconds / 60) % 60;
    const int hours = totalNumberOfSeconds / 60 / 60;

    return QStringLiteral("%0:%1:%2")
        .arg(hours, 2, 10, u'0')
        .arg(minutes, 2, 10, u'0')
        .arg(seconds, 2, 10, u'0');
}

void Player::setElapsed(const QString &newElapsed) {
    if (m_elapsed == newElapsed) {
        return;
    }

    m_elapsed = newElapsed;

    emit elapsedChanged();
}

void Player::setError(const ErrorInfo &error) {
    if (m_error == error) {
        return;
    }

    m_error = error;

    emit errorChanged();
}

void Player::raiseError(const QString &title, const QString &message) {
    qCWarning(lcPlayer).noquote().nospace() << title << ": " << message;

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

void Player::onStoppedForStationChange(const QString &error) {
    const std::optional<PendingStationChange> pending =
        std::exchange(m_pendingStationChange, std::nullopt);

    if (!error.isEmpty()) {
        raiseError(tr("Playback error"),
                   tr("Failed to stop playback (%0)").arg(error));

        return;
    }

    setState(State::Stopped);

    if (pending) {
        setStation(pending->station, pending->shouldPlay);
    }
}

void Player::onFileEnded(bool failed, const QString &reason) {
    m_stabilityTimer->stop();

    if (failed && shouldRetry()) {
        setRetryAttempt(m_retryAttempt + 1);

        const int delay = retryDelaySeconds();

        qCWarning(lcPlayer).noquote().nospace()
            << "Playback ended (" << reason << "), retry " << m_retryAttempt
            << " of " << Settings::instance()->player()->maxRetries() << " in "
            << delay << "s";

        setState(State::Retrying);
        startRetryCountdown(delay);

        return;
    }

    if (failed) {
        QString message;

        if (m_retryAttempt > 0) {
            message = tr("Unable to play the station after %n retries (%0)",
                         nullptr, m_retryAttempt)
                          .arg(reason);
        } else {
            message = tr("An error occurred trying to play the station (%0)")
                          .arg(reason);
        }

        raiseError(tr("Playback error"), message);
    }

    setRetryAttempt(0);

    setState(State::Stopped);
}

void Player::onFileStarted() {
    clearError();

    setState(State::Loading);
}

void Player::onFileLoaded() {
    qCInfo(lcPlayer) << "Playing" << m_station.name();

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

        qCInfo(lcPlayer) << "Retrying, attempt" << m_retryAttempt;

        play();

        return;
    }

    setRetrySecondsRemaining(remaining);
}

void Player::onPlaybackStable() {
    if (m_retryAttempt > 0) {
        qCDebug(lcPlayer) << "Playback stable, resetting retry attempts";
    }

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

QString Player::mpvVersion() const {
    return m_mpv->version();
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
            qCDebug(lcPlayer) << "Stopping before switching station";

            m_mpv->stop([this](const QString &error) {
                onStoppedForStationChange(error);
            });
        }

        return;
    }

    if (m_station != newStation) {
        m_station = newStation;

        qCInfo(lcPlayer) << "Current station is now" << m_station.name()
                         << m_station.streamUrl();

        emit stationChanged();
    }

    if (playImmediately) {
        play();
    }
}

void Player::play() {
    qCInfo(lcPlayer) << "Loading" << m_station.streamUrl();

    m_mpv->loadFile(m_station.streamUrl(), [](const QString &error) {
        if (!error.isEmpty()) {
            qCWarning(lcPlayer) << "loadfile failed:" << error;
        }
    });
}

void Player::stop() {
    qCInfo(lcPlayer) << "Stopping";

    cancelRetry();

    m_mpv->stop([this](const QString &error) {
        if (error.isEmpty()) {
            setState(State::Stopped);
        } else {
            raiseError(tr("Playback error"),
                       tr("Failed to stop playback (%0)").arg(error));
        }
    });
}

void Player::setVolume(int newVolume) {
    newVolume = qBound(0, newVolume, 100);

    if (m_volume == newVolume) {
        return;
    }

    m_mpv->setVolume(
        newVolume, [this, previous = m_volume](const QString &error) {
            if (error.isEmpty()) {
                return;
            }

            raiseError(tr("Audio error"),
                       tr("Failed to change the volume (%0)").arg(error));

            m_volume = previous;

            emit volumeChanged();
        });

    m_volume = newVolume;

    emit volumeChanged();

    setMuted(false);
}

void Player::setMuted(bool newMuted) {
    if (m_muted == newMuted) {
        return;
    }

    m_mpv->setMuted(newMuted, [this, previous = m_muted](const QString &error) {
        if (error.isEmpty()) {
            return;
        }

        raiseError(tr("Audio error"),
                   tr("Failed to mute the audio (%0)").arg(error));

        m_muted = previous;

        emit mutedChanged();
    });

    m_muted = newMuted;

    emit mutedChanged();
}

void Player::clearError() {
    setError(ErrorInfo{});
}

void Player::retryNow() {
    if (m_state != State::Retrying) {
        return;
    }

    qCInfo(lcPlayer) << "Retrying now, attempt" << m_retryAttempt;

    stopRetryCountdown();
    play();
}
