import QtQuick
import QtQuick.Templates as T

T.TextField {
    id: control

    color: palette.text
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding, placeholder.implicitHeight + topPadding + bottomPadding)
    implicitWidth: implicitBackgroundWidth + leftInset + rightInset || Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding
    padding: 4
    placeholderTextColor: palette.placeholderText
    selectedTextColor: palette.highlightedText
    selectionColor: palette.highlight
    verticalAlignment: TextInput.AlignVCenter

    background: Rectangle {
        border.color: control.activeFocus ? control.palette.highlight : "#00000000"
        color: control.palette.base
        implicitHeight: 24
        implicitWidth: 120
        radius: 2
    }

    Shortcut {
        enabled: control.activeFocus
        sequences: ["Menu", "Shift+F10"]

        onActivated: contextMenu.popup(Qt.point(control.leftPadding, control.cursorRectangle.y + control.cursorRectangle.height))
    }

    TapHandler {
        acceptedButtons: Qt.RightButton

        onTapped: event => contextMenu.popup(event.position)
    }

    Text {
        id: placeholder

        color: control.placeholderTextColor
        elide: Text.ElideRight
        font: control.font
        height: control.height - (control.topPadding + control.bottomPadding)
        horizontalAlignment: control.horizontalAlignment
        renderType: control.renderType
        text: control.placeholderText
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        width: control.width - (control.leftPadding + control.rightPadding)
        x: control.leftPadding
        y: control.topPadding
    }

    TextEditingContextMenu {
        id: contextMenu

        editor: control
    }
}
