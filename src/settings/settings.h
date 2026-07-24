#pragma once

#include "../common/singleton.h"
#include "playersettings.h"
#include "sourcessettings.h"
#include "traysettings.h"
#include "windowsettings.h"

class Settings : public QObject, public Singleton<Settings> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(WindowSettings *window READ window CONSTANT FINAL)
    Q_PROPERTY(SourcesSettings *sources READ sources CONSTANT FINAL)
    Q_PROPERTY(PlayerSettings *player READ player CONSTANT FINAL)
    Q_PROPERTY(TraySettings *tray READ tray CONSTANT FINAL)

    friend class Singleton<Settings>;

private:
    WindowSettings *m_window;
    SourcesSettings *m_sources;
    PlayerSettings *m_player;
    TraySettings *m_tray;

    explicit Settings(QObject *parent = nullptr);

public:
    WindowSettings *window() const;
    SourcesSettings *sources() const;
    PlayerSettings *player() const;
    TraySettings *tray() const;
};
