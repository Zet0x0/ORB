#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>

class AppearanceSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool slidingLabels READ slidingLabels WRITE setSlidingLabels
                   NOTIFY slidingLabelsChanged FINAL)

private:
    bool m_slidingLabels;

public:
    explicit AppearanceSettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    bool slidingLabels() const;
    void setSlidingLabels(bool newSlidingLabels);

signals:
    void slidingLabelsChanged();
};
