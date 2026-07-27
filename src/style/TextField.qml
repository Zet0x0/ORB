import ORB.QmlComponents
import QtQuick
import QtQuick.Controls.Fusion as C

C.TextField {
    id: control

    // i dont like the ugly icon styling of yours
    // and i dont like the way you position the popup
    C.ContextMenu.menu: null

    C.ContextMenu.onRequested: {
        const pos = hoverHandler.point.position;

        const overOverlay = children.find(child => child.visible && child.z > 0 && child.contains(Qt.point(pos.x - child.x, pos.y - child.y)));

        if (!overOverlay) {
            contextMenu.popup(pos.x, pos.y);
        }
    }

    TextFieldContextMenu {
        id: contextMenu

        editor: control
    }

    HoverHandler {
        id: hoverHandler
    }
}
