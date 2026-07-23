#pragma once

#include "../sources/station.h"
#include <QQmlEngine>
#include <QSettings>

class PlayerSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Station lastStation READ lastStation WRITE setLastStation NOTIFY
                   lastStationChanged FINAL)

    Q_PROPERTY(
        int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)

private:
    QSettings *m_settings;

    Station m_lastStation;

    int m_volume;
    bool m_muted;

public:
    explicit PlayerSettings(QObject *parent = nullptr);

    Station lastStation() const;
    void setLastStation(const Station &newLastStation);

    int volume() const;
    void setVolume(int newVolume);
    bool muted() const;
    void setMuted(bool newMuted);

signals:
    void lastStationChanged();

    void volumeChanged();
    void mutedChanged();
};
