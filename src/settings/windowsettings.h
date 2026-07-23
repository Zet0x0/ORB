#pragma once

#include <QQmlEngine>
#include <QSettings>

class WindowSettings : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged FINAL)

    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(
        int height READ height WRITE setHeight NOTIFY heightChanged FINAL)

private:
    QSettings *m_settings;

    int m_x;
    int m_y;

    int m_width;
    int m_height;

public:
    explicit WindowSettings(QObject *parent = nullptr);

    int x() const;
    void setX(int newX);
    int y() const;
    void setY(int newY);

    int width() const;
    void setWidth(int newWidth);
    int height() const;
    void setHeight(int newHeight);

signals:
    void xChanged();
    void yChanged();

    void widthChanged();
    void heightChanged();
};
