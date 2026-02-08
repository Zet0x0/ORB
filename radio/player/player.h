#pragma once

#include "../singleton.h"
#include <MpvController>
#include <QQmlEngine>
#include <QThread>

class Player : public QObject, public Singleton<Player> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(
        QString nowPlaying READ nowPlaying NOTIFY nowPlayingChanged FINAL)

    // TODO: make use of this in the UI - currently unused
    Q_PROPERTY(QString elapsed READ elapsed NOTIFY elapsedChanged FINAL)

private:
    MpvController *m_mpvController = nullptr;
    QThread *m_workerThread = nullptr;

    QString m_nowPlaying;

    QString m_elapsed;

    friend class Singleton<Player>;
    explicit Player(QObject *parent = nullptr);

    void setupObservations();
    void observeProperty(const QString &property, mpv_format format,
                         uint64_t id = 0);

    void setNowPlaying(QString newNowPlaying);

    QString formatTime(const double &time);
    void setElapsed(const QString &newElapsed);

public:
    ~Player();

    QString nowPlaying() const;

    QString elapsed() const;

signals:
    void nowPlayingChanged();

    void elapsedChanged();

private slots:
    void onPropertyChanged(const QString &property, const QVariant &value);
};
