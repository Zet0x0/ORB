import ORB.Player
import ORB.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: control

    ToolTip.text: qsTr("Attempt %0 of %1 failed. Retrying in %2s - click to retry now.").arg(Player.retryAttempt).arg(Settings.player.maxRetries).arg(Player.retrySecondsRemaining)
    ToolTip.visible: Player.state === Player.Retrying && hoverHandler.hovered
    visible: Player.state === Player.Loading || Player.state === Player.Retrying

    BusyIndicator {
        running: control.visible
    }

    Label {
        enabled: false
        text: qsTr("Retrying in %0s...").arg(Player.retrySecondsRemaining)
        textFormat: Text.PlainText
        visible: Player.state === Player.Retrying
    }

    HoverHandler {
        id: hoverHandler

        cursorShape: Player.state === Player.Retrying ? Qt.PointingHandCursor : Qt.ArrowCursor
    }

    TapHandler {
        enabled: Player.state === Player.Retrying

        onTapped: Player.retryNow()
    }
}
