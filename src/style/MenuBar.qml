import QtQuick
import QtQuick.Templates as T

T.MenuBar {
    id: control

    bottomPadding: SafeArea.margins.bottom
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: SafeArea.margins.left
    rightPadding: SafeArea.margins.right
    topPadding: SafeArea.margins.top

    background: Rectangle {
        color: control.palette.window
        implicitHeight: 20

        Rectangle {
            color: control.palette.mid
            height: 1
            width: parent.width
            y: parent.height - height
        }
    }
    contentItem: Row {
        spacing: control.spacing

        Repeater {
            model: control.contentModel
        }
    }
    delegate: MenuBarItem {}
}
