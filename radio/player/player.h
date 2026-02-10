#pragma once

#include "../singleton.h"
#include "station.h"
#include <MpvController>
#include <QQmlEngine>
#include <QThread>

class Player : public QObject, public Singleton<Player> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Station *station READ station WRITE setStation NOTIFY
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
    enum class AsyncCommandId { None, Stop };

    MpvController *m_mpvController = nullptr;
    QThread *m_workerThread = nullptr;

    Station *m_station = nullptr;
    QString m_nowPlaying;

    Player::State m_state = Player::State::Stopped;
    QString m_elapsed = QStringLiteral("00:00:00");

    explicit Player(QObject *parent = nullptr);

    void setupConnections() const;

    void setupObservations() const;
    void observeProperty(const QString &property, mpv_format format,
                         uint64_t id = 0) const;

    void commandAsync(
        const QStringList &params,
        Player::AsyncCommandId id = Player::AsyncCommandId::None) const;

    void setNowPlaying(QString newNowPlaying);

    void setState(const Player::State &newState);
    QString formatTime(const double &time) const;
    void setElapsed(const QString &newElapsed);

private slots:
    void onPropertyChanged(const QString &property, const QVariant &value);

    void onAsyncReply(const QVariant &data, mpv_event event);

    void onEndFile(QString reason);
    void onFileStarted();
    void onFileLoaded();

public:
    ~Player();

    Station *station() const;
    void setStation(Station *newStation);
    QString nowPlaying() const;

    Player::State state() const;
    QString elapsed() const;

public slots:
    void play() const;
    void stop() const;

signals:
    void stationChanged();
    void nowPlayingChanged();

    void stateChanged();
    void elapsedChanged();
};
