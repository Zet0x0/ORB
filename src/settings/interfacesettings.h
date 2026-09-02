#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>
#include <QSettings>

class InterfaceSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool slidingLabels READ slidingLabels WRITE setSlidingLabels
                   NOTIFY slidingLabelsChanged FINAL)

private:
    QSettings *m_settings;

    bool m_slidingLabels;

public:
    explicit InterfaceSettings(QObject *parent = nullptr);

    QString settingsCategory() const override;
    QList<SettingsFieldMeta> settingsFields() const override;

    bool slidingLabels() const;
    void setSlidingLabels(bool newSlidingLabels);

signals:
    void slidingLabelsChanged();
};
