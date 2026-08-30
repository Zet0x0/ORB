import QtQuick
import QtQuick.Templates as T

T.ScrollIndicator {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 2

    contentItem: Rectangle {
        id: indicator

        color: control.palette.midlight
        implicitHeight: 2
        implicitWidth: 2
        opacity: 0.0
        radius: Math.round(Math.min(width, height) / 2)
        visible: control.size < 1.0

        states: State {
            name: "active"
            when: control.active

            PropertyChanges {
                indicator.opacity: 1.0
            }
        }
        transitions: [
            Transition {
                from: "active"

                SequentialAnimation {
                    PauseAnimation {
                        duration: 450
                    }

                    NumberAnimation {
                        duration: 200
                        property: "opacity"
                        target: indicator
                        to: 0.0
                    }
                }
            }
        ]
    }
}
