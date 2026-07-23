#pragma once

#include "../common/singleton.h"
#include "../sources/station.h"
#include <MpvController>
#include <QQmlEngine>
#include <QThread>

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

    friend class Singleton<Player>;

public:
    enum class State { Stopped, Loading, Playing };
    Q_ENUM(State)

private:
    enum class AsyncReplyId {
        None,
        Stopping,
        StoppingForStationChange,
        ResolvingNowPlaying,
        SettingVolume,
        SettingMuted
    };

    struct PendingStationChange {
        Station station;
        bool shouldPlay = false;
    };

    MpvController *m_mpvController = nullptr;
    QThread *m_workerThread = nullptr;

    std::optional<QString> m_pendingNowPlaying;
    std::optional<PendingStationChange> m_pendingStationChange;

    Station m_station;
    QString m_nowPlaying;

    State m_state = State::Stopped;
    QString m_elapsed = QStringLiteral("00:00:00");

    int m_volume = 100;
    bool m_muted = false;

    int m_previousVolume = 100;
    bool m_previousMuted = false;

    explicit Player(QObject *parent = nullptr);

    void setupConnections() const;
    void setupObservations() const;

    void observePropertyAsync(const QString &property, mpv_format format,
                              AsyncReplyId id = AsyncReplyId::None) const;
    void getPropertyAsync(const QString &property, AsyncReplyId id) const;

    void commandAsync(const QStringList &params,
                      AsyncReplyId id = AsyncReplyId::None) const;

    void setPropertyAsync(const QString &property, const QVariant &value,
                          AsyncReplyId id = AsyncReplyId::None) const;

    void setNowPlaying(QString newNowPlaying);

    void setState(const State &newState);
    QString formatTime(double time) const;
    void setElapsed(const QString &newElapsed);

    void sendStop(AsyncReplyId id) const;

private slots:
    void onPropertyChanged(const QString &property, const QVariant &value);

    void onAsyncReply(const QVariant &data, mpv_event event);

    void onEndFile(QString reason);
    void onFileStarted();
    void onFileLoaded();

public:
    ~Player();

    Station station() const;
    QString nowPlaying() const;

    State state() const;
    QString elapsed() const;

    int volume() const;
    bool muted() const;

public slots:
    void setStation(const Station &newStation, bool play = false);

    void play() const;
    void stop() const;

    void setVolume(int newVolume);
    void setMuted(bool newMuted);

signals:
    void stationChanged();
    void nowPlayingChanged();

    void stateChanged();
    void elapsedChanged();

    void volumeChanged();
    void mutedChanged();
};
