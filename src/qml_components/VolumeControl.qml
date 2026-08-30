import ORB.Player
import ORB.Style
import QtQuick
import QtQuick.Layouts

RowLayout {
    id: root

    IconButton {
        ToolTip.text: Player.muted ? qsTr("Unmute") : qsTr("Mute")
        icon.name: {
            if (Player.muted) {
                return "volume-off";
            }

            if (Player.volume === 0) {
                return "volume-4";
            }

            if (Player.volume < 50) {
                return "volume-2";
            }

            return "volume";
        }

        onClicked: Player.muted = !Player.muted
    }

    Slider {
        ToolTip.text: qsTr("Volume: %0%").arg(Math.round(value))
        ToolTip.visible: pressed || hovered || visualFocus
        from: 0
        palette.accent: Player.muted ? root.palette.disabled.accent : root.palette.accent
        stepSize: 1.0
        to: 100
        value: Player.volume

        onMoved: Player.volume = value
    }
}
