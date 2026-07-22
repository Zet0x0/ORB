#include "utilities.h"
#include <QDateTime>
#include <QUrl>

QString Utilities::normalizeUserInputUrl(const QString &userInput) {
    return QUrl::fromUserInput(userInput).toString();
}

QString Utilities::escapeControlCharacters(QString string) {
    return string.replace('\a', "\\a")
        .replace('\b', "\\b")
        .replace('\t', "\\t")
        .replace('\n', "\\n")
        .replace('\v', "\\v")
        .replace('\f', "\\f")
        .replace('\r', "\\r")
        .replace('\x1b', "\\e");
}

qint64 Utilities::currentTimestampUtc() {
    return QDateTime::currentSecsSinceEpoch();
}

QString Utilities::pathFromRestReply(QRestReply &reply) {
    return reply.networkReply()->request().url().path();
}
