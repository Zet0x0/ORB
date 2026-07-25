import ORB.Player
import QtQuick
import QtQuick.Controls

Label {
    color: palette.disabled.windowText
    text: Player.elapsed
    visible: Player.state === Player.Playing
}
