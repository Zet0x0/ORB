import QtQuick
import QtQuick.Templates as T

T.ToolTip {
    id: root

    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    margins: 6
    padding: 6
    x: parent ? Math.round((parent.width - implicitWidth) / 2) : 0
    y: -implicitHeight - 3

    background: Rectangle {
        border.color: root.palette.toolTipText
        color: root.palette.toolTipBase
        radius: 2
    }
    contentItem: Text {
        color: root.palette.toolTipText
        font: root.font
        text: root.text
        wrapMode: Text.Wrap
    }
}
