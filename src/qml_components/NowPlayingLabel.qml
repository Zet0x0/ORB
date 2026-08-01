import ORB.Common
import ORB.Player
import QtQuick
import QtQuick.Controls

// TODO: sliding label OR elide
Label {
    id: control

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
            border.color: control.activeFocus ? control.palette.highlight : "#00000000"
            color: "#00000000"
            height: Math.round(control.contentHeight) + 4
            radius: 2
            width: Math.round(control.contentWidth) + 4
            x: -2
            y: -2
        }
    }
    states: [
        State {
            name: "playback-stopped"
            when: !Player.station.valid || Player.state === Player.Stopped

            PropertyChanges {
                control.text: qsTr("Not playing anything currently")
            }
        },
        State {
            name: "no-info"
            when: Player.nowPlaying === ""

            PropertyChanges {
                control.text: qsTr("No song information available")
            }
        },
        State {
            name: "showing-info"
            when: Player.nowPlaying !== ""

            PropertyChanges {
                control.text: Player.nowPlaying
            }
        }
    ]

    Shortcut {
        enabled: control.activeFocus
        sequences: ["Menu", "Shift+F10"]

        onActivated: contextMenu.popup(Qt.point(0, control.height))
    }

    TapHandler {
        acceptedButtons: Qt.RightButton

        onTapped: event => {
            const position = event.position;

            if (position.x <= control.contentWidth && position.y <= control.contentHeight) {
                contextMenu.popup(position);
            }
        }
    }

    HoverHandler {
        id: hoverHandler
    }

    Menu {
        id: contextMenu

        MenuItem {
            enabled: control.state === "showing-info"
            text: qsTr("Copy")

            onTriggered: Utilities.copyToClipboard(Player.nowPlaying)

            icon {
                color: palette.buttonText
                name: "copy"
            }
        }
    }
}
