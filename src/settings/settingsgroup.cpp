#include "settingsgroup.h"
#include "settingsfactory.h"

SettingsGroup::SettingsGroup(QObject *parent, const QString &section)
    : QObject(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(section);
}

QList<SettingsFieldMeta> SettingsGroup::settingsFields() const {
    return {};
}
