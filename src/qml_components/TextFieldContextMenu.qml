import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion as FusionC

Menu {
    id: control

    required property FusionC.TextField editor

    ContextMenuItem {
        enabled: control.editor.canUndo
        icon.name: "arrow-back-up"
        text: qsTr("Undo")

        onTriggered: control.editor.undo()
    }

    ContextMenuItem {
        enabled: control.editor.canRedo
        icon.name: "arrow-forward-up"
        text: qsTr("Redo")

        onTriggered: control.editor.redo()
    }

    MenuSeparator {}

    ContextMenuItem {
        enabled: !control.editor.readOnly && control.editor.selectedText.length > 0
        icon.name: "cut"
        text: qsTr("Cut")

        onTriggered: control.editor.cut()
    }

    ContextMenuItem {
        enabled: control.editor.selectedText.length > 0
        icon.name: "copy"
        text: qsTr("Copy")

        onTriggered: control.editor.copy()
    }

    ContextMenuItem {
        enabled: !control.editor.readOnly
        icon.name: "clipboard"
        text: qsTr("Paste")

        onTriggered: control.editor.paste()
    }

    ContextMenuItem {
        enabled: !control.editor.readOnly && control.editor.selectedText.length > 0
        icon.name: "trash"
        text: qsTr("Delete")

        onTriggered: control.editor.remove(control.editor.selectionStart, control.editor.selectionEnd)
    }

    MenuSeparator {}

    ContextMenuItem {
        icon.name: "select-all"
        text: qsTr("Select All")

        onTriggered: control.editor.selectAll()
    }

    component ContextMenuItem: MenuItem {
        icon.color: palette.text
    }
}
