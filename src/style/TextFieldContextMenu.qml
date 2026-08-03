import QtQuick
import QtQuick.Controls

Menu {
    id: control

    required property TextField editor

    Action {
        enabled: control.editor.canUndo
        icon.name: "arrow-back-up"
        text: qsTr("Undo")

        onTriggered: control.editor.undo()
    }

    Action {
        enabled: control.editor.canRedo
        icon.name: "arrow-forward-up"
        text: qsTr("Redo")

        onTriggered: control.editor.redo()
    }

    MenuSeparator {}

    Action {
        enabled: !control.editor.readOnly && control.editor.selectedText.length > 0
        icon.name: "cut"
        text: qsTr("Cut")

        onTriggered: control.editor.cut()
    }

    Action {
        enabled: control.editor.selectedText.length > 0
        icon.name: "copy"
        text: qsTr("Copy")

        onTriggered: control.editor.copy()
    }

    Action {
        enabled: !control.editor.readOnly
        icon.name: "clipboard"
        text: qsTr("Paste")

        onTriggered: control.editor.paste()
    }

    Action {
        enabled: !control.editor.readOnly && control.editor.selectedText.length > 0
        icon.name: "trash"
        text: qsTr("Delete")

        onTriggered: control.editor.remove(control.editor.selectionStart, control.editor.selectionEnd)
    }

    MenuSeparator {}

    Action {
        icon.name: "select-all"
        text: qsTr("Select All")

        onTriggered: control.editor.selectAll()
    }
}
