#include "utilities.h"
#include <QDateTime>
#include <QUrl>

QString Utilities::parseUserInputUrl(const QString &userInput) {
    return QUrl::fromUserInput(userInput).toString();
}

qint64 Utilities::currentTimestampUtc() {
    return QDateTime::currentSecsSinceEpoch();
}
