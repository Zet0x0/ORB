#include "settingsgroup.h"

SettingsGroup::SettingsGroup(QObject *parent) : QObject(parent) {}

QString SettingsGroup::settingsSubcategory() const {
    return QString();
}

QList<SettingsFieldMeta> SettingsGroup::settingsFields() const {
    return {};
}
