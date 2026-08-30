import QtQuick
import QtQuick.Templates as T

T.MenuSeparator {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    verticalPadding: 1

    contentItem: Rectangle {
        color: root.palette.mid
        implicitHeight: 1
        implicitWidth: 186
    }
}
