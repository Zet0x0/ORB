#pragma once

#include "settingsfieldmeta.h"
#include "settingsgroup.h"
#include <QList>
#include <QQmlEngine>

class TraySettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)

    Q_PROPERTY(bool closeToTray READ closeToTray WRITE setCloseToTray NOTIFY
                   closeToTrayChanged FINAL)

private:
    bool m_enabled;

    bool m_closeToTray;

public:
    explicit TraySettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    bool enabled() const;
    void setEnabled(bool newEnabled);

    bool closeToTray() const;
    void setCloseToTray(bool newCloseToTray);

signals:
    void enabledChanged();

    void closeToTrayChanged();
};
