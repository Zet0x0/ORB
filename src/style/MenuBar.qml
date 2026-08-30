import QtQuick
import QtQuick.Templates as T

T.MenuBar {
    id: root

    bottomPadding: SafeArea.margins.bottom
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: SafeArea.margins.left
    rightPadding: SafeArea.margins.right
    topPadding: SafeArea.margins.top

    background: Rectangle {
        color: root.palette.window
        implicitHeight: 20

        Rectangle {
            color: root.palette.mid
            height: 1
            width: parent.width
            y: parent.height - height
        }
    }
    contentItem: Row {
        spacing: root.spacing

        Repeater {
            model: root.contentModel
        }
    }
    delegate: MenuBarItem {}
}
