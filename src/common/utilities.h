#pragma once

#include "singleton.h"
#include <QPoint>
#include <QRect>
#include <QRestReply>

class Utilities : public QObject, public Singleton<Utilities> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    friend class Singleton<Utilities>;

public:
    static QString normalizeUserInputUrl(const QString &userInput);
    static QString escapeControlCharacters(QString string);

    static qint64 currentTimestamp();

    static QString requestPathFromRestReply(QRestReply &reply);

    Q_INVOKABLE static void copyToClipboard(const QString &text);
    Q_INVOKABLE static QString pasteFromClipboard();

    Q_INVOKABLE static QPoint getGlobalCursorPos();
    Q_INVOKABLE static QRect getScreenAvailableGeometry(const QPoint &point);
};
