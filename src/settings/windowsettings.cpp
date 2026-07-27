#include "windowsettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

WindowSettings::WindowSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("window"));

    m_x = SettingsIO::readInt(m_settings, QStringLiteral("x"), -1);
    m_y = SettingsIO::readInt(m_settings, QStringLiteral("y"), -1);

    m_width = SettingsIO::readInt(m_settings, QStringLiteral("width"), 640);
    m_height = SettingsIO::readInt(m_settings, QStringLiteral("height"), 480);
}

int WindowSettings::x() const {
    return m_x;
}

void WindowSettings::setX(int newX) {
    if (m_x == newX) {
        return;
    }

    m_x = newX;
    SettingsIO::write(m_settings, QStringLiteral("x"), m_x);

    emit xChanged();
}

int WindowSettings::y() const {
    return m_y;
}

void WindowSettings::setY(int newY) {
    if (m_y == newY) {
        return;
    }

    m_y = newY;
    SettingsIO::write(m_settings, QStringLiteral("y"), m_y);

    emit yChanged();
}

int WindowSettings::width() const {
    return m_width;
}

void WindowSettings::setWidth(int newWidth) {
    if (m_width == newWidth) {
        return;
    }

    m_width = newWidth;
    SettingsIO::write(m_settings, QStringLiteral("width"), m_width);

    emit widthChanged();
}

int WindowSettings::height() const {
    return m_height;
}

void WindowSettings::setHeight(int newHeight) {
    if (m_height == newHeight) {
        return;
    }

    m_height = newHeight;
    SettingsIO::write(m_settings, QStringLiteral("height"), m_height);

    emit heightChanged();
}
