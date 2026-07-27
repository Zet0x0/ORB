#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>
#include <QSettings>

class SourcesSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString lastSearchSource READ lastSearchSource WRITE
                   setLastSearchSource NOTIFY lastSearchSourceChanged FINAL)

private:
    QSettings *m_settings;

    QString m_lastSearchSource;

public:
    explicit SourcesSettings(QObject *parent = nullptr);

    QString lastSearchSource() const;
    void setLastSearchSource(const QString &newLastSearchSource);

signals:
    void lastSearchSourceChanged();
};
