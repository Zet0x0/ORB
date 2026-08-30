import QtQuick
import QtQuick.Templates as T

T.ScrollBar {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    minimumSize: Math.max(orientation === Qt.Horizontal ? height / width : width / height, 0.1)
    padding: 0
    visible: policy !== T.ScrollBar.AlwaysOff

    contentItem: Rectangle {
        color: contentItemHoverHandler.hovered ? root.palette.light : root.palette.midlight
        implicitHeight: root.interactive ? 6 : 2
        implicitWidth: root.interactive ? 6 : 2
        radius: Math.round(width / 2)

        HoverHandler {
            id: contentItemHoverHandler
        }
    }
}
