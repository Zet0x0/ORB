import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.Button {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 4
    spacing: 6

    background: Rectangle {
        border.color: root.visualFocus ? root.palette.highlight : "#00000000"
        color: Qt.darker(root.highlighted ? root.palette.highlight : root.palette.button, root.down || root.checked ? 1.2 : (enabled && (root.hovered || root.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 24
        implicitWidth: 80
        radius: 2
        visible: !root.flat || root.down || root.checked || root.highlighted || root.visualFocus || (enabled && (root.hovered || root.visualFocus))
    }
    contentItem: IconLabel {
        color: root.palette.buttonText
        defaultIconColor: root.palette.buttonText
        display: root.display
        font: root.font
        icon: root.icon
        mirrored: root.mirrored
        spacing: root.spacing
        text: root.text
    }

    icon {
        height: 16
        width: 16
    }
}
