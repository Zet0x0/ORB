import ORB.Player
import ORB.Sources
import ORB.Style
import QtQuick
import QtQuick.Layouts

Frame {
    id: root

    property bool keyPressed: false
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
        id: background

        border.color: root.ListView.isCurrentItem && root.ListView.view.activeFocus ? palette.highlight : "#00000000"
        color: palette.base.darker(tapHandler.pressed || root.keyPressed ? 1.2 : (enabled && (root.hovered || root.visualFocus || (root.ListView.isCurrentItem && root.ListView.view.activeFocus)) ? 0.8 : 1.0))
    }

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
            keyPressed = true;

            handleInteraction();

            event.accepted = true;
        }
    }
    Keys.onReleased: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
            keyPressed = false;

            event.accepted = true;
        }
    }
    onActiveFocusChanged: {
        if (!activeFocus) {
            keyPressed = false;
        }
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    TapHandler {
        id: tapHandler

        onTapped: root.handleInteraction()
    }

    Control {
        anchors.fill: parent
        padding: 8 + background.border.width

        contentItem: RowLayout {
            StationImage {
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: 48
                Layout.preferredWidth: 48
                imageUrl: root.station.imageUrl
            }

            Label {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumHeight: 48
                ToolTip.text: root.station.name
                ToolTip.visible: truncated && (enabled && (root.hovered || root.visualFocus || (root.ListView.isCurrentItem && root.ListView.view.activeFocus)))
                elide: Text.ElideRight
                text: root.station.name
                textFormat: Text.PlainText
                verticalAlignment: Qt.AlignVCenter
                wrapMode: Text.Wrap
            }
        }
    }
}
