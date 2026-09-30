#pragma once

#include "settingsfieldmeta.h"
#include <QObject>
#include <QSettings>

class SettingsGroup : public QObject {
    Q_OBJECT

protected:
    QSettings *m_settings;

public:
    explicit SettingsGroup(QObject *parent, const QString &section);

    virtual QList<SettingsFieldMeta> settingsFields() const;
};
