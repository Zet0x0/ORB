import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.CheckBox {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    contentItem: Text {
        color: control.palette.windowText
        elide: Text.ElideRight
        font: control.font
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0
        text: control.text
        verticalAlignment: Text.AlignVCenter
    }
    indicator: Rectangle {
        border.color: control.visualFocus ? control.palette.highlight : "#00000000"
        color: Qt.darker(control.palette.base, control.down ? 1.2 : (enabled && (control.hovered || control.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 14
        implicitWidth: 14
        x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + Math.round((control.availableWidth - width) / 2)
        y: control.topPadding + Math.round((control.availableHeight - height) / 2)

        IconImage {
            color: control.palette.text
            height: width
            name: "check"
            sourceSize: Qt.size(width, height)
            visible: control.checkState === Qt.Checked || (control.checked && control.checkState === undefined)
            width: parent.width
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
        }

        Rectangle {
            color: control.palette.text
            height: parent.width - 6
            visible: control.checkState === Qt.PartiallyChecked
            width: parent.width - 6
            x: 3
            y: 3
        }
    }
}
