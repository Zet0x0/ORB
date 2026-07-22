#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent), m_window(new WindowSettings(this)),
      m_sources(new SourcesSettings(this)) {}

WindowSettings *Settings::window() const {
    return m_window;
}

SourcesSettings *Settings::sources() const {
    return m_sources;
}
