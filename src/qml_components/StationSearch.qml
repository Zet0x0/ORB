import ORB.Sources
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: control

    required property ComboBox sourceSelector

    currentIndex: state === "showing-stations" ? 1 : 0

    states: [
        State {
            name: "no-source"
            when: control.sourceSelector.currentIndex === 0

            PropertyChanges {
                statusLabel.text: qsTr("# Nothing to show\nStart by [selecting a source](#sourceSelector)")
            }
        },
        State {
            name: "searching"
            when: SourceController.searchState === SourceController.Searching

            PropertyChanges {
                statusLabel.text: qsTr("# Searching stations...")
            }
        },
        State {
            name: "error"
            when: SourceController.searchState === SourceController.Error

            PropertyChanges {
                statusLabel.text: qsTr("# %0\n%1").arg(SourceController.error.title).arg(SourceController.error.message)
            }
        },
        State {
            name: "showing-stations"
            when: stationView.count > 0

            PropertyChanges {
                statusLabel.text: qsTr("# You should be seeing the stations,\nnot this message")
            }
        },
        State {
            name: "no-default-stations"
            when: !SourceController.canShowDefaultStations && stationView.count === 0

            PropertyChanges {
                statusLabel.text: qsTr("# Nothing to show\nType something in the search field")
            }
        },
        State {
            name: "no-results"
            when: SourceController.canShowDefaultStations && stationView.count === 0

            PropertyChanges {
                statusLabel.text: qsTr("# Nothing found\nCheck your search query")
            }
        }
    ]

    Label {
        id: statusLabel

        horizontalAlignment: Qt.AlignHCenter
        textFormat: Text.MarkdownText
        verticalAlignment: Qt.AlignVCenter

        onLinkActivated: link => {
            if (link === "#sourceSelector") {
                control.sourceSelector.focus = true;
                control.sourceSelector.focusReason = Qt.TabFocusReason;
            }
        }

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
            enabled: statusLabel.hoveredLink
        }
    }

    RowLayout {
        ListView {
            id: stationView

            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.vertical: stationViewScrollBar
            activeFocusOnTab: true
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            highlightFollowsCurrentItem: false
            keyNavigationWraps: true
            model: SourceController.stationModel()
            pixelAligned: true
            spacing: 5

            delegate: StationDelegate {
                width: ListView.view.width
            }
            highlight: Rectangle {
                color: "transparent"
                height: ListView.view.currentItem?.height ?? 0
                visible: (!(ListView.view.currentItem as StationDelegate)?.hovered ?? false) && ListView.view.activeFocus
                width: ListView.view.currentItem?.width ?? 0
                y: ListView.view.currentItem?.y ?? 0
                z: 2

                border {
                    color: palette.highlight
                    width: 4
                }
            }

            onCountChanged: currentIndex = count === 0 ? -1 : 0
            onCurrentItemChanged: positionViewAtIndex(currentIndex, ListView.Contain)
        }

        ScrollBar {
            id: stationViewScrollBar

            Layout.fillHeight: true
            minimumSize: 0.1
            padding: 0
            policy: visible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            visible: stationView.contentHeight > stationView.height

            contentItem: Rectangle {
                color: stationViewScrollBar.pressed ? palette.midlight : palette.light
                implicitHeight: 100
                implicitWidth: 6
                radius: Math.floor(width / 2)
            }
        }
    }
}
