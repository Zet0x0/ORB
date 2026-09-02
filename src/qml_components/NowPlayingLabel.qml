import ORB.Common
import ORB.Player
import ORB.Style
import QtQuick

MarqueeLabel {
    id: root

    readonly property bool showingInfo: Player.station.valid && Player.state !== Player.Stopped && Player.nowPlaying !== ""

    ToolTip.text: text
    ToolTip.visible: truncated && !marqueeing && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: true
    font.italic: !showingInfo
    paused: hoverHandler.hovered || contextMenu.visible
    text: {
        if (!Player.station.valid || Player.state === Player.Stopped) {
            return qsTr("Not playing anything currently");
        }

        if (Player.nowPlaying === "") {
            return qsTr("No song information available");
        }

        return Player.nowPlaying;
    }
    textColor: showingInfo ? palette.windowText : palette.disabled.windowText
    textFormat: Text.PlainText

    Rectangle {
        border.color: root.activeFocus || contextMenu.visible ? root.palette.highlight : "#00000000"
        color: "#00000000"
        height: Math.round(root.contentHeight) + 4
        radius: 2
        width: Math.round(root.boxWidth) + 4
        x: -2
        y: -2
        z: 1
    }

    Shortcut {
        enabled: root.activeFocus
        sequences: ["Menu", "Shift+F10"]

        onActivated: contextMenu.popup(Qt.point(0, root.height))
    }

    TapHandler {
        acceptedButtons: Qt.RightButton

        onTapped: event => {
            const position = event.position;

            if (position.x <= root.boxWidth && position.y <= root.contentHeight) {
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
