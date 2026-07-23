#pragma once

#include "../sources/station.h"
#include <QQmlEngine>
#include <QSettings>

class PlayerSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Station lastStation READ lastStation WRITE setLastStation NOTIFY
                   lastStationChanged FINAL)

private:
    QSettings *m_settings;

    Station m_lastStation;

public:
    explicit PlayerSettings(QObject *parent = nullptr);

    Station lastStation() const;
    void setLastStation(const Station &newLastStation);

signals:
    void lastStationChanged();
};
