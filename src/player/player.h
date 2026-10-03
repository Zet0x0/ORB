#pragma once

#include "../common/errorinfo.h"
#include "../common/singleton.h"
#include "../sources/station.h"
#include "mpv.h"
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QTimer>
#include <optional>

class Player : public QObject, public Singleton<Player> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Station station READ station WRITE setStation NOTIFY
                   stationChanged FINAL)
    Q_PROPERTY(
        QString nowPlaying READ nowPlaying NOTIFY nowPlayingChanged FINAL)

    Q_PROPERTY(Player::State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString elapsed READ elapsed NOTIFY elapsedChanged FINAL)

    Q_PROPERTY(
        int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)

    Q_PROPERTY(ErrorInfo error READ error NOTIFY errorChanged FINAL)

    Q_PROPERTY(
        int retryAttempt READ retryAttempt NOTIFY retryAttemptChanged FINAL)
    Q_PROPERTY(int retrySecondsRemaining READ retrySecondsRemaining NOTIFY
                   retrySecondsRemainingChanged FINAL)

    friend class Singleton<Player>;

public:
    enum class State { Stopped, Loading, Playing, Retrying };
    Q_ENUM(State)

    ~Player();

    Station station() const;
    QString nowPlaying() const;

    State state() const;
    QString elapsed() const;

    int volume() const;
    bool muted() const;

    ErrorInfo error() const;

    int retryAttempt() const;
    int retrySecondsRemaining() const;

    Q_INVOKABLE QString mpvVersion() const;

signals:
    void stationChanged();
    void nowPlayingChanged();

    void stateChanged();
    void elapsedChanged();

    void volumeChanged();
    void mutedChanged();

    void errorChanged();

    void retryAttemptChanged();
    void retrySecondsRemainingChanged();

public slots:
    void setStation(const Station &newStation, bool playImmediately = false);

    void play();
    void stop();

    void setVolume(int newVolume);
    void setMuted(bool newMuted);

    void clearError();

    void retryNow();

private:
    struct PendingStationChange {
        Station station;
        bool shouldPlay = false;
    };

    Mpv *m_mpv = nullptr;

    std::optional<PendingStationChange> m_pendingStationChange;

    Station m_station;
    QString m_nowPlaying;

    State m_state = State::Stopped;
    QString m_elapsed = QStringLiteral("00:00:00");

    int m_volume = 100;
    bool m_muted = false;

    ErrorInfo m_error;

    QTimer *m_retryTimer = nullptr;
    QTimer *m_stabilityTimer = nullptr;
    int m_retryAttempt = 0;
    int m_retrySecondsRemaining = 0;

    explicit Player(QObject *parent = nullptr);

    void shutdown();

    void setNowPlaying(QString newNowPlaying);

    void setState(const State &newState);
    QString formatTime(double time) const;
    void setElapsed(const QString &newElapsed);

    void setError(const ErrorInfo &error);
    void raiseError(const QString &title, const QString &message);

    bool shouldRetry() const;
    int retryDelaySeconds() const;
    void cancelRetry();

    void setRetryAttempt(int newRetryAttempt);
    void setRetrySecondsRemaining(int newRetrySecondsRemaining);
    void startRetryCountdown(int seconds);
    void stopRetryCountdown();

    void onStoppedForStationChange(const QString &error);

private slots:
    void onFileStarted();
    void onFileLoaded();
    void onFileEnded(bool failed, const QString &reason);

    void onRetryTick();
    void onPlaybackStable();
};
