#pragma once

#include <QQmlEngine>
#include <QSettings>

class TraySettings : public QObject {
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

    bool enabled() const;
    void setEnabled(bool newEnabled);

    bool closeToTray() const;
    void setCloseToTray(bool newCloseToTray);

signals:
    void enabledChanged();

    void closeToTrayChanged();
};
