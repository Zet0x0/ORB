import QtQuick
import QtQuick.Templates as T

T.TextField {
    id: root

    color: palette.text
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding, placeholder.implicitHeight + topPadding + bottomPadding)
    implicitWidth: implicitBackgroundWidth + leftInset + rightInset || Math.max(contentWidth, placeholder.implicitWidth) + leftPadding + rightPadding
    padding: 4
    placeholderTextColor: palette.placeholderText
    selectedTextColor: palette.highlightedText
    selectionColor: palette.highlight
    verticalAlignment: TextInput.AlignVCenter

    background: Rectangle {
        border.color: root.activeFocus ? root.palette.highlight : "#00000000"
        color: root.palette.base
        implicitHeight: 24
        implicitWidth: 120
        radius: 2
    }

    Shortcut {
        enabled: root.activeFocus
        sequences: ["Menu", "Shift+F10"]

        onActivated: contextMenu.popup(root.leftPadding, root.cursorRectangle.y + root.cursorRectangle.height)
    }

    MouseArea {
        acceptedButtons: Qt.RightButton
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor

        onClicked: contextMenu.popup()
    }

    Text {
        id: placeholder

        color: root.placeholderTextColor
        elide: Text.ElideRight
        font: root.font
        height: root.height - (root.topPadding + root.bottomPadding)
        horizontalAlignment: root.horizontalAlignment
        renderType: root.renderType
        text: root.placeholderText
        verticalAlignment: root.verticalAlignment
        visible: !root.length && !root.preeditText && (!root.activeFocus || root.horizontalAlignment !== Qt.AlignHCenter)
        width: root.width - (root.leftPadding + root.rightPadding)
        x: root.leftPadding
        y: root.topPadding
    }

    TextEditingContextMenu {
        id: contextMenu

        editor: root
    }
}
