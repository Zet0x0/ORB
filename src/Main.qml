import ORB.Common
import ORB.Player
import ORB.QmlComponents
import ORB.Settings
import ORB.Sources
import ORB.Style
import QtQuick
import QtQuick.Layouts

MainWindow {
    id: root

    function persistSession() {
        Settings.window.x = x;
        Settings.window.y = y;

        Settings.window.width = width;
        Settings.window.height = height;

        Settings.player.lastStation = Player.station;
        Settings.player.volume = Player.volume;
        Settings.player.muted = Player.muted;

        Settings.sources.lastSearchSource = sourceSelector.currentValue;
    }

    function restoreWindowPosition() {
        if (!Settings.window.hasPosition) {
            return;
        }

        const savedX = Settings.window.x;
        const savedY = Settings.window.y;

        if (Utilities.isPointOnScreen(Qt.point(savedX, savedY))) {
            x = savedX;
            y = savedY;
        }
    }

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
            title: qsTr("&Edit")

            Action {
                text: qsTr("&Preferences...")

                onTriggered: preferencesDialog.open()
            }
        }

        Menu {
            title: qsTr("&Playback")

            // HACK: A MenuItem whose `text` changes at runtime stops rendering its
            // mnemonic underline (`&`) for the new label, so instead of one item
            // that toggles Play/Stop we keep two fixed-text items and kill off
            // the hidden one by setting height to 0
            // Which QTBUG is this???

            MenuItem {
                enabled: visible && Player.station.valid
                height: visible ? implicitHeight : 0
                text: qsTr("&Play")
                visible: Player.state === Player.Stopped

                onTriggered: Player.play()
            }

            MenuItem {
                enabled: visible
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

            // HACK: Using the same hack as the Play/Stop items

            MenuItem {
                enabled: visible
                height: visible ? implicitHeight : 0
                text: qsTr("&Mute")
                visible: !Player.muted

                onTriggered: Player.muted = true
            }

            MenuItem {
                enabled: visible
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
    palette: AppPalette {}

    Component.onCompleted: {
        restoreWindowPosition();

        Player.station = Settings.player.lastStation;
        Player.volume = Settings.player.volume;
        Player.muted = Settings.player.muted;
    }
    Component.onDestruction: persistSession()
    onClosing: close => {
        if (Settings.tray.enabled && Settings.tray.closeToTray) {
            close.accepted = false;
            hide();

            return;
        }

        Qt.quit();
    }
    onHeightChanged: persistTimer.restart()
    onWidthChanged: persistTimer.restart()
    onXChanged: persistTimer.restart()
    onYChanged: persistTimer.restart()

    Timer {
        id: persistTimer

        interval: 10 * 1000 // 10 seconds

        onTriggered: root.persistSession()
    }

    Connections {
        function onMutedChanged() {
            persistTimer.restart();
        }

        function onStationChanged() {
            persistTimer.restart();
        }

        function onVolumeChanged() {
            persistTimer.restart();
        }

        target: Player
    }

    AboutDialog {
        id: aboutDialog
    }

    OpenLocationDialog {
        id: openLocationDialog
    }

    PreferencesDialog {
        id: preferencesDialog
    }

    SystemTrayIcon {
        id: systemTrayIcon

        applicationWindow: root

        trayMenu: SystemTrayMenu {
            palette: root.palette
        }
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

                RowLayout {
                    Layout.fillWidth: true

                    StationNameLabel {
                        Layout.fillWidth: true
                        Layout.maximumWidth: implicitWidth
                    }

                    ElapsedLabel {}
                }

                RowLayout {
                    PlayButton {
                        id: playButton
                    }

                    PlaybackStatusIndicator {
                        Layout.maximumHeight: playButton.height
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    VolumeControl {}
                }
            }
        }

        RowLayout {
            IconButton {
                ToolTip.text: qsTr("Open source's official website")
                enabled: !SourceController.currentSourceIsNull && SourceController.currentSourceUrl.length > 0
                icon.name: "external-link"

                onClicked: Qt.openUrlExternally(SourceController.currentSourceUrl)
            }

            ComboBox {
                id: sourceSelector

                ToolTip.text: qsTr("Station search source")
                ToolTip.visible: hovered || visualFocus
                model: SourceController.getSources()
                textRole: "name"
                valueRole: "key"

                Component.onCompleted: {
                    currentIndex = Math.max(0, indexOfValue(Settings.sources.lastSearchSource));
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
                ToolTip.text: qsTr("Search")
                enabled: stationSearchField.enabled
                icon.name: "search"

                onClicked: stationSearchField.engageSearch()
            }
        }

        StationSearch {
            sourceSelector: sourceSelector
        }
    }

    ErrorBanner {
        error: Player.error
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        onDismissed: Player.clearError()
    }
}
