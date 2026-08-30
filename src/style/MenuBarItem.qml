import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.MenuBarItem {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    background: Rectangle {
        color: Qt.lighter(root.palette.base, 1.2)
        implicitHeight: 20
        implicitWidth: 20
        visible: root.down || root.hovered || root.visualFocus || (root.menu && root.menu.opened) || (root.highlighted && !root.menuBar.hovered)
    }
    contentItem: IconLabel {
        alignment: Qt.AlignLeft
        color: defaultIconColor
        defaultIconColor: root.palette.text
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
