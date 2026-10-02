#pragma once

#include "singleton.h"
#include <QObject>
#include <QPoint>
#include <QQmlEngine>
#include <QRect>
#include <QString>
#include <QUrl>

class Utilities : public QObject, public Singleton<Utilities> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    friend class Singleton<Utilities>;

public:
    static QString normalizeUserInputUrl(const QString &userInput);
    static QString escapeControlCharacters(QString string);

    static qint64 currentTimestamp();

    Q_INVOKABLE static void copyToClipboard(const QString &text);
    Q_INVOKABLE static QString pasteFromClipboard();

    Q_INVOKABLE static bool openUrlExternally(const QUrl &url);

    Q_INVOKABLE static QPoint getGlobalCursorPos();
    Q_INVOKABLE static QRect getScreenAvailableGeometry(const QPoint &point);

    Q_INVOKABLE static bool isPointOnScreen(const QPoint &point);

    Q_INVOKABLE static QString qtVersion();
};
