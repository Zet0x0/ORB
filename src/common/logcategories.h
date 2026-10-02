#pragma once

#include <QLoggingCategory>

// QML categories are in ORB.QmlComponents/LogCategories
// mpv's own ones (orb.player.mpv.<something>) are created at runtime, in
// player.cpp

Q_DECLARE_LOGGING_CATEGORY(lcApp)
Q_DECLARE_LOGGING_CATEGORY(lcLogging)
Q_DECLARE_LOGGING_CATEGORY(lcSettings)
Q_DECLARE_LOGGING_CATEGORY(lcSources)
Q_DECLARE_LOGGING_CATEGORY(lcFavorites)
Q_DECLARE_LOGGING_CATEGORY(lcPlayer)
Q_DECLARE_LOGGING_CATEGORY(lcUi)
