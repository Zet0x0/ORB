import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.player
import orb.qml_components
import orb.settings
import orb.sources

ApplicationWindow {
    height: Settings.window.height
    minimumHeight: 480
    minimumWidth: 640
    title: qsTr("ORB")
    visible: true
    width: Settings.window.width

    Component.onCompleted: {
        if (Settings.window.x >= 0 && Settings.window.y >= 0) {
            x = Settings.window.x;
            y = Settings.window.y;
        }
    }
    Component.onDestruction: {
        Settings.window.x = x;
        Settings.window.y = y;

        Settings.window.width = width;
        Settings.window.height = height;
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
                Component.onDestruction: {
                    Settings.sources.lastSearchSource = currentValue;
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
