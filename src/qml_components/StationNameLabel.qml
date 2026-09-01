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
    font.italic: !Player.station.valid
    text: Player.station.valid ? Player.station.name : qsTr("No station selected")
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

    HoverHandler {
        id: hoverHandler
    }
}
