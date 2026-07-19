import QtQuick
import QtQuick.Controls

Button {
    display: AbstractButton.IconOnly
    implicitHeight: icon.height + verticalPadding * 2
    implicitWidth: icon.width + horizontalPadding * 2

    icon {
        color: enabled ? palette.active.text : palette.disabled.text
    }
}
