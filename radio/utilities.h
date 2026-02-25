#pragma once

#include <QRestReply>

namespace Utilities {
QString parseUserInputUrl(const QString &userInput);

qint64 currentTimestampUtc();

QString pathFromRestReply(QRestReply &reply);
}
