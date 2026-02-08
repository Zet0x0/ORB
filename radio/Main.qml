import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import radio.player

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
                Label {
                    id: nowPlayingLabel

                    enabled: state === "showing-info"
                    font.italic: !enabled
                    textFormat: Text.PlainText

                    states: [
                        State {
                            name: "playback-stopped"
                            when: !Player.station.valid || Player.state === Player.Stopped

                            PropertyChanges {
                                target: nowPlayingLabel
                                text: qsTr("Not playing anything currently")
                            }
                        },
                        State {
                            name: "no-info"
                            when: Player.nowPlaying === ""

                            PropertyChanges {
                                target: nowPlayingLabel
                                text: qsTr("No song information available")
                            }
                        },
                        State {
                            name: "showing-info"
                            when: Player.nowPlaying !== ""

                            PropertyChanges {
                                target: nowPlayingLabel
                                text: Player.nowPlaying
                            }
                        }
                    ]
                }

                Label {
                    id: stationNameLabel

                    font.italic: state !== "showing-name"
                    textFormat: Text.PlainText

                    states: [
                        State {
                            name: "no-station"
                            when: !Player.station.valid

                            PropertyChanges {
                                target: stationNameLabel
                                text: qsTr("No station selected")
                            }
                        },
                        State {
                            name: "showing-name"
                            when: Player.station.valid

                            PropertyChanges {
                                target: stationNameLabel
                                text: Player.station.name
                            }
                        }
                    ]
                }

                RowLayout {
                    ToolButton {
                        enabled: Player.station.valid
                        // TODO: use real icons
                        // TODO: make this change depending on state;
                        // NOTE: this app might or might not use icon.name
                        // instead of icon.source
                        icon.source: "https://picsum.photos/24/24"

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

                // TODO: this should probably be removed
                // along with the QML's ListModel that's
                // currently set as ComboBox's model
                textRole: "text"
                valueRole: "value"

                // TODO: retrieve the model from backend instead
                model: ListModel {
                    ListElement {
                        text: "Not selected"
                        value: -1
                    }
                }
            }

            SearchField {
                id: stationSearchField

                Layout.fillWidth: true
                enabled: sourceSelector.currentValue !== -1
            }
        }

        StackLayout {
            currentIndex: (state === "showing-stations") ? 1 : 0

            states: [
                State {
                    name: "no-source"
                    when: sourceSelector.currentValue === -1

                    PropertyChanges {
                        target: statusLabel
                        text: qsTr("# Nothing to show\n\nStart by [selecting a source](#sourceSelector)")
                    }
                },
                State {
                    name: "showing-stations"

                    // TODO: use a real variable
                    // when: results > 0

                    PropertyChanges {
                        target: statusLabel
                        text: qsTr("# You should be seeing the stations,\n\nnot this message")
                    }
                },
                State {
                    name: "empty-query"
                    when: stationSearchField.text.trim().length === 0

                    PropertyChanges {
                        target: statusLabel
                        text: qsTr("# Nothing to show\n\nYour search query is empty")
                    }
                },
                State {
                    name: "error"

                    // TODO: use a real variable
                    // when: results < 0

                    PropertyChanges {
                        target: statusLabel
                        // TODO: use real variables and insert them as needed
                        text: qsTr("# Error title\n\nError message")
                    }
                },
                State {
                    name: "no-results"

                    // TODO: use a real variable
                    // when: results === 0

                    PropertyChanges {
                        target: statusLabel
                        text: qsTr("# Nothing found\n\nCheck your search query")
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

            // TODO: show the results in this ListView
            ListView {
            }
        }
    }
}
