#pragma once

#include "../common/singleton.h"
#include "windowsettings.h"

class Settings : public QObject, public Singleton<Settings> {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(WindowSettings *window READ window CONSTANT FINAL)

    friend class Singleton<Settings>;

private:
    WindowSettings *m_window;

    explicit Settings(QObject *parent = nullptr);

public:
    WindowSettings *window() const;
};
