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

    friend class Singleton<Player>;

public:
    enum class State { Stopped, Loading, Playing };
    Q_ENUM(State)

private:
    enum class AsyncCommandId {
        None,
        Stop,
        StopAndSetStation,
        GetFilenameAndFilterNowPlaying
    };

    MpvController *m_mpvController = nullptr;
    QThread *m_workerThread = nullptr;

    QString m_pendingNowPlaying;
    Station m_pendingStation;
    bool m_pendingPlay = false;

    Station m_station;
    QString m_nowPlaying;

    State m_state = State::Stopped;
    QString m_elapsed = QStringLiteral("00:00:00");

    explicit Player(QObject *parent = nullptr);

    void setupConnections() const;
    void setupObservations() const;

    void observeProperty(const QString &property, mpv_format format,
                         uint64_t id = 0) const;
    void getPropertyAsync(const QString &property, AsyncCommandId id) const;

    void commandAsync(const QStringList &params,
                      AsyncCommandId id = AsyncCommandId::None) const;

    void setNowPlaying(QString newNowPlaying);

    void setState(const State &newState);
    QString formatTime(const double &time) const;
    void setElapsed(const QString &newElapsed);

    void stop(AsyncCommandId id) const;

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

public slots:
    void setStation(const Station &newStation, const bool &play = false);

    void play() const;
    void stop() const;

signals:
    void stationChanged();
    void nowPlayingChanged();

    void stateChanged();
    void elapsedChanged();
};
