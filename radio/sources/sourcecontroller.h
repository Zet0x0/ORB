#pragma once

#include "../singleton.h"
#include "source.h"
#include <QQmlEngine>

class SourceController : public QObject, public Singleton<SourceController> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    friend class Singleton<SourceController>;

private:
    QMap<QString, Source *> m_sources;

    explicit SourceController(QObject *parent = nullptr);

public:
    bool sourceExists(const QString &sourceName);
    bool registerSource(const QString &name, Source *source);
    Q_INVOKABLE QStringList getSources() const;
};
