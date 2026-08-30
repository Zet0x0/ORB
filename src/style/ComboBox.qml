pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Window

T.ComboBox {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: padding + (!mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    background: Rectangle {
        readonly property bool highlighted: root.visualFocus || root.contentItem.activeFocus

        border.color: highlighted ? root.palette.highlight : "#00000000"
        color: Qt.darker(root.palette.button, root.down ? 1.2 : (enabled && (root.hovered || root.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 24
        implicitWidth: 120
        radius: 2
        visible: !root.flat || root.down
    }
    contentItem: T.TextField {
        id: textField

        autoScroll: root.editable
        bottomPadding: 4
        color: root.editable ? root.palette.text : root.palette.buttonText
        enabled: root.editable
        implicitHeight: contentHeight + topPadding + bottomPadding
        inputMethodHints: root.inputMethodHints
        leftPadding: 4 - root.padding
        readOnly: root.down
        rightPadding: 4 - root.padding
        selectByMouse: root.selectTextByMouse
        selectedTextColor: root.palette.highlightedText
        selectionColor: root.palette.highlight
        text: root.editable ? root.editText : root.displayText
        topPadding: 4
        validator: root.validator
        verticalAlignment: Text.AlignVCenter

        background: PaddedRectangle {
            clip: true
            color: root.palette.base
            leftPadding: root.mirrored ? -2 : padding
            padding: 1
            radius: 2
            rightPadding: !root.mirrored ? -2 : padding
            visible: root.editable && !root.flat
        }

        Shortcut {
            enabled: textField.activeFocus
            sequences: ["Menu", "Shift+F10"]

            onActivated: contextMenu.popup(textField.leftPadding, textField.cursorRectangle.y + textField.cursorRectangle.height)
        }

        TapHandler {
            acceptedButtons: Qt.RightButton

            onTapped: event => contextMenu.popup(event.position)
        }

        TextEditingContextMenu {
            id: contextMenu

            editor: textField
        }
    }
    delegate: MenuItem {
        required property int index
        required property var model

        font.weight: root.currentIndex === index ? Font.Bold : Font.Normal
        highlighted: root.highlightedIndex === index
        hoverEnabled: root.hoverEnabled
        text: model[root.textRole]
        width: ListView.view.width
    }
    indicator: IconImage {
        color: root.editable ? root.palette.text : root.palette.buttonText
        fillMode: Image.Pad
        name: "chevron-down"
        width: 20
        x: root.mirrored ? root.padding : root.width - width - root.padding
        y: root.topPadding + Math.round((root.availableHeight - height) / 2)
    }
    popup: T.Popup {
        bottomMargin: 6
        // qmllint disable missing-property
        height: Math.min(contentItem.implicitHeight + 2, root.Window.height - topMargin - bottomMargin)
        // qmllint enable missing-property
        padding: 1
        palette: root.palette
        topMargin: 6
        width: root.width

        background: Rectangle {
            border.color: root.palette.mid
            color: root.popup.palette.window
        }
        contentItem: ListView {
            clip: true
            currentIndex: root.highlightedIndex
            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            implicitHeight: contentHeight
            model: root.delegateModel

            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }
}
