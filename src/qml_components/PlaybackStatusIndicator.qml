import ORB.Player
import ORB.Settings
import ORB.Style
import QtQuick
import QtQuick.Layouts

Control {
    id: root

    ToolTip.text: qsTr("Playback failed - retrying in %0s (retry %1 of %2). Click to retry now").arg(Player.retrySecondsRemaining).arg(Player.retryAttempt).arg(Settings.player.maxRetries)
    ToolTip.visible: Player.state === Player.Retrying && (hovered || visualFocus)
    activeFocusOnTab: Player.state === Player.Retrying || activeFocus
    padding: 2
    visible: Player.state === Player.Loading || Player.state === Player.Retrying

    background: Rectangle {
        border.color: root.visualFocus ? root.palette.highlight : "#00000000"
        color: "#00000000"
        radius: 2
    }
    contentItem: RowLayout {
        BusyIndicator {
            Layout.fillHeight: true
            Layout.preferredWidth: height
            running: root.visible
        }

        Label {
            color: palette.disabled.windowText
            text: qsTr("Retrying in %0s...").arg(Player.retrySecondsRemaining)
            textFormat: Text.PlainText
            visible: Player.state === Player.Retrying
        }
    }

    Keys.onPressed: event => {
        if (Player.state !== Player.Retrying) {
            return;
        }

        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
            Player.retryNow();

            event.accepted = true;
        }
    }

    HoverHandler {
        cursorShape: Player.state === Player.Retrying ? Qt.PointingHandCursor : Qt.ArrowCursor
    }

    TapHandler {
        enabled: Player.state === Player.Retrying

        onTapped: Player.retryNow()
    }
}
