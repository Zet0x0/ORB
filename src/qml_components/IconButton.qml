import QtQuick
import QtQuick.Controls

Button {
    display: AbstractButton.IconOnly
    icon.color: palette.buttonText
    implicitHeight: icon.height + verticalPadding * 2
    implicitWidth: icon.width + horizontalPadding * 2
}
