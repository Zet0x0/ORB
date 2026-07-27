#include "traysettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

TraySettings::TraySettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("tray"));

    m_enabled =
        SettingsIO::readBool(m_settings, QStringLiteral("enabled"), true);

    m_closeToTray =
        SettingsIO::readBool(m_settings, QStringLiteral("closeToTray"), false);
}

QString TraySettings::settingsCategory() const {
    return tr("System");
}

QString TraySettings::settingsSubcategory() const {
    return tr("Tray");
}

QList<SettingsFieldMeta> TraySettings::settingsFields() const {
    return {
        {"enabled", tr("Enabled")},
        {"closeToTray", tr("Close window to tray")},
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
