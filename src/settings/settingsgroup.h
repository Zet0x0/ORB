#pragma once

#include "settingsfieldmeta.h"
#include <QObject>

class SettingsGroup : public QObject {
    Q_OBJECT

public:
    explicit SettingsGroup(QObject *parent = nullptr);

    virtual QString settingsCategory() const = 0;
    virtual QString settingsSubcategory() const;

    virtual QList<SettingsFieldMeta> settingsFields() const;
};
