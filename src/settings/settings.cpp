#include "settings.h"

WindowSettings *Settings::window() const {
    return m_window;
}

SourcesSettings *Settings::sources() const {
    return m_sources;
}

PlayerSettings *Settings::player() const {
    return m_player;
}

TraySettings *Settings::tray() const {
    return m_tray;
}

AppearanceSettings *Settings::appearance() const {
    return m_appearance;
}

LoggingSettings *Settings::logging() const {
    return m_logging;
}

Settings::Settings(QObject *parent)
    : QObject(parent), m_window(new WindowSettings(this)),
      m_sources(new SourcesSettings(this)), m_player(new PlayerSettings(this)),
      m_tray(new TraySettings(this)),
      m_appearance(new AppearanceSettings(this)),
      m_logging(new LoggingSettings(this)) {}
