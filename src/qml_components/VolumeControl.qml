import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.player

RowLayout {
    id: root

    SystemPalette {
        id: activeSystemPalette

        colorGroup: SystemPalette.Active
    }

    IconButton {
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
        from: 0
        palette.highlight: Player.muted ? Qt.darker(activeSystemPalette.highlight, 1.5) : activeSystemPalette.highlight
        stepSize: 1.0
        to: 100
        value: Player.volume

        onMoved: Player.volume = value
    }
}
