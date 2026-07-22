#include "playersettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

PlayerSettings::PlayerSettings(QObject *parent)
    : QObject(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("player"));

    m_lastStation = Station::fromMap(
        m_settings->value(QStringLiteral("lastStation")).toMap());
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
