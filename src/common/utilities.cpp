#include "utilities.h"
#include <QClipboard>
#include <QCursor>
#include <QDateTime>
#include <QGuiApplication>
#include <QScreen>
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

qint64 Utilities::currentTimestamp() {
    return QDateTime::currentSecsSinceEpoch();
}

void Utilities::copyToClipboard(const QString &text) {
    QGuiApplication::clipboard()->setText(text);
}

QString Utilities::pasteFromClipboard() {
    return QGuiApplication::clipboard()->text();
}

QPoint Utilities::getGlobalCursorPos() {
    return QCursor::pos();
}

QRect Utilities::getScreenAvailableGeometry(const QPoint &point) {
    QScreen *screen = QGuiApplication::screenAt(point);

    if (screen == nullptr) {
        screen = QGuiApplication::primaryScreen();
    }

    if (screen == nullptr) {
        return QRect();
    }

    return screen->availableGeometry();
}
