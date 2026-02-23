import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import radio.player

Rectangle {
    id: control

    required property station station

    border.width: 1
    color: "#00000000"
    height: layout.implicitHeight + layout.anchors.topMargin + layout.anchors.bottomMargin
    width: ListView.view.width

    RowLayout {
        id: layout

        anchors {
            fill: parent
            margins: 8
        }

        StationImage {
            Layout.preferredHeight: layout.height
            Layout.preferredWidth: layout.height
            imageUrl: control.station.imageUrl
        }

        ColumnLayout {
            Label {
                Layout.fillWidth: true
                elide: Text.ElideMiddle
                text: control.station.name
                textFormat: Text.PlainText
            }

            ToolButton {
                // TODO: use real icons
                // NOTE: this app might or might not use icon.name
                // instead of icon.source
                icon.source: (Player.state === Player.Stopped || Player.station !== control.station) ? "https://picsum.photos/24/24" : "https://picsum.photos/20/20"

                onClicked: {
                    if (Player.station === control.station) {
                        if (Player.state === Player.Stopped) {
                            Player.play();
                        } else {
                            Player.stop();
                        }
                    } else {
                        Player.setStation(control.station, true);
                    }
                }
            }
        }
    }
}
