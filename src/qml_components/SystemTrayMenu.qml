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
        y = Math.max(Screen.virtualY, Math.min(Screen.virtualY + Screen.height - height, lastPosition.y - height + 1)); // `+ 1` for perfect alignment
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

        // HACK: using the hack for play/stop, mute/unmute
        // like in Main.qml for the menu bar

        MenuItem {
            enabled: Player.station.valid
            height: visible ? implicitHeight : 0
            text: qsTr("&Play")
            visible: Player.state === Player.Stopped

            onTriggered: Player.play()
        }

        MenuItem {
            height: visible ? implicitHeight : 0
            text: qsTr("&Stop")
            visible: Player.state !== Player.Stopped

            onTriggered: Player.stop()
        }

        MenuItem {
            height: visible ? implicitHeight : 0
            text: qsTr("&Mute")
            visible: !Player.muted

            onTriggered: Player.muted = true
        }

        MenuItem {
            height: visible ? implicitHeight : 0
            text: qsTr("&Unmute")
            visible: Player.muted

            onTriggered: Player.muted = false
        }

        MenuSeparator {}

        Action {
            text: qsTr("&Quit %0").arg(Qt.application.name)

            onTriggered: Qt.quit()
        }
    }
}
