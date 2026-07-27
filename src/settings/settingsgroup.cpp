#include "settingsgroup.h"

SettingsGroup::SettingsGroup(QObject *parent) : QObject(parent) {}

QString SettingsGroup::settingsCategory() const {
    return tr("Invalid");
}

QString SettingsGroup::settingsSubcategory() const {
    return QString();
}

QList<SettingsFieldMeta> SettingsGroup::settingsFields() const {
    return {};
}
