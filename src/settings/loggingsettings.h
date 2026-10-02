#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>

class LoggingSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int maxEntries READ maxEntries WRITE setMaxEntries NOTIFY
                   maxEntriesChanged FINAL)
    Q_PROPERTY(int maxFiles READ maxFiles WRITE setMaxFiles NOTIFY
                   maxFilesChanged FINAL)

    Q_PROPERTY(QString filterRules READ filterRules WRITE setFilterRules NOTIFY
                   filterRulesChanged FINAL)

private:
    int m_maxEntries;
    int m_maxFiles;

    QString m_filterRules;

public:
    explicit LoggingSettings(QObject *parent = nullptr);

    QList<SettingsFieldMeta> settingsFields() const override;

    int maxEntries() const;
    void setMaxEntries(int newMaxEntries);

    int maxFiles() const;
    void setMaxFiles(int newMaxFiles);

    QString filterRules() const;
    void setFilterRules(const QString &newFilterRules);

signals:
    void maxEntriesChanged();
    void maxFilesChanged();

    void filterRulesChanged();
};
