#include "settingsgroup.h"

SettingsGroup::SettingsGroup(QObject *parent) : QObject(parent) {}

QByteArray SettingsGroup::settingsCategory() const {
    return QT_TRANSLATE_NOOP("SettingsCategory", "Invalid");
}

QByteArray SettingsGroup::settingsSubcategory() const {
    return QByteArray();
}

QList<SettingsFieldMeta> SettingsGroup::settingsFields() const {
    return {};
}
