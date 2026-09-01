#pragma once

#include "settingsgroup.h"
#include <QQmlEngine>
#include <QSettings>

class WindowSettings : public SettingsGroup {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged FINAL)
    // Whether an explicit position is stored; not a setting
    Q_PROPERTY(
        bool hasPosition READ hasPosition NOTIFY hasPositionChanged FINAL)

    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(
        int height READ height WRITE setHeight NOTIFY heightChanged FINAL)

private:
    QSettings *m_settings;

    int m_x;
    int m_y;
    bool m_hasPosition = false;

    int m_width;
    int m_height;

    void refreshHasPosition();

public:
    explicit WindowSettings(QObject *parent = nullptr);

    int x() const;
    void setX(int newX);
    int y() const;
    void setY(int newY);

    bool hasPosition() const;

    int width() const;
    void setWidth(int newWidth);
    int height() const;
    void setHeight(int newHeight);

signals:
    void xChanged();
    void yChanged();
    void hasPositionChanged();

    void widthChanged();
    void heightChanged();
};
