import ORB.Common
import ORB.Logging
import ORB.Player
import ORB.Settings
import Qt.labs.platform
import QtQuick

SystemTrayIcon {
    id: root

    required property MainWindow applicationWindow
    required property SystemTrayMenu trayMenu
    // HACK: QTBUG-33481 workaround
    // held in a property because this thing has no default property for children
    readonly property Connections windowConnections: Connections {
        function onActiveChanged(): void {
            if (root.applicationWindow.active && root.trayMenu !== null) {
                root.trayMenu.dismiss();
            }
        }

        target: root.applicationWindow
    }

    icon.source: "qrc:/icons/ORB.svg"
    tooltip: {
        if (!Player.station.valid) {
            return "ORB";
        }

        if (Player.nowPlaying !== "") {
            return Player.nowPlaying;
        }

        return Player.station.name;
    }
    visible: available && trayMenu !== null && Settings.tray.enabled

    Component.onCompleted: {
        if (!available) {
            console.warn(LogCategories.tray, "System tray is not available");
        }
    }
    onActivated: reason => {
        switch (reason) {
        case SystemTrayIcon.Context:
            {
                trayMenu.popup(Utilities.getGlobalCursorPos());

                break;
            }
        case SystemTrayIcon.Trigger:
            {
                applicationWindow.activate();

                break;
            }
        default:
            {
                return;
            }
        }
    }
}
