import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.BusyIndicator {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)

    contentItem: Item {
        implicitHeight: 28
        implicitWidth: 28

        IconImage {
            color: root.palette.windowText
            height: width
            name: "dots"
            sourceSize: Qt.size(width, height)
            width: parent.width

            RotationAnimator on rotation {
                duration: 600
                from: 0
                loops: Animation.Infinite
                running: root.running && root.visible
                to: 360
            }
        }
    }
}
