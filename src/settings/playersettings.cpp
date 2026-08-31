#include "playersettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

PlayerSettings::PlayerSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("player"));

    m_lastStation = Station::fromMap(
        m_settings->value(QStringLiteral("lastStation")).toMap());

    m_volume = SettingsIO::readInt(m_settings, QStringLiteral("volume"), 100);
    m_muted = SettingsIO::readBool(m_settings, QStringLiteral("muted"), false);

    m_retryOnError =
        SettingsIO::readBool(m_settings, QStringLiteral("retryOnError"), true);
    m_maxRetries =
        SettingsIO::readInt(m_settings, QStringLiteral("maxRetries"), 5);
}

QString PlayerSettings::settingsCategory() const {
    return tr("Player");
}

QString PlayerSettings::settingsSubcategory() const {
    return tr("Playback");
}

QList<SettingsFieldMeta> PlayerSettings::settingsFields() const {
    return {
        {"retryOnError", tr("Retry on playback error")},
        {"maxRetries", tr("Max retry attempts"), QString(), 1, 99},
    };
}

Station PlayerSettings::lastStation() const {
    return m_lastStation;
}

void PlayerSettings::setLastStation(const Station &newLastStation) {
    if (m_lastStation == newLastStation) {
        return;
    }

    m_lastStation = newLastStation;
    SettingsIO::write(m_settings, QStringLiteral("lastStation"),
                      m_lastStation.toMap());

    emit lastStationChanged();
}

int PlayerSettings::volume() const {
    return m_volume;
}

void PlayerSettings::setVolume(int newVolume) {
    if (m_volume == newVolume) {
        return;
    }

    m_volume = newVolume;
    SettingsIO::write(m_settings, QStringLiteral("volume"), m_volume);

    emit volumeChanged();
}

bool PlayerSettings::muted() const {
    return m_muted;
}

void PlayerSettings::setMuted(bool newMuted) {
    if (m_muted == newMuted) {
        return;
    }

    m_muted = newMuted;
    SettingsIO::write(m_settings, QStringLiteral("muted"), m_muted);

    emit mutedChanged();
}

bool PlayerSettings::retryOnError() const {
    return m_retryOnError;
}

void PlayerSettings::setRetryOnError(bool newRetryOnError) {
    if (m_retryOnError == newRetryOnError) {
        return;
    }

    m_retryOnError = newRetryOnError;
    SettingsIO::write(m_settings, QStringLiteral("retryOnError"),
                      m_retryOnError);

    emit retryOnErrorChanged();
}

int PlayerSettings::maxRetries() const {
    return m_maxRetries;
}

void PlayerSettings::setMaxRetries(int newMaxRetries) {
    newMaxRetries = qMax(0, newMaxRetries);

    if (m_maxRetries == newMaxRetries) {
        return;
    }

    m_maxRetries = newMaxRetries;
    SettingsIO::write(m_settings, QStringLiteral("maxRetries"), m_maxRetries);

    emit maxRetriesChanged();
}
