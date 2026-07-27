import ORB.Common
import ORB.Player
import QtQuick
import QtQuick.Controls

// TODO: sliding label OR elide
Label {
    id: control

    ToolTip.text: text
    ToolTip.visible: truncated && hoverHandler.hovered
    color: state === "showing-info" ? palette.windowText : palette.disabled.windowText
    elide: Text.ElideRight
    font.italic: state !== "showing-info"
    maximumLineCount: 1
    textFormat: Text.PlainText

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

    ContextMenu.onRequested: position => {
        if (position.x <= control.contentWidth && position.y <= control.contentHeight) {
            contextMenu.popup(position);
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
