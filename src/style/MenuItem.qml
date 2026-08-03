import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.MenuItem {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    arrow: IconImage {
        color: control.palette.text
        height: 20
        name: control.subMenu ? (control.mirrored ? "chevron-left" : "chevron-right") : ""
        sourceSize: Qt.size(width, height)
        visible: control.subMenu
        width: 20
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + Math.round((control.availableHeight - height) / 2)
    }
    background: Rectangle {
        readonly property bool active: control.enabled && (control.hovered || control.visualFocus || (control.subMenu && control.subMenu.opened) || control.highlighted)

        color: Qt.darker(control.palette.base, control.down ? 1.2 : (active ? 0.8 : 1.0))
        implicitHeight: 20
        implicitWidth: 150
        visible: control.down || active
    }
    contentItem: IconLabel {
        readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.checkable && control.indicator ? control.indicator.width + control.spacing : 0

        alignment: Qt.AlignLeft
        color: control.palette.text
        defaultIconColor: control.palette.text
        display: control.display
        font: control.font
        icon: control.icon
        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        mirrored: control.mirrored
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding
        spacing: control.spacing
        text: control.text
    }
    indicator: Rectangle {
        color: Qt.darker(control.palette.base, control.down ? 1.0 : 1.2)
        implicitHeight: 14
        implicitWidth: 14
        visible: control.checkable
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + Math.round((control.availableHeight - height) / 2)

        IconLabel {
            display: T.AbstractButton.IconOnly
            height: width
            visible: control.checked
            width: parent.width
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)

            icon {
                color: control.palette.text
                height: height
                name: "check"
                width: width
            }
        }
    }

    icon {
        height: 16
        width: 16
    }
}
