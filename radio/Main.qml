import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import radio.player
import radio.sources

ApplicationWindow {
    height: 480
    minimumHeight: 480
    minimumWidth: 640
    title: qsTr("Unnamed Radio App")
    visible: true
    width: 640

    ColumnLayout {
        anchors {
            fill: parent
            margins: 7
        }

        RowLayout {
            id: playerStatus

            StationImage {
                Layout.preferredHeight: playerStatus.height
                Layout.preferredWidth: playerStatus.height
                imageUrl: Player.station.imageUrl
            }

            ColumnLayout {
                Layout.fillWidth: true

                Label {
                    id: nowPlayingLabel

                    Layout.fillWidth: true
                    elide: Text.ElideMiddle
                    enabled: state === "showing-info"
                    font.italic: !enabled
                    textFormat: Text.PlainText

                    states: [
                        State {
                            name: "playback-stopped"
                            when: !Player.station.valid || Player.state === Player.Stopped

                            PropertyChanges {
                                nowPlayingLabel.text: qsTr("Not playing anything currently")
                            }
                        },
                        State {
                            name: "no-info"
                            when: Player.nowPlaying === ""

                            PropertyChanges {
                                nowPlayingLabel.text: qsTr("No song information available")
                            }
                        },
                        State {
                            name: "showing-info"
                            when: Player.nowPlaying !== ""

                            PropertyChanges {
                                nowPlayingLabel.text: Player.nowPlaying
                            }
                        }
                    ]
                }

                Row {
                    Layout.fillWidth: true
                    spacing: parent.spacing

                    Label {
                        id: stationNameLabel

                        elide: Text.ElideMiddle
                        font.italic: state !== "showing-name"
                        textFormat: Text.PlainText
                        width: Math.min(parent.width - (elapsedLabel.visible ? elapsedLabel.width + parent.spacing : 0), implicitWidth)

                        states: [
                            State {
                                name: "no-station"
                                when: !Player.station.valid

                                PropertyChanges {
                                    stationNameLabel.text: qsTr("No station selected")
                                }
                            },
                            State {
                                name: "showing-name"
                                when: Player.station.valid

                                PropertyChanges {
                                    stationNameLabel.text: Player.station.name
                                }
                            }
                        ]
                    }

                    Label {
                        id: elapsedLabel

                        enabled: false
                        text: Player.elapsed
                        visible: Player.state === Player.Playing
                    }
                }

                RowLayout {
                    // NOTE: this is play/stop
                    ToolButton {
                        enabled: Player.station.valid
                        // TODO: use real icons
                        // NOTE: this app might or might not use icon.name
                        // instead of icon.source
                        icon.source: Player.state === Player.Stopped ? "https://picsum.photos/24/24" : "https://picsum.photos/24/24"

                        onClicked: {
                            if (Player.state === Player.Stopped) {
                                Player.play();
                            } else {
                                Player.stop();
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            ComboBox {
                id: sourceSelector

                model: SourceController.getSources()

                onCurrentValueChanged: {
                    SourceController.setSource(currentValue);

                    stationSearchField.engageSearch();
                }
            }

            TextField {
                id: stationSearchField

                function engageSearch() {
                    const query = text.trim();

                    if (query.length === 0) {
                        SourceController.showDefaultStations();

                        return;
                    }

                    SourceController.search(text);
                }

                Layout.fillWidth: true
                enabled: sourceSelector.currentIndex > 0

                onAccepted: engageSearch()
            }

            // NOTE: this is Search
            ToolButton {
                enabled: stationSearchField.enabled
                // TODO: use real icons
                // NOTE: this app might or might not use icon.name
                // instead of icon.source
                icon.source: "https://picsum.photos/24/24"

                onClicked: stationSearchField.engageSearch()
            }

            // NOTE: this is Reset search
            ToolButton {
                enabled: stationSearchField.enabled
                // TODO: use real icons
                // NOTE: this app might or might not use icon.name
                // instead of icon.source
                icon.source: "https://picsum.photos/24/24"

                onClicked: {
                    stationSearchField.clear();
                    stationSearchField.engageSearch();
                }
            }
        }

        StackLayout {
            currentIndex: state === "showing-stations" ? 1 : 0

            states: [
                State {
                    name: "no-source"
                    when: sourceSelector.currentIndex === 0

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
                        sourceSelector.focus = true;
                        sourceSelector.focusReason = Qt.TabFocusReason;
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
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    focusPolicy: Qt.StrongFocus
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
    }
}
