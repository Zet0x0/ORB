import QtQuick
import QtQuick.Controls
import radio.player

Label {
    color: palette.disabled.windowText
    text: Player.elapsed
    visible: Player.state === Player.Playing
}
