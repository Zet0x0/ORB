#include "settingsgroup.h"
#include "../logging/logcategories.h"
#include "settingsfactory.h"

SettingsGroup::SettingsGroup(QObject *parent, const QString &section)
    : QObject(parent), m_settings(SettingsFactory::create(this)) {
    if (m_settings->status() != QSettings::NoError) {
        qCWarning(lcSettings) << "Cannot read section" << section << "of"
                              << m_settings->fileName() << m_settings->status();
    }

    m_settings->beginGroup(section);
}

QList<SettingsFieldMeta> SettingsGroup::settingsFields() const {
    return {};
}
