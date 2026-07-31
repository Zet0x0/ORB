import QtQuick
import QtQuick.Templates as T

T.ToolTip {
    id: control

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    margins: 6
    padding: 6
    x: parent ? Math.floor((parent.width - implicitWidth) / 2) : 0
    y: -implicitHeight - 3

    background: Rectangle {
        border.color: control.palette.toolTipText
        color: control.palette.toolTipBase
        radius: 2
    }
    contentItem: Text {
        color: control.palette.toolTipText
        font: control.font
        text: control.text
        wrapMode: Text.Wrap
    }
}
