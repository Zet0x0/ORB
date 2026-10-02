#include "player.h"
#include "../common/logcategories.h"
#include "../common/utilities.h"
#include "../settings/playersettings.h"
#include "../settings/settings.h"
#include "mpvproperties.h"
#include <QByteArray>
#include <QCoreApplication>
#include <QHash>
#include <QLatin1StringView>
#include <QLoggingCategory>
#include <QMetaObject>
#include <cstdint>
#include <mpvqt_version.h>
#include <utility>

namespace {
constexpr int RetryMaxDelaySeconds = 30;
constexpr int StabilityThresholdMs = 15000;

// raw mpv version reads something like "mpv v0.41.0-1017-g02a595ddc"
constexpr QLatin1StringView MpvVersionPrefix("mpv ");

// make or return a saved QLoggingCategory out of a mpv's module
// e.g. ffmpeg/demuxer -> orb.player.mpv.ffmpeg.demuxer
const QLoggingCategory &mpvCategory(const char *module) {
    static QHash<QByteArray, const QLoggingCategory *> categories;

    const QLoggingCategory *&category = categories[QByteArray(module)];

    if (!category) {
        const QByteArray name =
            "orb.player.mpv." + QByteArray(module).replace("/", ".");

        // debug messages are hidden by default because they're mpv's verbose
        // output, which gives out a line for every downloaded stream segment;
        // to show them put a rule like orb.player.mpv.*.debug=true
        category = new QLoggingCategory(qstrdup(name.constData()), QtInfoMsg);
    }

    return *category;
}

QString mpvMessageText(const mpv_event_log_message &message) {
    QString text = QString::fromUtf8(message.text);

    // ends in a newline, sometimes with spaces before it
    while (!text.isEmpty() && text.back().isSpace()) {
        text.chop(1);
    }

    return text;
}

void logMpvMessage(const mpv_event_log_message &message) {
    const QLoggingCategory &category = mpvCategory(message.prefix);

    switch (message.log_level) {
    case MPV_LOG_LEVEL_FATAL:
    case MPV_LOG_LEVEL_ERROR:
        qCCritical(category).noquote() << mpvMessageText(message);

        break;

    case MPV_LOG_LEVEL_WARN:
        qCWarning(category).noquote() << mpvMessageText(message);

        break;

    case MPV_LOG_LEVEL_INFO:
        qCInfo(category).noquote() << mpvMessageText(message);

        break;

    default:
        qCDebug(category).noquote() << mpvMessageText(message);

        break;
    }
}
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

    qCDebug(lcPlayer) << "mpv initialized on worker thread";

    m_retryTimer->setInterval(1000);
    connect(m_retryTimer, &QTimer::timeout, this, &Player::onRetryTick);

    m_stabilityTimer->setSingleShot(true);
    m_stabilityTimer->setInterval(StabilityThresholdMs);
    connect(m_stabilityTimer, &QTimer::timeout, this,
            &Player::onPlaybackStable);

    setupConnections();
    setupObservations();
    setupLogClient();
    readMpvVersion();

    connect(qApp, &QCoreApplication::aboutToQuit, this, &Player::shutdown);
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

void Player::setupLogClient() {
    m_logClient =
        mpv_create_weak_client(m_mpvController->mpv(), "orb_log_reader");

    if (!m_logClient) {
        qCWarning(lcPlayer) << "Failed to create mpv log client";

        return;
    }

    mpv_set_wakeup_callback(
        m_logClient,
        [](void *player) {
            QMetaObject::invokeMethod(static_cast<Player *>(player),
                                      &Player::readLogMessages,
                                      Qt::QueuedConnection);
        },
        this);

    mpv_request_log_messages(m_logClient, "v");
}

// synchronous, so the About dialog can get the version
void Player::readMpvVersion() {
    char *version = nullptr;
    const int error =
        mpv_get_property(m_mpvController->mpv(), MpvProperties::Version.data(),
                         MPV_FORMAT_STRING, &version);

    if (error < 0) {
        qCWarning(lcPlayer) << "Failed to read" << MpvProperties::Version
                            << MpvController::getError(error);

        return;
    }

    m_mpvVersion = QString::fromUtf8(version);
    mpv_free(version);

    if (m_mpvVersion.startsWith(MpvVersionPrefix)) {
        m_mpvVersion.remove(0, MpvVersionPrefix.size());
    }

    qCInfo(lcPlayer).noquote() << "Using mpv" << m_mpvVersion;
}

void Player::destroyLogClient() {
    if (!m_logClient) {
        return;
    }

    mpv_set_wakeup_callback(m_logClient, nullptr, nullptr);
    mpv_destroy(m_logClient);

    m_logClient = nullptr;
}

void Player::shutdown() {
    if (m_shutDown) {
        return;
    }

    m_shutDown = true;

    qCDebug(lcPlayer) << "Shutting down mpv";

    m_retryTimer->stop();
    m_stabilityTimer->stop();

    // mpv_terminate_destroy waits until every other client
    // is destroyed, so our little one has to go first
    readLogMessages();
    destroyLogClient();

    m_workerThread->quit();
    m_workerThread->wait();
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

void Player::readLogMessages() {
    while (m_logClient) {
        const mpv_event *event = mpv_wait_event(m_logClient, 0);

        switch (event->event_id) {
        case MPV_EVENT_NONE:
            return;

        case MPV_EVENT_SHUTDOWN:
            destroyLogClient();

            return;

        case MPV_EVENT_LOG_MESSAGE:
            logMpvMessage(*static_cast<mpv_event_log_message *>(event->data));

            break;

        default:
            break;
        }
    }
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
    case AsyncReplyId::None: {
        if (!succeeded) {
            qCWarning(lcPlayer)
                << "mpv request failed:" << MpvController::getError(error);
        }

        break;
    }

    case AsyncReplyId::LoadingFile: {
        if (!succeeded) {
            qCWarning(lcPlayer)
                << "loadfile failed:" << MpvController::getError(error);
        }

        break;
    }

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

        const int delay = retryDelaySeconds();

        qCWarning(lcPlayer).noquote().nospace()
            << "Playback ended (" << reason << "), retry " << m_retryAttempt
            << " of " << Settings::instance()->player()->maxRetries() << " in "
            << delay << "s";

        setState(State::Retrying);
        startRetryCountdown(delay);

        return;
    }

    if (isPlaybackError) {
        raiseError(tr("Playback error"),
                   m_retryAttempt > 0
                       ? tr("Unable to play the station after %n retries",
                            nullptr, m_retryAttempt)
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
    return m_mpvVersion;
}

QString Player::mpvQtVersion() {
    return QStringLiteral(MPVQT_VERSION_STRING);
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

            sendStop(AsyncReplyId::StoppingForStationChange);
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

void Player::play() const {
    qCInfo(lcPlayer) << "Loading" << m_station.streamUrl();

    commandAsync({QStringLiteral("loadfile"), m_station.streamUrl()},
                 AsyncReplyId::LoadingFile);
}

void Player::stop() {
    qCInfo(lcPlayer) << "Stopping";

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

    qCInfo(lcPlayer) << "Retrying now, attempt" << m_retryAttempt;

    stopRetryCountdown();
    play();
}
