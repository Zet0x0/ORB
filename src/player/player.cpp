#include "player.h"
#include "../common/utilities.h"
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
        break;
    }

    case AsyncReplyId::Stopping: {
        if (succeeded) {
            setState(State::Stopped);
        } else {
            // TODO: replace with UI error message
            qWarning() << "mpv stop command failed:" << error;
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
            // TODO: replace with UI error message
            qWarning() << "mpv stop command failed:" << error;
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
    }
}

void Player::onEndFile(QString reason) {
    if (reason == QStringLiteral("eof") || reason == QStringLiteral("error")) {
        // TODO: replace with UI error message
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

void Player::setStation(const Station &newStation, bool play) {
    if (m_state == State::Playing) {
        const bool alreadyStopping = m_pendingStationChange.has_value();
        m_pendingStationChange = PendingStationChange{newStation, play};

        if (!alreadyStopping) {
            sendStop(AsyncReplyId::StoppingForStationChange);
        }

        return;
    }

    if (m_station != newStation) {
        m_station = newStation;

        emit stationChanged();
    }

    if (play) {
        this->play();
    }
}

void Player::play() const {
    commandAsync({QStringLiteral("loadfile"), m_station.streamUrl()});
}

void Player::stop() const {
    sendStop(AsyncReplyId::Stopping);
}
