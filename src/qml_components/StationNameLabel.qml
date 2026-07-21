import QtQuick
import QtQuick.Controls
import orb.player

Label {
    id: control

    elide: Text.ElideMiddle
    font.italic: state !== "showing-name"
    textFormat: Text.PlainText

    states: [
        State {
            name: "no-station"
            when: !Player.station.valid

            PropertyChanges {
                control.text: qsTr("No station selected")
            }
        },
        State {
            name: "showing-name"
            when: Player.station.valid

            PropertyChanges {
                control.text: Player.station.name
            }
        }
    ]
}
