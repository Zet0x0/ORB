#pragma once

#include <QRestReply>

namespace Utilities {
QString parseUserInputUrl(const QString &userInput);
QString escapeControlCharacters(QString string);

qint64 currentTimestampUtc();

QString pathFromRestReply(QRestReply &reply);
}
