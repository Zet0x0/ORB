import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.BusyIndicator {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)

    contentItem: Item {
        implicitHeight: 28
        implicitWidth: 28

        IconLabel {
            display: T.AbstractButton.IconOnly
            height: width
            width: parent.width

            RotationAnimator on rotation {
                duration: 600
                from: 0
                loops: Animation.Infinite
                running: control.running && control.visible
                to: 360
            }

            icon {
                color: control.palette.windowText
                height: height
                name: "dots"
                width: width
            }
        }
    }
}
