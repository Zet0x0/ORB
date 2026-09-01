import QtQuick
import QtQuick.Templates as T

T.Frame {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 9

    background: Rectangle {
        border.color: root.palette.mid
        color: "#00000000"
        radius: 2
    }
}
