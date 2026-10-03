pragma Singleton

import QtQml

// C++ categories are in logcategories.cpp

QtObject {
    readonly property LoggingCategory tray: LoggingCategory {
        name: "orb.ui.tray"
    }
    readonly property LoggingCategory window: LoggingCategory {
        name: "orb.ui.window"
    }
}
