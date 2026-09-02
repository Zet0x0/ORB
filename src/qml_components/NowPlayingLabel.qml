import ORB.Common
import ORB.Player
import ORB.Style
import QtQuick

// TODO: sliding label OR elide
Label {
    id: root

    readonly property bool showingInfo: Player.station.valid && Player.state !== Player.Stopped && Player.nowPlaying !== ""

    ToolTip.text: text
    ToolTip.visible: truncated && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: true
    color: showingInfo ? palette.windowText : palette.disabled.windowText
    elide: Text.ElideRight
    font.italic: !showingInfo
    maximumLineCount: 1
    text: {
        if (!Player.station.valid || Player.state === Player.Stopped) {
            return qsTr("Not playing anything currently");
        }

        if (Player.nowPlaying === "") {
            return qsTr("No song information available");
        }

        return Player.nowPlaying;
    }
    textFormat: Text.PlainText

    background: Rectangle {
        color: "#00000000"

        Rectangle {
            border.color: root.activeFocus || contextMenu.visible ? root.palette.highlight : "#00000000"
            color: "#00000000"
            height: Math.round(root.contentHeight) + 4
            radius: 2
            width: Math.round(root.contentWidth) + 4
            x: -2
            y: -2
        }
    }

    Shortcut {
        enabled: root.activeFocus
        sequences: ["Menu", "Shift+F10"]

        onActivated: contextMenu.popup(0, root.height)
    }

    TapHandler {
        acceptedButtons: Qt.RightButton

        onTapped: event => {
            const position = event.position;

            if (position.x <= root.contentWidth && position.y <= root.contentHeight) {
                contextMenu.popup(position);
            }
        }
    }

    HoverHandler {
        id: hoverHandler
    }

    Menu {
        id: contextMenu

        Action {
            enabled: root.showingInfo
            icon.name: "copy"
            text: qsTr("Copy")

            onTriggered: Utilities.copyToClipboard(Player.nowPlaying)
        }
    }
}
