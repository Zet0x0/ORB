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

            StackLayout {
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: playerStatus.height
                Layout.preferredWidth: playerStatus.height
                currentIndex: (stationImage.status === Image.Error) ? 0 : stationImage.status

                // Image.Null
                // Image.Error
                Image {
                    fillMode: Image.PreserveAspectCrop
                    // TODO: use real placeholder image
                    source: "https://picsum.photos/256/128"
                }

                // Image.Ready
                Image {
                    id: stationImage

                    asynchronous: true
                    cache: false
                    fillMode: Image.PreserveAspectCrop
                    source: Player.station.imageUrl
                }

                // Image.Loading
                BusyIndicator {
                    running: StackLayout.isCurrentItem
                }
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
                        width: Math.min(parent.width - ((elapsedLabel.visible) ? elapsedLabel.width + parent.spacing : 0), implicitWidth)

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
                    ToolButton {
                        enabled: Player.station.valid
                        // TODO: use real icons
                        // NOTE: this app might or might not use icon.name
                        // instead of icon.source
                        icon.source: (Player.state === Player.Stopped) ? "https://picsum.photos/24/24" : "https://picsum.photos/24/24"

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
            }

            SearchField {
                id: stationSearchField

                Layout.fillWidth: true
                enabled: sourceSelector.currentIndex > 0

                onSearchTriggered: {
                    const query = text.trim();

                    if (query.length === 0) {
                        SourceController.loadDefaultStations(sourceSelector.currentText);

                        return;
                    }

                    SourceController.search(sourceSelector.currentValue, text);
                }
            }
        }

        StackLayout {
            currentIndex: (state === "showing-stations") ? 1 : 0

            states: [
                State {
                    name: "no-source"
                    when: sourceSelector.currentIndex === 0

                    PropertyChanges {
                        statusLabel.text: qsTr("# Nothing to show\nStart by [selecting a source](#sourceSelector)")
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
                    name: "looking-up"
                    when: SourceController.searchState === SourceController.Searching

                    PropertyChanges {
                        statusLabel.text: qsTr("# Looking up stations...")
                    }
                },
                State {
                    name: "error"
                    when: SourceController.searchState === SourceController.Error

                    PropertyChanges {
                        statusLabel.text: qsTr("# %0\n%1").arg(SourceController.searchError.title).arg(SourceController.searchError.message)
                    }
                },
                State {
                    name: "no-default-stations"
                    when: !SourceController.hasDefaultStations(sourceSelector.currentText) && stationView.count === 0

                    PropertyChanges {
                        statusLabel.text: qsTr("# Nothing to show\nType something in the search field")
                    }
                },
                State {
                    name: "no-results"
                    when: SourceController.hasDefaultStations(sourceSelector.currentText) && stationView.count === 0

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

            ListView {
                id: stationView

                clip: true
                model: SourceController.stationModel
            }
        }
    }
}
