import QtQuick
import QtQuick.Templates as T

T.ScrollBar {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    minimumSize: Math.max(orientation === Qt.Horizontal ? height / width : width / height, 0.1)
    padding: 0
    visible: policy !== T.ScrollBar.AlwaysOff

    contentItem: Rectangle {
        color: contentItemHoverHandler.hovered ? control.palette.light : control.palette.midlight
        implicitHeight: control.interactive ? 6 : 2
        implicitWidth: control.interactive ? 6 : 2
        radius: Math.floor(width / 2)

        HoverHandler {
            id: contentItemHoverHandler
        }
    }
}
