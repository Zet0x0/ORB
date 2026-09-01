import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.ItemDelegate {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    background: Rectangle {
        color: Qt.darker(root.palette.base, root.down || root.checked ? 1.2 : (enabled && (root.hovered || root.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 20
        implicitWidth: 100
        visible: root.down || root.hovered || root.visualFocus || root.checked || root.highlighted
    }
    contentItem: IconLabel {
        alignment: root.display === IconLabel.IconOnly || root.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft
        color: root.palette.text
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
