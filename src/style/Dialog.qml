pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T

T.Dialog {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0) + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0))
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding, implicitHeaderWidth, implicitFooterWidth)
    padding: 6
    z: 1

    T.Overlay.modal: Rectangle {
        color: Qt.alpha(control.palette.dark, 0.33)
    }
    T.Overlay.modeless: Rectangle {
        color: Qt.alpha(control.palette.dark, 0.33)
    }
    background: Rectangle {
        border.color: control.palette.mid
        color: control.palette.window
        radius: 2
    }
    footer: DialogButtonBox {
        visible: count > 0
    }
    header: Label {
        elide: Label.ElideRight
        font.bold: true
        padding: 6
        text: control.title
        visible: control.title && parent?.parent === T.Overlay.overlay

        background: Rectangle {
            color: "#00000000"
            height: parent.height
            width: parent.width

            Rectangle {
                color: control.palette.mid
                height: 1
                width: parent.width
                y: parent.height - height
            }
        }
    }
}
