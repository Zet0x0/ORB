import QtQuick

Menu {
    id: root

    required property TextInput editor

    Action {
        enabled: root.editor.canUndo
        icon.name: "arrow-back-up"
        text: qsTr("Undo")

        onTriggered: root.editor.undo()
    }

    Action {
        enabled: root.editor.canRedo
        icon.name: "arrow-forward-up"
        text: qsTr("Redo")

        onTriggered: root.editor.redo()
    }

    MenuSeparator {}

    Action {
        enabled: !root.editor.readOnly && root.editor.selectedText.length > 0
        icon.name: "cut"
        text: qsTr("Cut")

        onTriggered: root.editor.cut()
    }

    Action {
        enabled: root.editor.selectedText.length > 0
        icon.name: "copy"
        text: qsTr("Copy")

        onTriggered: root.editor.copy()
    }

    Action {
        enabled: !root.editor.readOnly
        icon.name: "clipboard"
        text: qsTr("Paste")

        onTriggered: root.editor.paste()
    }

    Action {
        enabled: !root.editor.readOnly && root.editor.selectedText.length > 0
        icon.name: "trash"
        text: qsTr("Delete")

        onTriggered: root.editor.remove(root.editor.selectionStart, root.editor.selectionEnd)
    }

    MenuSeparator {}

    Action {
        icon.name: "select-all"
        text: qsTr("Select All")

        onTriggered: root.editor.selectAll()
    }
}
