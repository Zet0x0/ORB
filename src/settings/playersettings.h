#pragma once

#include "../sources/station.h"
#include "settingsfieldmeta.h"
#include "settingsgroup.h"
#include <QList>
#include <QQmlEngine>

class PlayerSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Station lastStation READ lastStation WRITE setLastStation NOTIFY
                   lastStationChanged FINAL)

    Q_PROPERTY(
        int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)

    Q_PROPERTY(bool retryOnError READ retryOnError WRITE setRetryOnError NOTIFY
                   retryOnErrorChanged FINAL)
    Q_PROPERTY(int maxRetries READ maxRetries WRITE setMaxRetries NOTIFY
                   maxRetriesChanged FINAL)

private:
    Station m_lastStation;

    int m_volume;
    bool m_muted;

    bool m_retryOnError;
    int m_maxRetries;

public:
    explicit PlayerSettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    Station lastStation() const;
    void setLastStation(const Station &newLastStation);

    int volume() const;
    void setVolume(int newVolume);
    bool muted() const;
    void setMuted(bool newMuted);

    bool retryOnError() const;
    void setRetryOnError(bool newRetryOnError);
    int maxRetries() const;
    void setMaxRetries(int newMaxRetries);

signals:
    void lastStationChanged();

    void volumeChanged();
    void mutedChanged();

    void retryOnErrorChanged();
    void maxRetriesChanged();
};
