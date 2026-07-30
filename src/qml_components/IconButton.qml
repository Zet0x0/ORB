import QtQuick
import QtQuick.Controls

Button {
    ToolTip.visible: ToolTip.text.length > 0 && hovered
    display: AbstractButton.IconOnly
    implicitHeight: icon.height + verticalPadding * 2
    implicitWidth: icon.width + horizontalPadding * 2
}
