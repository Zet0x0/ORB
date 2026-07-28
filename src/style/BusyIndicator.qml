import QtQuick
import QtQuick.Controls.Fusion as C
import QtQuick.Templates as T

T.BusyIndicator {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)

    contentItem: Item {
        height: control.availableHeight
        width: control.availableWidth

        C.Button {
            background: null
            display: C.AbstractButton.IconOnly
            enabled: false
            height: width
            padding: 0
            width: parent.width

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
