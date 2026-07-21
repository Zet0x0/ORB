import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.player

Frame {
    id: control

    required property station station

    function handleInteraction() {
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

    contentHeight: layout.implicitHeight + layout.anchors.topMargin + layout.anchors.bottomMargin
    contentWidth: layout.implicitWidth + layout.anchors.leftMargin + layout.anchors.rightMargin
    hoverEnabled: enabled
    padding: 0

    background: Rectangle {
        border.color: enabled ? (control.hovered ? palette.active.accent : palette.active.base) : palette.disabled.base
        color: enabled ? (control.hovered ? palette.active.highlight : palette.active.window) : palette.disabled.window
    }

    Keys.onSpacePressed: handleInteraction()

    RowLayout {
        id: layout

        anchors {
            fill: parent
            margins: 8 + (control.background as Rectangle).border.width
        }

        Control {
            Layout.preferredHeight: Math.max(layout.height, 48)
            Layout.preferredWidth: Math.max(layout.height, 48)
            padding: (background as Rectangle).border.width

            background: Rectangle {
                border.color: enabled ? (control.hovered ? palette.active.accent : palette.active.base) : palette.disabled.base
                color: enabled ? palette.active.window : palette.disabled.window
            }
            contentItem: StationImage {
                imageUrl: control.station.imageUrl
            }
        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: layout.height
            color: enabled ? (control.hovered ? palette.active.highlightedText : palette.active.windowText) : palette.disabled.windowText
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

        onClicked: control.handleInteraction()
    }
}
