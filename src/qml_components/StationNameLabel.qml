import ORB.Player
import ORB.Style
import QtQuick

MarqueeLabel {
    id: root

    ToolTip.text: text
    ToolTip.visible: truncated && !marqueeing && (hoverHandler.hovered || activeFocus)
    activeFocusOnTab: (truncated && !marqueeing) || activeFocus
    font.italic: !Player.station.valid
    paused: hoverHandler.hovered
    text: Player.station.valid ? Player.station.name : qsTr("No station selected")
    textFormat: Text.PlainText

    Rectangle {
        border.color: root.activeFocus ? root.palette.highlight : "#00000000"
        color: "#00000000"
        height: Math.round(root.contentHeight) + 4
        radius: 2
        width: Math.round(root.boxWidth) + 4
        x: -2
        y: -2
        z: 1
    }

    HoverHandler {
        id: hoverHandler
    }
}
