#pragma once

#include <QQmlEngine>

struct ErrorInfo {
    Q_GADGET
    QML_NAMED_ELEMENT(errorInfo)

    Q_PROPERTY(QString title MEMBER title CONSTANT FINAL)
    Q_PROPERTY(QString message MEMBER message CONSTANT FINAL)

public:
    QString title;
    QString message;

    ErrorInfo() = default;
    ErrorInfo(QString newTitle, QString newMessage);

    bool operator==(const ErrorInfo &other) const;
};
