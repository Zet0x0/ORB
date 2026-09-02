import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.MenuItem {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    arrow: IconImage {
        color: root.palette.text
        height: 20
        name: root.subMenu ? (root.mirrored ? "chevron-left" : "chevron-right") : ""
        sourceSize: Qt.size(width, height)
        visible: root.subMenu
        width: 20
        x: root.mirrored ? root.leftPadding : root.width - width - root.rightPadding
        y: root.topPadding + Math.round((root.availableHeight - height) / 2)
    }
    background: Rectangle {
        readonly property bool active: root.enabled && (root.hovered || root.visualFocus || (root.subMenu && root.subMenu.opened) || root.highlighted)

        color: root.palette.base.darker(root.down ? 1.2 : (active ? 0.8 : 1.0))
        implicitHeight: 20
        implicitWidth: 150
        visible: root.down || active
    }
    contentItem: IconLabel {
        readonly property real arrowPadding: root.subMenu && root.arrow ? root.arrow.width + root.spacing : 0
        readonly property real indicatorPadding: root.checkable && root.indicator ? root.indicator.width + root.spacing : 0

        alignment: Qt.AlignLeft
        color: root.palette.text
        defaultIconColor: root.palette.text
        display: root.display
        font: root.font
        icon: root.icon
        leftPadding: !root.mirrored ? indicatorPadding : arrowPadding
        mirrored: root.mirrored
        rightPadding: root.mirrored ? indicatorPadding : arrowPadding
        spacing: root.spacing
        text: root.text
    }
    indicator: Rectangle {
        color: root.palette.base.darker(root.down ? 1.0 : 1.2)
        implicitHeight: 14
        implicitWidth: 14
        visible: root.checkable
        x: root.mirrored ? root.width - width - root.rightPadding : root.leftPadding
        y: root.topPadding + Math.round((root.availableHeight - height) / 2)

        IconImage {
            color: root.palette.text
            height: width
            name: "check"
            sourceSize: Qt.size(width, height)
            visible: root.checked
            width: parent.width
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
        }
    }

    icon {
        height: 16
        width: 16
    }
}
