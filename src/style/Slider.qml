import QtQuick
import QtQuick.Templates as T

T.Slider {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitHandleHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitHandleWidth + leftPadding + rightPadding)

    background: Rectangle {
        color: root.palette.button
        height: root.horizontal ? implicitHeight : root.availableHeight
        implicitHeight: root.horizontal ? 3 : 160
        implicitWidth: root.horizontal ? 160 : 3
        radius: 2
        width: root.horizontal ? root.availableWidth : implicitWidth
        x: root.horizontal ? 0 : Math.round((root.availableWidth - width) / 2)
        y: root.horizontal ? Math.round((root.availableHeight - height) / 2) : 0

        Rectangle {
            color: root.palette.accent
            height: root.horizontal ? 3 : Math.round(root.position * parent.height)
            radius: 2
            width: root.horizontal ? Math.round(root.position * parent.width) : 3
            y: root.horizontal ? 0 : Math.round(root.visualPosition * parent.height)
        }
    }
    handle: Rectangle {
        border.color: root.visualFocus ? root.palette.highlight : "#00000000"
        color: root.palette.button.darker(root.pressed ? 1.2 : (enabled && (handleHoverHandler.hovered || root.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 13
        implicitWidth: 13
        radius: 2
        x: root.leftPadding + Math.round(root.horizontal ? root.visualPosition * (root.availableWidth - width) : (root.availableWidth - width) / 2)
        y: root.topPadding + Math.round(root.horizontal ? (root.availableHeight - height) / 2 : root.visualPosition * (root.availableHeight - height))

        HoverHandler {
            id: handleHoverHandler
        }
    }
}
