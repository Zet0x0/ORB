import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.MenuBarItem {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    background: Rectangle {
        color: Qt.lighter(control.palette.base, 1.2)
        implicitHeight: 20
        implicitWidth: 20
        visible: control.down || control.hovered || (control.menu && control.menu.opened) || (control.highlighted && !control.menuBar.hovered)
    }
    contentItem: IconLabel {
        alignment: Qt.AlignLeft
        color: defaultIconColor
        defaultIconColor: control.palette.text
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
