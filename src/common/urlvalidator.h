#pragma once

#include <QQmlEngine>
#include <QValidator>

class UrlValidator : public QValidator {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit UrlValidator(QObject *parent = nullptr);

    QValidator::State validate(QString &input, int &pos) const override;
    void fixup(QString &input) const override;
};
