#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>
#include <QSettings>

class LoggingSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int maxEntries READ maxEntries WRITE setMaxEntries NOTIFY
                   maxEntriesChanged FINAL)

private:
    QSettings *m_settings;

    int m_maxEntries;

public:
    explicit LoggingSettings(QObject *parent = nullptr);

    QByteArray settingsCategory() const override;
    QByteArray settingsSubcategory() const override;
    QList<SettingsFieldMeta> settingsFields() const override;

    int maxEntries() const;
    void setMaxEntries(int newMaxEntries);

signals:
    void maxEntriesChanged();
};
