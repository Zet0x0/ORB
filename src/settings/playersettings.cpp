#include "playersettings.h"
#include "settingscategories.h"
#include "settingsio.h"

namespace {
constexpr int VolumeMin = 0;
constexpr int VolumeMax = 100;

constexpr int MaxRetriesMin = 1;
constexpr int MaxRetriesMax = 99;
}

PlayerSettings::PlayerSettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("player")) {
    m_lastStation = Station::fromMap(
        m_settings->value(QStringLiteral("lastStation")).toMap());

    m_volume = SettingsIO::readInt(m_settings, QStringLiteral("volume"), 100,
                                   VolumeMin, VolumeMax);
    m_muted = SettingsIO::readBool(m_settings, QStringLiteral("muted"), false);

    m_retryOnError =
        SettingsIO::readBool(m_settings, QStringLiteral("retryOnError"), true);
    m_maxRetries = SettingsIO::readInt(m_settings, QStringLiteral("maxRetries"),
                                       5, MaxRetriesMin, MaxRetriesMax);
}

QList<SettingsFieldMeta> PlayerSettings::settingsFields() const {
    return {
        {.propertyName = "retryOnError",
         .label = tr("Retry on error"),
         .category = SettingsCategory::Playback,
         .subcategory = SettingsSubcategory::General},
        {.propertyName = "maxRetries",
         .label = tr("Retry attempts"),
         .category = SettingsCategory::Playback,
         .subcategory = SettingsSubcategory::General,
         .min = MaxRetriesMin,
         .max = MaxRetriesMax},
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
    newVolume = qBound(VolumeMin, newVolume, VolumeMax);

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
    newMaxRetries = qBound(MaxRetriesMin, newMaxRetries, MaxRetriesMax);

    if (m_maxRetries == newMaxRetries) {
        return;
    }

    m_maxRetries = newMaxRetries;
    SettingsIO::write(m_settings, QStringLiteral("maxRetries"), m_maxRetries);

    emit maxRetriesChanged();
}
