import ORB.Common
import ORB.Style
import QtQuick
import QtQuick.Layouts

Frame {
    id: root

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

    height: Math.min(implicitHeight, parent.height)
    opacity: error.title.length > 0 || error.message.length > 0 ? 1 : 0
    visible: opacity > 0
    width: Math.min(400, implicitWidth, parent.width)

    background: Rectangle {
        id: bannerBackground

        border.color: AppColors.semantic.danger
        color: palette.window
        radius: 2

        SequentialAnimation {
            id: flashAnimation

            loops: 3

            ColorAnimation {
                duration: 200
                property: "color"
                target: bannerBackground
                to: Qt.tint(root.palette.window, Qt.alpha(AppColors.semantic.danger, 0.2))
            }

            ColorAnimation {
                duration: 200
                property: "color"
                target: bannerBackground
                to: root.palette.window
            }
        }
    }
    Behavior on opacity {
        NumberAnimation {
            duration: 250
        }
    }

    Component.onCompleted: {
        if (error.title.length > 0 || error.message.length > 0) {
            displayedError = error;
        }
    }
    onErrorChanged: {
        if (error.title.length > 0 || error.message.length > 0) {
            displayedError = error;

            flashAnimation.restart();
        }

        updateAutoDismiss();
    }
    onHoveredChanged: updateAutoDismiss()

    Timer {
        id: autoDismissTimer

        interval: 5000

        onTriggered: root.dismissed()
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
                text: root.displayedError.title
                textFormat: Text.PlainText
            }

            Label {
                Layout.fillHeight: true
                Layout.fillWidth: true
                elide: Text.ElideRight
                text: root.displayedError.message
                textFormat: Text.PlainText
                wrapMode: Label.Wrap
            }
        }

        IconButton {
            Layout.alignment: Qt.AlignTop
            ToolTip.text: qsTr("Copy to clipboard")
            icon.name: "copy"

            onClicked: Utilities.copyToClipboard(`${root.displayedError.title}\n\n${root.displayedError.message}`)
        }

        IconButton {
            Layout.alignment: Qt.AlignTop
            ToolTip.text: qsTr("Dismiss")
            icon.name: "x"

            onClicked: root.dismissed()
        }
    }
}
