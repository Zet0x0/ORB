#include "urlvalidator.h"
#include <QUrl>

UrlValidator::UrlValidator(QObject *parent) : QValidator(parent) {}

QValidator::State UrlValidator::validate(QString &input, int &pos) const {
    Q_UNUSED(pos)

    if (input.startsWith(' ') || input.endsWith(' ')) {
        return QValidator::Invalid;
    }

    if (input.isEmpty()) {
        return QValidator::Intermediate;
    }

    QUrl url(input);

    if (url.scheme().isEmpty()) {
        return QValidator::Intermediate;
    }

    return url.isValid() ? QValidator::Acceptable : QValidator::Intermediate;
}

void UrlValidator::fixup(QString &input) const {
    input = input.trimmed();
    QUrl fixed(input, QUrl::TolerantMode);
    input = fixed.toString();
}
