import ORB.Style
import QtQuick

TextField {
    id: root

    signal cleared

    rightPadding: leftPadding + clearButton.width

    IconButton {
        id: clearButton

        ToolTip.text: qsTr("Clear")
        flat: true
        icon.name: "circle-x"
        implicitHeight: parent.height
        visible: root.text.length > 0

        onClicked: {
            root.clear();
            root.cleared();
        }

        anchors {
            right: parent.right
            top: parent.top
        }
    }
}
