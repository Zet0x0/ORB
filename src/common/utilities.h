#pragma once

#include <QRestReply>

namespace Utilities {
QString normalizeUserInputUrl(const QString &userInput);
QString escapeControlCharacters(QString string);

qint64 currentTimestamp();

QString requestPathFromRestReply(QRestReply &reply);
}
