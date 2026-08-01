import QtQuick
import QtQuick.Controls

Button {
    ToolTip.visible: ToolTip.text.length > 0 && (hovered || visualFocus)
    display: AbstractButton.IconOnly
    implicitHeight: icon.height + verticalPadding * 2
    implicitWidth: icon.width + horizontalPadding * 2

    // HACK: (?): prevent hijacking TapHandlers below this button
    TapHandler {
        acceptedButtons: Qt.AllButtons & ~Qt.LeftButton
        gesturePolicy: TapHandler.ReleaseWithinBounds
    }
}
