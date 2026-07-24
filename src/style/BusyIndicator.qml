import QtQuick
import QtQuick.Controls.Fusion as C
import QtQuick.Templates as T

T.BusyIndicator {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)

    contentItem: Item {
        height: control.availableHeight
        implicitHeight: 24
        implicitWidth: 24
        width: control.availableWidth

        C.Button {
            background: null
            display: C.AbstractButton.IconOnly
            enabled: false
            height: width
            padding: 0
            width: Math.floor(parent.width / 2)
            x: Math.floor((parent.width - width) / 2)
            y: Math.floor((parent.height - height) / 2)

            RotationAnimator on rotation {
                duration: 600
                from: 0
                loops: Animation.Infinite
                running: control.running && control.visible
                to: 360
            }

            icon {
                color: control.palette.buttonText
                height: height
                name: "dots"
                width: width
            }
        }
    }
}
