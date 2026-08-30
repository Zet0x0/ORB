pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Window

T.ComboBox {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: padding + (!mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    background: Rectangle {
        readonly property bool highlighted: control.visualFocus || control.contentItem.activeFocus

        border.color: highlighted ? control.palette.highlight : "#00000000"
        color: Qt.darker(control.palette.button, control.down ? 1.2 : (enabled && (control.hovered || control.visualFocus) ? 0.8 : 1.0))
        implicitHeight: 24
        implicitWidth: 120
        radius: 2
        visible: !control.flat || control.down
    }
    contentItem: T.TextField {
        id: textField

        autoScroll: control.editable
        bottomPadding: 4
        color: control.editable ? control.palette.text : control.palette.buttonText
        enabled: control.editable
        implicitHeight: contentHeight + topPadding + bottomPadding
        inputMethodHints: control.inputMethodHints
        leftPadding: 4 - control.padding
        readOnly: control.down
        rightPadding: 4 - control.padding
        selectByMouse: control.selectTextByMouse
        selectedTextColor: control.palette.highlightedText
        selectionColor: control.palette.highlight
        text: control.editable ? control.editText : control.displayText
        topPadding: 4
        validator: control.validator
        verticalAlignment: Text.AlignVCenter

        background: PaddedRectangle {
            clip: true
            color: control.palette.base
            leftPadding: control.mirrored ? -2 : padding
            padding: 1
            radius: 2
            rightPadding: !control.mirrored ? -2 : padding
            visible: control.editable && !control.flat
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

        font.weight: control.currentIndex === index ? Font.Bold : Font.Normal
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
        text: model[control.textRole]
        width: ListView.view.width
    }
    indicator: IconImage {
        color: control.editable ? control.palette.text : control.palette.buttonText
        fillMode: Image.Pad
        name: "chevron-down"
        width: 20
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + Math.round((control.availableHeight - height) / 2)
    }
    popup: T.Popup {
        bottomMargin: 6
        height: Math.min(contentItem.implicitHeight + 2, control.Window.height - topMargin - bottomMargin)
        padding: 1
        palette: control.palette
        topMargin: 6
        width: control.width

        background: Rectangle {
            border.color: control.palette.mid
            color: control.popup.palette.window
        }
        contentItem: ListView {
            clip: true
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            implicitHeight: contentHeight
            model: control.delegateModel

            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }
}
