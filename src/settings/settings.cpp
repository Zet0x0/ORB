#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent), m_window(new WindowSettings(this)) {}

WindowSettings *Settings::window() const {
    return m_window;
}
