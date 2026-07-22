#pragma once

#include "../common/singleton.h"
#include "sourcessettings.h"
#include "windowsettings.h"

class Settings : public QObject, public Singleton<Settings> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(WindowSettings *window READ window CONSTANT FINAL)
    Q_PROPERTY(SourcesSettings *sources READ sources CONSTANT FINAL)

    friend class Singleton<Settings>;

private:
    WindowSettings *m_window;
    SourcesSettings *m_sources;

    explicit Settings(QObject *parent = nullptr);

public:
    WindowSettings *window() const;
    SourcesSettings *sources() const;
};
