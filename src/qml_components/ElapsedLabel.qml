import QtQuick
import QtQuick.Controls
import orb.player

Label {
    color: palette.disabled.windowText
    text: Player.elapsed
    visible: Player.state === Player.Playing
}
