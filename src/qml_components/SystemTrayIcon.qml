import ORB.Common
import ORB.Player
import ORB.Settings
import Qt.labs.platform
import QtQuick

SystemTrayIcon {
    required property MainWindow applicationWindow
    required property SystemTrayMenu trayMenu

    icon.source: "qrc:/icons/ORB.svg"
    tooltip: {
        if (!Player.station.valid) {
            return Qt.application.name;
        }

        if (Player.nowPlaying !== "") {
            return Player.nowPlaying;
        }

        return Player.station.name;
    }
    visible: available && trayMenu !== null && Settings.tray.enabled

    // HACK: QTBUG-33481 workaround
    Component.onCompleted: {
        applicationWindow.activeChanged.connect(() => {
            if (applicationWindow.active && trayMenu !== null) {
                trayMenu.dismiss();
            }
        });
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
