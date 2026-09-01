import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.CheckBox {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    contentItem: Text {
        color: root.palette.windowText
        elide: Text.ElideRight
        font: root.font
        leftPadding: root.indicator && !root.mirrored ? root.indicator.width + root.spacing : 0
        rightPadding: root.indicator && root.mirrored ? root.indicator.width + root.spacing : 0
        text: root.text
        verticalAlignment: Text.AlignVCenter
    }
    indicator: Rectangle {
        border.color: root.visualFocus ? root.palette.highlight : "#00000000"
        color: Qt.darker(root.palette.base, root.down ? 1.2 : (enabled && (root.hovered || root.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 14
        implicitWidth: 14
        x: root.text ? (root.mirrored ? root.width - width - root.rightPadding : root.leftPadding) : root.leftPadding + Math.round((root.availableWidth - width) / 2)
        y: root.topPadding + Math.round((root.availableHeight - height) / 2)

        IconImage {
            color: root.palette.text
            height: width
            name: "check"
            sourceSize: Qt.size(width, height)
            visible: root.checkState === Qt.Checked
            width: parent.width
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
        }

        Rectangle {
            color: root.palette.text
            height: parent.width - 6
            visible: root.checkState === Qt.PartiallyChecked
            width: parent.width - 6
            x: 3
            y: 3
        }
    }
}
