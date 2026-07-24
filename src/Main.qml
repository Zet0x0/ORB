import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.common
import orb.player
import orb.qml_components
import orb.settings
import orb.sources

MainWindow {
    id: applicationWindow

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

            // HACK: keeping one MenuItem with text based
            // on Player.state will guarantee failure to
            // render the underscores for P in Play and
            // S in Stop - that's the reason there are
            // two buttons for one function, using a weird
            // `height` hack (QTBUG-???)

            MenuItem {
                enabled: Player.station.valid
                height: visible ? implicitHeight : 0
                text: qsTr("&Play")
                visible: Player.state === Player.Stopped

                onTriggered: Player.play()
            }

            MenuItem {
                height: visible ? implicitHeight : 0
                text: qsTr("&Stop")
                visible: Player.state !== Player.Stopped

                onTriggered: Player.stop()
            }
        }

        Menu {
            title: qsTr("&Audio")

            Action {
                enabled: Player.volume < 100
                text: qsTr("&Increase Volume")

                onTriggered: Player.volume += 5
            }

            Action {
                enabled: Player.volume > 0
                text: qsTr("&Decrease Volume")

                onTriggered: Player.volume -= 5
            }

            MenuSeparator {}

            // HACK: keeping one MenuItem with text based
            // on Player.muted will guarantee failure to
            // render the underscores for M in Muted and
            // U in Unmuted - that's the reason there are
            // two buttons for one function, using a weird
            // `height` hack (QTBUG-???)

            MenuItem {
                height: visible ? implicitHeight : 0
                text: qsTr("&Mute")
                visible: !Player.muted

                onTriggered: Player.muted = true
            }

            MenuItem {
                height: visible ? implicitHeight : 0
                text: qsTr("&Unmute")
                visible: Player.muted

                onTriggered: Player.muted = false
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
        Player.volume = Settings.player.volume;
        Player.muted = Settings.player.muted;
    }
    Component.onDestruction: {
        Settings.window.x = x;
        Settings.window.y = y;

        Settings.window.width = width;
        Settings.window.height = height;

        Settings.player.lastStation = Player.station;
        Settings.player.volume = Player.volume;
        Settings.player.muted = Player.muted;

        Settings.sources.lastSearchSource = sourceSelector.currentValue;
    }
    onClosing: close => {
        if (Settings.tray.enabled && Settings.tray.closeToTray) {
            return;
        }

        Qt.quit();
    }

    AboutDialog {
        id: aboutDialog
    }

    OpenLocationDialog {
        id: openLocationDialog
    }

    SystemTrayIcon {
        id: systemTrayIcon

        applicationWindow: applicationWindow

        trayMenu: SystemTrayMenu {}
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

                    Item {
                        Layout.fillWidth: true
                    }

                    VolumeControl {}
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
