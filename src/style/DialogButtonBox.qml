import QtQuick
import QtQuick.Templates as T

T.DialogButtonBox {
    id: control

    alignment: Qt.AlignRight
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    background: Rectangle {
        color: "#00000000"
        implicitHeight: 34
        implicitWidth: 80

        Rectangle {
            color: control.palette.mid
            height: 1
            width: parent.width
        }
    }
    contentItem: ListView {
        boundsBehavior: Flickable.StopAtBounds
        implicitWidth: contentWidth
        model: control.contentModel
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem
        spacing: control.spacing
    }
    delegate: Button {}
}
