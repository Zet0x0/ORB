#pragma once

#include <QRestReply>

namespace Utilities {
QString normalizeUserInputUrl(const QString &userInput);
QString escapeControlCharacters(QString string);

qint64 currentTimestampUtc();

QString requestPathFromRestReply(QRestReply &reply);
}
