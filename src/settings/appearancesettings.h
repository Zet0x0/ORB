#pragma once

#include "settingsfieldmeta.h"
#include "settingsgroup.h"
#include <QList>
#include <QQmlEngine>

class AppearanceSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool slidingLabels READ slidingLabels WRITE setSlidingLabels
                   NOTIFY slidingLabelsChanged FINAL)

public:
    explicit AppearanceSettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    bool slidingLabels() const;
    void setSlidingLabels(bool newSlidingLabels);

signals:
    void slidingLabelsChanged();

private:
    bool m_slidingLabels;
};
