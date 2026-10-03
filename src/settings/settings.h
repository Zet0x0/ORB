#pragma once

#include "../common/singleton.h"
#include "appearancesettings.h"
#include "loggingsettings.h"
#include "playersettings.h"
#include "sourcessettings.h"
#include "traysettings.h"
#include "windowsettings.h"
#include <QObject>
#include <QQmlEngine>

class Settings : public QObject, public Singleton<Settings> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(WindowSettings *window READ window CONSTANT FINAL)
    Q_PROPERTY(SourcesSettings *sources READ sources CONSTANT FINAL)
    Q_PROPERTY(PlayerSettings *player READ player CONSTANT FINAL)
    Q_PROPERTY(TraySettings *tray READ tray CONSTANT FINAL)
    Q_PROPERTY(AppearanceSettings *appearance READ appearance CONSTANT FINAL)
    Q_PROPERTY(LoggingSettings *logging READ logging CONSTANT FINAL)

    friend class Singleton<Settings>;

public:
    WindowSettings *window() const;
    SourcesSettings *sources() const;
    PlayerSettings *player() const;
    TraySettings *tray() const;
    AppearanceSettings *appearance() const;
    LoggingSettings *logging() const;

private:
    WindowSettings *m_window;
    SourcesSettings *m_sources;
    PlayerSettings *m_player;
    TraySettings *m_tray;
    AppearanceSettings *m_appearance;
    LoggingSettings *m_logging;

    explicit Settings(QObject *parent = nullptr);
};
