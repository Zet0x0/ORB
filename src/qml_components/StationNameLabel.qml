import ORB.Player
import ORB.Style
import QtQuick

// TODO: sliding label OR elide
Label {
    id: root

    ToolTip.text: text
    ToolTip.visible: truncated && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: truncated || activeFocus
    elide: Text.ElideRight
    font.italic: state !== "showing-name"
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
            name: "no-station"
            when: !Player.station.valid

            PropertyChanges {
                root.text: qsTr("No station selected")
            }
        },
        State {
            name: "showing-name"
            when: Player.station.valid

            PropertyChanges {
                root.text: Player.station.name
            }
        }
    ]

    HoverHandler {
        id: hoverHandler
    }
}
