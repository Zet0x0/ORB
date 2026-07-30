import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.Button {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 4
    spacing: 6

    background: Rectangle {
        color: Qt.darker(control.highlighted ? control.palette.highlight : control.palette.button, control.down || control.checked ? 1.2 : (enabled && control.hovered ? 0.8 : 1.0))
        implicitHeight: 24
        implicitWidth: 80
        radius: 2
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus || (enabled && control.hovered)
    }
    contentItem: IconLabel {
        color: control.palette.buttonText
        defaultIconColor: control.palette.buttonText
        display: control.display
        font: control.font
        icon: control.icon
        mirrored: control.mirrored
        spacing: control.spacing
        text: control.text
    }

    icon {
        height: 16
        width: 16
    }
}
