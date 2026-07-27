import ORB.Common
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    id: control

    property errorInfo displayedError
    required property errorInfo error

    signal dismissed

    function updateAutoDismiss() {
        if (error.title.length === 0 && error.message.length === 0) {
            autoDismissTimer.stop();
        } else if (hovered) {
            autoDismissTimer.stop();
        } else {
            autoDismissTimer.restart();
        }
    }

    height: Math.min(control.implicitHeight, parent.height)
    hoverEnabled: true
    opacity: error.title.length > 0 || error.message.length > 0 ? 1 : 0
    visible: opacity > 0
    width: Math.min(400, control.implicitWidth, parent.width)

    background: Rectangle {
        id: bannerBackground

        border.color: "#f00"
        color: control.palette.window
        radius: 2

        SequentialAnimation {
            id: flashAnimation

            loops: 3

            ColorAnimation {
                duration: 200
                property: "color"
                target: bannerBackground
                to: Qt.tint(control.palette.window, "#33ff0000")
            }

            ColorAnimation {
                duration: 200
                property: "color"
                target: bannerBackground
                to: control.palette.window
            }
        }
    }
    Behavior on opacity {
        NumberAnimation {
            duration: 250
        }
    }

    Component.onCompleted: {
        if (control.error.title.length > 0 || control.error.message.length > 0) {
            control.displayedError = control.error;
        }
    }
    onErrorChanged: {
        if (control.error.title.length > 0 || control.error.message.length > 0) {
            control.displayedError = control.error;

            flashAnimation.restart();
        }

        control.updateAutoDismiss();
    }
    onHoveredChanged: updateAutoDismiss()

    Timer {
        id: autoDismissTimer

        interval: 5000

        onTriggered: control.dismissed()
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true

            Label {
                Layout.fillWidth: true
                elide: Text.ElideRight
                font.bold: true
                maximumLineCount: 1
                text: control.displayedError.title
                textFormat: Text.PlainText
            }

            Label {
                Layout.fillHeight: true
                Layout.fillWidth: true
                elide: Text.ElideRight
                text: control.displayedError.message
                textFormat: Text.PlainText
                wrapMode: Label.Wrap
            }
        }

        IconButton {
            Layout.alignment: Qt.AlignTop
            icon.name: "copy"

            onClicked: Utilities.copyToClipboard(`${control.displayedError.title}\n\n${control.displayedError.message}`)
        }

        IconButton {
            Layout.alignment: Qt.AlignTop
            icon.name: "x"

            onClicked: control.dismissed()
        }
    }
}
