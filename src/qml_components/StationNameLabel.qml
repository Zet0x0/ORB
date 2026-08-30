import ORB.Player
import ORB.Style
import QtQuick

// TODO: sliding label OR elide
Label {
    id: control

    ToolTip.text: text
    ToolTip.visible: truncated && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: truncated || activeFocus
    elide: Text.ElideRight
    font.italic: state !== "showing-name"
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
            name: "no-station"
            when: !Player.station.valid

            PropertyChanges {
                control.text: qsTr("No station selected")
            }
        },
        State {
            name: "showing-name"
            when: Player.station.valid

            PropertyChanges {
                control.text: Player.station.name
            }
        }
    ]

    HoverHandler {
        id: hoverHandler
    }
}
