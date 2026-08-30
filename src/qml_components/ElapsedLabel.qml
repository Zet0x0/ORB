import ORB.Player
import ORB.Style
import QtQuick

Label {
    color: palette.disabled.windowText
    text: Player.elapsed
    textFormat: Text.PlainText
    visible: Player.state === Player.Playing
}
