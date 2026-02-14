#pragma once

#include <QQmlEngine>

struct SearchError {
    Q_GADGET
    QML_NAMED_ELEMENT(searchError)

    Q_PROPERTY(QString title MEMBER title CONSTANT FINAL)
    Q_PROPERTY(QString message MEMBER message CONSTANT FINAL)

public:
    QString title;
    QString message;
};
