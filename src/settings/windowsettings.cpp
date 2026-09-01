#include "windowsettings.h"
#include "settingsfactory.h"
#include "settingsio.h"

WindowSettings::WindowSettings(QObject *parent)
    : SettingsGroup(parent), m_settings(SettingsFactory::create(this)) {
    m_settings->beginGroup(QStringLiteral("window"));

    m_x = SettingsIO::readInt(m_settings, QStringLiteral("x"), 0);
    m_y = SettingsIO::readInt(m_settings, QStringLiteral("y"), 0);
    refreshHasPosition();

    m_width = SettingsIO::readInt(m_settings, QStringLiteral("width"), 640);
    m_height = SettingsIO::readInt(m_settings, QStringLiteral("height"), 480);
}

void WindowSettings::refreshHasPosition() {
    const bool newHasPosition = m_settings->contains(QStringLiteral("x")) &&
                                m_settings->contains(QStringLiteral("y"));

    if (m_hasPosition == newHasPosition) {
        return;
    }

    m_hasPosition = newHasPosition;

    emit hasPositionChanged();
}

int WindowSettings::x() const {
    return m_x;
}

void WindowSettings::setX(int newX) {
    // `firstWrite` should solve a problem of not writing if
    // the value was never set before and is suddenly 0
    const bool firstWrite = !m_settings->contains(QStringLiteral("x"));

    if (m_x == newX && !firstWrite) {
        return;
    }

    const bool changed = m_x != newX;
    m_x = newX;
    SettingsIO::write(m_settings, QStringLiteral("x"), m_x);

    refreshHasPosition();

    if (changed) {
        emit xChanged();
    }
}

int WindowSettings::y() const {
    return m_y;
}

void WindowSettings::setY(int newY) {
    const bool firstWrite = !m_settings->contains(QStringLiteral("y"));

    if (m_y == newY && !firstWrite) {
        return;
    }

    const bool changed = m_y != newY;
    m_y = newY;
    SettingsIO::write(m_settings, QStringLiteral("y"), m_y);

    refreshHasPosition();

    if (changed) {
        emit yChanged();
    }
}

bool WindowSettings::hasPosition() const {
    return m_hasPosition;
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
