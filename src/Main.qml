import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.player
import orb.qml_components
import orb.sources

ApplicationWindow {
    height: 480
    minimumHeight: 480
    minimumWidth: 640
    title: qsTr("ORB")
    visible: true
    width: 640

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
                    PlayButton {
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
