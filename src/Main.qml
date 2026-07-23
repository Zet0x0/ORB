import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.common
import orb.player
import orb.qml_components
import orb.settings
import orb.sources

ApplicationWindow {
    height: Settings.window.height
    maximumHeight: Screen.height
    maximumWidth: Screen.width
    minimumHeight: 480
    minimumWidth: 640
    visible: true
    width: Settings.window.width

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            Action {
                shortcut: StandardKey.Paste
                text: qsTr("&Open Location from Clipboard...")

                onTriggered: openLocationDialog.openWithText(Utilities.pasteFromClipboard())
            }

            Action {
                enabled: Player.station.valid
                text: qsTr("&Copy Stream URL")

                onTriggered: Utilities.copyToClipboard(Player.station.streamUrl)
            }

            MenuSeparator {}

            Action {
                text: qsTr("&Quit")

                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: qsTr("&Playback")

            Action {
                enabled: Player.station.valid
                text: Player.state === Player.Stopped ? qsTr("&Play") : qsTr("&Stop")

                onTriggered: {
                    if (Player.state === Player.Stopped) {
                        Player.play();
                    } else {
                        Player.stop();
                    }
                }
            }
        }

        Menu {
            title: qsTr("&Help")

            Action {
                text: qsTr("&About")

                onTriggered: aboutDialog.open()
            }
        }
    }

    Component.onCompleted: {
        if (Settings.window.x >= 0 && Settings.window.y >= 0) {
            x = Settings.window.x;
            y = Settings.window.y;
        }

        Player.station = Settings.player.lastStation;
    }
    Component.onDestruction: {
        Settings.window.x = x;
        Settings.window.y = y;

        Settings.window.width = width;
        Settings.window.height = height;

        Settings.player.lastStation = Player.station;

        Settings.sources.lastSearchSource = sourceSelector.currentValue;
    }

    AboutDialog {
        id: aboutDialog
    }

    OpenLocationDialog {
        id: openLocationDialog
    }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 7
        }

        RowLayout {
            StationImage {
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.height
                imageUrl: Player.station.imageUrl
            }

            ColumnLayout {
                Layout.fillWidth: true

                NowPlayingLabel {
                    Layout.fillWidth: true
                }

                Row {
                    Layout.fillWidth: true
                    spacing: parent.spacing

                    StationNameLabel {
                        width: Math.min(parent.width - (elapsedLabel.visible ? elapsedLabel.width + parent.spacing : 0), implicitWidth)
                    }

                    ElapsedLabel {
                        id: elapsedLabel
                    }
                }

                RowLayout {
                    PlayButton {}
                }
            }
        }

        RowLayout {
            ComboBox {
                id: sourceSelector

                currentValue: Settings.sources.lastSearchSource
                model: SourceController.getSources()
                textRole: "name"
                valueRole: "key"

                Component.onCompleted: {
                    if (currentIndex === -1) {
                        currentIndex = 0;
                    }
                }
                onCurrentValueChanged: {
                    SourceController.setSource(currentValue);

                    stationSearchField.engageSearch();
                }
            }

            StationSearchField {
                id: stationSearchField

                Layout.fillWidth: true
                sourceSelector: sourceSelector
            }

            IconButton {
                enabled: stationSearchField.enabled
                icon.name: "search"

                onClicked: stationSearchField.engageSearch()
            }
        }

        StationSearch {
            sourceSelector: sourceSelector
        }
    }
}
