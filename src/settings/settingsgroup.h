#pragma once

#include "settingsfieldmeta.h"
#include <QObject>

class SettingsGroup : public QObject {
    Q_OBJECT

public:
    explicit SettingsGroup(QObject *parent = nullptr);

    // QT_TRANSLATE_NOOP("SettingsCategory", "...")
    virtual QByteArray settingsCategory() const;
    virtual QByteArray settingsSubcategory() const;

    virtual QList<SettingsFieldMeta> settingsFields() const;
};
