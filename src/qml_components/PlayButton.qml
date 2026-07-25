import ORB.Player
import QtQuick

IconButton {
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
