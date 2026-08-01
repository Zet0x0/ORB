import ORB.Player
import ORB.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Control {
    id: control

    ToolTip.text: qsTr("Attempt %0 of %1 failed. Retrying in %2s - click to retry now.").arg(Player.retryAttempt).arg(Settings.player.maxRetries).arg(Player.retrySecondsRemaining)
    ToolTip.visible: Player.state === Player.Retrying && (hovered || visualFocus)
    activeFocusOnTab: Player.state === Player.Retrying || activeFocus
    padding: 2
    visible: Player.state === Player.Loading || Player.state === Player.Retrying

    background: Rectangle {
        border.color: control.visualFocus ? control.palette.highlight : "#00000000"
        color: "#00000000"
        radius: 2
    }
    contentItem: RowLayout {
        BusyIndicator {
            Layout.fillHeight: true
            Layout.preferredWidth: height
            running: control.visible
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
