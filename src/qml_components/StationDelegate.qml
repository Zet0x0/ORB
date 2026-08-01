import ORB.Player
import ORB.Sources
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    id: control

    required property station station

    function handleInteraction() {
        if (Player.station === station) {
            if (Player.state === Player.Stopped) {
                Player.play();
            } else {
                Player.stop();
            }
        } else {
            Player.setStation(station, true);
        }
    }

    padding: 0

    background: Rectangle {
        border.color: control.ListView.isCurrentItem && control.ListView.view.activeFocus ? palette.highlight : "#00000000"
        color: Qt.darker(palette.base, (enabled && control.hovered) || (control.ListView.isCurrentItem && control.ListView.view.activeFocus) ? 0.8 : 1.0)
    }

    Keys.onSpacePressed: handleInteraction()

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    TapHandler {
        onTapped: control.handleInteraction()
    }

    Control {
        anchors.fill: parent
        padding: 8 + (control.background as Rectangle).border.width

        contentItem: RowLayout {
            StationImage {
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: 48
                Layout.preferredWidth: 48
                imageUrl: control.station.imageUrl
            }

            Label {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 48
                ToolTip.text: control.station.name
                ToolTip.visible: truncated && (enabled && (control.hovered || control.visualFocus || (control.ListView.isCurrentItem && control.ListView.view.activeFocus)))
                elide: Text.ElideRight
                text: control.station.name
                textFormat: Text.PlainText
                verticalAlignment: Qt.AlignVCenter
                wrapMode: Text.Wrap
            }
        }
    }
}
