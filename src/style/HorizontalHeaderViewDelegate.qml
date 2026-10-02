import QtQuick
import QtQuick.Templates as T

T.HeaderViewDelegate {
    id: root

    highlighted: selected
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 8

    background: Rectangle {
        color: root.palette.button
    }
    contentItem: Label {
        horizontalAlignment: Text.AlignHCenter
        text: root.model[root.headerView.textRole]
        verticalAlignment: Text.AlignVCenter
    }
}
