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
    return string.replace(u'\a', "\\a")
        .replace(u'\b', "\\b")
        .replace(u'\t', "\\t")
        .replace(u'\n', "\\n")
        .replace(u'\v', "\\v")
        .replace(u'\f', "\\f")
        .replace(u'\r', "\\r")
        .replace(u'\x1b', "\\e");
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

bool Utilities::isPointOnScreen(const QPoint &point) {
    return QGuiApplication::screenAt(point) != nullptr;
}
