#include "traysettings.h"
#include "settingscategories.h"
#include "settingsio.h"

TraySettings::TraySettings(QObject *parent)
    : SettingsGroup(parent, QStringLiteral("tray")) {
    m_enabled =
        SettingsIO::readBool(m_settings, QStringLiteral("enabled"), true);

    m_closeToTray =
        SettingsIO::readBool(m_settings, QStringLiteral("closeToTray"), false);
}

QList<SettingsFieldMeta> TraySettings::settingsFields() const {
    return {
        {.propertyName = "enabled",
         .label = tr("Enabled"),
         .category = SettingsCategory::System,
         .subcategory = SettingsSubcategory::Tray},
        {.propertyName = "closeToTray",
         .label = tr("Close to tray"),
         .category = SettingsCategory::System,
         .subcategory = SettingsSubcategory::Tray},
    };
}

bool TraySettings::enabled() const {
    return m_enabled;
}

void TraySettings::setEnabled(bool newEnabled) {
    if (m_enabled == newEnabled) {
        return;
    }

    m_enabled = newEnabled;
    SettingsIO::write(m_settings, QStringLiteral("enabled"), m_enabled);

    emit enabledChanged();
}

bool TraySettings::closeToTray() const {
    return m_closeToTray;
}

void TraySettings::setCloseToTray(bool newCloseToTray) {
    if (m_closeToTray == newCloseToTray) {
        return;
    }

    m_closeToTray = newCloseToTray;
    SettingsIO::write(m_settings, QStringLiteral("closeToTray"), m_closeToTray);

    emit closeToTrayChanged();
}
