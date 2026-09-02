import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.SpinBox {
    id: root

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, up.implicitIndicatorHeight + down.implicitIndicatorHeight)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentItem.implicitWidth + leftPadding + rightPadding)
    leftPadding: padding + (mirrored ? (up.indicator ? up.indicator.width : 0) : 0)
    padding: 4
    rightPadding: padding + (!mirrored ? (up.indicator ? up.indicator.width : 0) : 0)

    background: Rectangle {
        id: background

        readonly property bool focused: root.activeFocus || contextMenu.visible

        border.color: focused ? root.palette.highlight : root.palette.mid
        color: root.palette.base
        implicitHeight: 24
        implicitWidth: 120
        radius: 2

        Rectangle {
            color: root.palette.mid
            height: background.focused ? parent.height - 2 : parent.height
            width: 1
            x: {
                const value = Math.max(root.up.indicator ? root.up.indicator.width : 0, root.down.indicator ? root.down.indicator.width : 0) + 2;

                return root.mirrored ? value : parent.width - value;
            }
            y: background.focused ? 1 : 0
        }
    }
    // HACK: trying to keep textInput's width even to prevent text
    // from going out of selection rectangle's bounds horizontally
    contentItem: Item {
        implicitHeight: textInput.implicitHeight
        implicitWidth: textInput.implicitWidth

        TextInput {
            id: textInput

            clip: width < implicitWidth
            color: root.palette.text
            font: root.font
            height: parent.height
            horizontalAlignment: Qt.AlignHCenter
            inputMethodHints: root.inputMethodHints
            readOnly: !root.editable
            selectedTextColor: root.palette.highlightedText
            selectionColor: root.palette.highlight
            text: root.displayText
            validator: root.validator
            verticalAlignment: Qt.AlignVCenter
            width: parent.width - parent.width % 2

            Shortcut {
                enabled: textInput.activeFocus
                sequences: ["Menu", "Shift+F10"]

                onActivated: contextMenu.popup(textInput.leftPadding, textInput.cursorRectangle.y + textInput.cursorRectangle.height)
            }

            MouseArea {
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                cursorShape: root.editable ? Qt.IBeamCursor : Qt.ArrowCursor

                onClicked: contextMenu.popup()
            }

            TextEditingContextMenu {
                id: contextMenu

                editor: textInput
            }
        }
    }
    down.indicator: PaddedRectangle {
        bottomLeftRadius: root.mirrored ? 2 : 0
        bottomRightRadius: root.mirrored ? 0 : 2
        clip: true
        color: palette.button.darker(root.down.pressed ? 1.2 : (root.down.hovered ? 0.8 : 1.0))
        height: Math.round(root.height / 2) - 1
        implicitHeight: 10
        implicitWidth: 15
        x: root.mirrored ? 1 : root.width - width - 1
        y: root.height - height - 1

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
        color: palette.button.darker(root.up.pressed ? 1.2 : (root.up.hovered ? 0.8 : 1.0))
        height: Math.round(root.height / 2) - 1
        implicitHeight: 10
        implicitWidth: 15
        topLeftRadius: root.mirrored ? 2 : 0
        topRightRadius: root.mirrored ? 0 : 2
        x: root.mirrored ? 1 : root.width - width - 1
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
        bottom: Math.min(root.from, root.to)
        locale: root.locale.name
        top: Math.max(root.from, root.to)
    }
}
