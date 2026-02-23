#pragma once

#include <QQmlEngine>

struct SourceError {
    Q_GADGET
    QML_NAMED_ELEMENT(sourceError)

    Q_PROPERTY(QString title MEMBER title CONSTANT FINAL)
    Q_PROPERTY(QString message MEMBER message CONSTANT FINAL)

public:
    QString title;
    QString message;

    bool operator==(const SourceError &other) const;
};
