import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import radio.player

Frame {
    id: control

    required property station station

    contentHeight: layout.implicitHeight + layout.anchors.topMargin + layout.anchors.bottomMargin
    contentWidth: layout.implicitWidth + layout.anchors.leftMargin + layout.anchors.rightMargin
    padding: 0

    background: Rectangle {
        border.color: mouseArea.containsMouse ? palette.accent : palette.base
        color: mouseArea.containsMouse ? palette.highlight : palette.window
    }

    RowLayout {
        id: layout

        anchors {
            fill: parent
            margins: 8
        }

        Control {
            Layout.preferredHeight: Math.max(layout.height, 48)
            Layout.preferredWidth: Math.max(layout.height, 48)
            padding: (background as Rectangle).border.width

            background: Rectangle {
                border.color: mouseArea.containsMouse ? palette.accent : palette.base
                color: palette.window
            }
            contentItem: StationImage {
                imageUrl: control.station.imageUrl
            }
        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: layout.height
            color: mouseArea.containsMouse ? palette.highlightedText : palette.windowText
            elide: Text.ElideRight
            text: control.station.name
            textFormat: Text.PlainText
            verticalAlignment: Qt.AlignVCenter
            wrapMode: Text.Wrap
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true

        onClicked: {
            if (Player.station === control.station) {
                if (Player.state === Player.Stopped) {
                    Player.play();
                } else {
                    Player.stop();
                }
            } else {
                Player.setStation(control.station, true);
            }
        }
    }
}
