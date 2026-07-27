import ORB.Player
import QtQuick
import QtQuick.Controls

IconButton {
    ToolTip.text: Player.state === Player.Stopped ? qsTr("Play") : qsTr("Stop")
    enabled: Player.station.valid
    icon.name: Player.state === Player.Stopped ? "player-play" : "player-stop"

    onClicked: {
        if (Player.state === Player.Stopped) {
            Player.play();
        } else {
            Player.stop();
        }
    }
}
