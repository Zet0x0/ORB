import ORB.Player
import QtQuick
import QtQuick.Controls

// TODO: sliding label OR elide
Label {
    id: control

    color: state === "showing-info" ? palette.windowText : palette.disabled.windowText
    elide: Text.ElideMiddle
    font.italic: state !== "showing-info"
    textFormat: Text.PlainText

    states: [
        State {
            name: "playback-stopped"
            when: !Player.station.valid || Player.state === Player.Stopped

            PropertyChanges {
                control.text: qsTr("Not playing anything currently")
            }
        },
        State {
            name: "no-info"
            when: Player.nowPlaying === ""

            PropertyChanges {
                control.text: qsTr("No song information available")
            }
        },
        State {
            name: "showing-info"
            when: Player.nowPlaying !== ""

            PropertyChanges {
                control.text: Player.nowPlaying
            }
        }
    ]
}
