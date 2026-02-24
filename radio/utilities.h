#pragma once

#include <QObject>

namespace Utilities {
QString parseUserInputUrl(const QString &userInput);

qint64 currentTimestampUtc();
}
