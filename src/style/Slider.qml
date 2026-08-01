import QtQuick
import QtQuick.Templates as T

T.Slider {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitHandleHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitHandleWidth + leftPadding + rightPadding)

    background: Rectangle {
        color: control.palette.button
        height: control.horizontal ? implicitHeight : control.availableHeight
        implicitHeight: control.horizontal ? 3 : 160
        implicitWidth: control.horizontal ? 160 : 3
        radius: 2
        width: control.horizontal ? control.availableWidth : implicitWidth
        x: control.horizontal ? 0 : Math.round((control.availableWidth - width) / 2)
        y: control.horizontal ? Math.round((control.availableHeight - height) / 2) : 0

        Rectangle {
            color: control.palette.accent
            height: control.horizontal ? 3 : Math.round(control.position * parent.height)
            radius: 2
            width: control.horizontal ? Math.round(control.position * parent.width) : 3
            y: control.horizontal ? 0 : Math.round(control.visualPosition * parent.height)
        }
    }
    handle: Rectangle {
        border.color: control.visualFocus ? control.palette.highlight : "#00000000"
        color: Qt.darker(control.palette.button, control.pressed ? 1.2 : (enabled && (handleHoverHandler.hovered || control.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 13
        implicitWidth: 13
        radius: 2
        x: control.leftPadding + Math.round(control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + Math.round(control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))

        HoverHandler {
            id: handleHoverHandler
        }
    }
}
