import QtQuick
import QtQuick.Controls
import orb.player

Window {
    id: window

    property var lastPosition: null

    function dismiss() {
        if (menu.opened) {
            menu.close();
        }
    }

    function popup(position) {
        lastPosition = position;
        reposition();

        show();
        menu.popup();
    }

    function reposition() {
        if (lastPosition === null) {
            return;
        }

        x = Math.max(Screen.virtualX, Math.min(Screen.virtualX + Screen.width - width, lastPosition.x));
        // fencepost problem: window's last pixel row is `y + height - 1`,
        // so `y` must be `lastPosition.y - (height - 1)` for that row to land on the cursor
        y = Math.max(Screen.virtualY, Math.min(Screen.virtualY + Screen.height - height, lastPosition.y - (height - 1)));
    }

    color: "#00000000"
    flags: Qt.Popup | Qt.NoDropShadowWindowHint
    height: menu.implicitHeight
    width: menu.implicitWidth

    onHeightChanged: reposition()
    onWidthChanged: reposition()

    Menu {
        id: menu

        onClosed: window.hide()

        Action {
            enabled: Player.station.valid
            text: Player.state === Player.Stopped ? qsTr("Play") : qsTr("Stop")

            onTriggered: {
                if (Player.state === Player.Stopped) {
                    Player.play();
                } else {
                    Player.stop();
                }
            }
        }

        Action {
            text: Player.muted ? qsTr("Unmute") : qsTr("Mute")

            onTriggered: Player.muted = !Player.muted
        }

        MenuSeparator {}

        Action {
            text: qsTr("Quit %0").arg(Qt.application.name)

            onTriggered: Qt.quit()
        }
    }
}
