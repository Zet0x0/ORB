#include "utilities.h"
#include <QUrl>

QString Utilities::parseUserInputUrl(const QString &userInput) {
    return QUrl::fromUserInput(userInput).toString();
}
