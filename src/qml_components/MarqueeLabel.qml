pragma ComponentBehavior: Bound

import ORB.Settings
import ORB.Style
import QtQuick

Label {
    id: root

    readonly property real boxWidth: marqueeing ? width : contentWidth
    property color fadeColor: palette.window
    property real fadeWidth: 12 // width of each edge fade gradient, in pixels
    readonly property bool marqueeing: sliding && truncated
    property bool paused: false
    property real scrollSpacing: 64 // gap between the repeated copies, in pixels

    property real scrollSpeed: 64 // pixels per second
    property bool sliding: Settings.interface.slidingLabels
    property color textColor: palette.windowText

    function restartScroll(): void {
        scroll.stop();
        track.x = 0;
        scroll.running = Qt.binding(() => root.marqueeing && root.visible);
    }

    color: marqueeing ? "#00000000" : textColor
    elide: Text.ElideRight
    maximumLineCount: 1

    onTextChanged: Qt.callLater(restartScroll)

    TextMetrics {
        id: metrics

        font: root.font
        text: root.text
    }

    Item {
        id: viewport

        anchors.fill: parent
        clip: true
        visible: root.marqueeing

        Row {
            id: track

            height: viewport.height
            spacing: root.scrollSpacing

            Repeater {
                model: 2

                Text {
                    color: root.textColor
                    font: root.font
                    height: track.height
                    text: root.text
                    textFormat: root.textFormat
                    verticalAlignment: root.verticalAlignment
                }
            }
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            opacity: track.x < 0 && track.x > -metrics.advanceWidth ? 1 : 0
            width: Math.min(root.fadeWidth, Math.round(viewport.width / 2))

            gradient: Gradient {
                orientation: Gradient.Horizontal

                GradientStop {
                    color: root.fadeColor
                    position: 0
                }

                GradientStop {
                    color: "#00000000"
                    position: 1
                }
            }
            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: parent.top
            width: Math.min(root.fadeWidth, Math.round(viewport.width / 2))

            gradient: Gradient {
                orientation: Gradient.Horizontal

                GradientStop {
                    color: "#00000000"
                    position: 0
                }

                GradientStop {
                    color: root.fadeColor
                    position: 1
                }
            }
        }

        SequentialAnimation {
            id: scroll

            loops: Animation.Infinite
            paused: running && root.paused
            running: root.marqueeing && root.visible

            PropertyAction {
                property: "x"
                target: track
                value: 0
            }

            PauseAnimation {
                duration: 2000
            }

            NumberAnimation {
                duration: Math.max(16, (metrics.advanceWidth + root.scrollSpacing) / root.scrollSpeed * 1000)
                easing.type: Easing.Linear
                property: "x"
                target: track
                to: -(metrics.advanceWidth + root.scrollSpacing)
            }
        }
    }
}
