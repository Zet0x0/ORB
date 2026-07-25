#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>
#include <QSettings>

class TraySettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)

    Q_PROPERTY(bool closeToTray READ closeToTray WRITE setCloseToTray NOTIFY
                   closeToTrayChanged FINAL)

private:
    QSettings *m_settings;

    bool m_enabled;

    bool m_closeToTray;

public:
    explicit TraySettings(QObject *parent = nullptr);

    QString settingsCategory() const override;
    QString settingsSubcategory() const override;
    QList<SettingsFieldMeta> settingsFields() const override;

    bool enabled() const;
    void setEnabled(bool newEnabled);

    bool closeToTray() const;
    void setCloseToTray(bool newCloseToTray);

signals:
    void enabledChanged();

    void closeToTrayChanged();
};
