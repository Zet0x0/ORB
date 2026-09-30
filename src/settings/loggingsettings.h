#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>

class LoggingSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int maxEntries READ maxEntries WRITE setMaxEntries NOTIFY
                   maxEntriesChanged FINAL)

private:
    int m_maxEntries;

public:
    explicit LoggingSettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    int maxEntries() const;
    void setMaxEntries(int newMaxEntries);

signals:
    void maxEntriesChanged();
};
