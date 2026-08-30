import ORB.Common
import ORB.Player
import ORB.Style
import QtQuick

// TODO: sliding label OR elide
Label {
    id: root

    ToolTip.text: text
    ToolTip.visible: truncated && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: true
    color: state === "showing-info" ? palette.windowText : palette.disabled.windowText
    elide: Text.ElideRight
    font.italic: state !== "showing-info"
    maximumLineCount: 1
    textFormat: Text.PlainText

    background: Rectangle {
        color: "#00000000"

        Rectangle {
            border.color: root.activeFocus ? root.palette.highlight : "#00000000"
            color: "#00000000"
            height: Math.round(root.contentHeight) + 4
            radius: 2
            width: Math.round(root.contentWidth) + 4
            x: -2
            y: -2
        }
    }
    states: [
        State {
            name: "playback-stopped"
            when: !Player.station.valid || Player.state === Player.Stopped

            PropertyChanges {
                root.text: qsTr("Not playing anything currently")
            }
        },
        State {
            name: "no-info"
            when: Player.nowPlaying === ""

            PropertyChanges {
                root.text: qsTr("No song information available")
            }
        },
        State {
            name: "showing-info"
            when: Player.nowPlaying !== ""

            PropertyChanges {
                root.text: Player.nowPlaying
            }
        }
    ]

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
            enabled: root.state === "showing-info"
            icon.name: "copy"
            text: qsTr("Copy")

            onTriggered: Utilities.copyToClipboard(Player.nowPlaying)
        }
    }
}
