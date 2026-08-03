import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.SpinBox {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, up.implicitIndicatorHeight + down.implicitIndicatorHeight)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentItem.implicitWidth + leftPadding + rightPadding)
    leftPadding: padding + (mirrored ? (up.indicator ? up.indicator.width : 0) : 0)
    padding: 4
    rightPadding: padding + (!mirrored ? (up.indicator ? up.indicator.width : 0) : 0)

    background: Rectangle {
        border.color: control.activeFocus ? control.palette.highlight : control.palette.mid
        color: control.palette.base
        implicitHeight: 24
        implicitWidth: 120
        radius: 2

        Rectangle {
            color: control.palette.mid
            height: control.activeFocus ? parent.height - 2 : parent.height
            width: 1
            x: {
                const value = Math.max(control.up.indicator ? control.up.indicator.width : 0, control.down.indicator ? control.down.indicator.width : 0) + 2;

                return control.mirrored ? value : parent.width - value;
            }
            y: control.activeFocus ? 1 : 0
        }
    }
    contentItem: TextInput {
        id: textInput

        clip: width < implicitWidth
        color: control.palette.text
        font: control.font
        horizontalAlignment: Qt.AlignHCenter
        inputMethodHints: control.inputMethodHints
        readOnly: !control.editable
        selectedTextColor: control.palette.highlightedText
        selectionColor: control.palette.highlight
        text: control.displayText
        validator: control.validator
        verticalAlignment: Qt.AlignVCenter
        z: 2

        Shortcut {
            enabled: textInput.activeFocus
            sequences: ["Menu", "Shift+F10"]

            onActivated: contextMenu.popup(Qt.point(textInput.leftPadding, textInput.cursorRectangle.y + textInput.cursorRectangle.height))
        }

        TapHandler {
            acceptedButtons: Qt.RightButton

            onTapped: event => contextMenu.popup(event.position)
        }

        TextEditingContextMenu {
            id: contextMenu

            editor: textInput
        }
    }
    down.indicator: PaddedRectangle {
        bottomLeftRadius: control.mirrored ? 2 : 0
        bottomRightRadius: control.mirrored ? 0 : 2
        clip: true
        color: Qt.darker(palette.button, control.down.pressed ? 1.2 : (control.down.hovered ? 0.8 : 1.0))
        height: Math.round(control.height / 2) - 1
        implicitHeight: 10
        implicitWidth: 15
        x: control.mirrored ? 1 : control.width - width - 1
        y: control.height - height - 1

        IconImage {
            color: palette.buttonText
            fillMode: Image.Pad
            height: parent.height
            name: "chevron-down"
            width: parent.width
        }
    }
    up.indicator: PaddedRectangle {
        clip: true
        color: Qt.darker(palette.button, control.up.pressed ? 1.2 : (control.up.hovered ? 0.8 : 1.0))
        height: Math.round(control.height / 2) - 1
        implicitHeight: 10
        implicitWidth: 15
        topLeftRadius: control.mirrored ? 2 : 0
        topRightRadius: control.mirrored ? 0 : 2
        x: control.mirrored ? 1 : control.width - width - 1
        y: 1

        IconImage {
            color: palette.buttonText
            fillMode: Image.Pad
            height: parent.height
            name: "chevron-up"
            width: parent.width
        }
    }
    validator: IntValidator {
        bottom: Math.min(control.from, control.to)
        locale: control.locale.name
        top: Math.max(control.from, control.to)
    }
}
