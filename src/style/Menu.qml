pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Templates as T

T.Menu {
    id: control

    property bool keyboardNavigated: false

    function isSelectable(index: int): bool {
        if (index < 0 || index >= count) {
            return false;
        }

        const item = itemAt(index) as MenuItem;

        return item ? item.enabled && item.visible : false;
    }

    function selectableIndex(from: int, step: int): int {
        for (let i = from; i >= 0 && i < count; i += step) {
            if (isSelectable(i)) {
                return i;
            }
        }

        return -1;
    }

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    margins: 0
    overlap: 1
    padding: 1
    z: 2

    T.Overlay.modal: Rectangle {
        color: Qt.alpha(control.palette.dark, 0.1)
    }
    T.Overlay.modeless: Rectangle {
        color: Qt.alpha(control.palette.dark, 0.1)
    }
    background: Rectangle {
        border.color: control.palette.mid
        color: control.palette.window
        implicitHeight: 20
        implicitWidth: 200
    }
    contentItem: ListView {
        id: listView

        clip: true
        currentIndex: control.currentIndex
        focus: true
        implicitHeight: contentHeight
        interactive: Window.window ? contentHeight + control.topPadding + control.bottomPadding > control.height : false
        keyNavigationEnabled: false
        model: control.contentModel

        ScrollIndicator.vertical: ScrollIndicator {}

        Keys.onPressed: event => {
            const last = control.selectableIndex(control.count - 1, -1);
            const first = control.selectableIndex(0, 1);
            let target = -1;

            switch (event.key) {
            case Qt.Key_Down:
                {
                    control.keyboardNavigated = true;
                    target = control.selectableIndex(control.currentIndex + 1, 1);

                    if (target < 0) {
                        target = first;
                    }

                    break;
                }
            case Qt.Key_Up:
                {
                    control.keyboardNavigated = true;
                    target = control.currentIndex < 0 ? -1 : control.selectableIndex(control.currentIndex - 1, -1);

                    if (target < 0) {
                        target = last;
                    }

                    break;
                }
            case Qt.Key_Home:
                {
                    control.keyboardNavigated = true;
                    target = first;

                    break;
                }
            case Qt.Key_End:
                {
                    control.keyboardNavigated = true;
                    target = last;

                    break;
                }
            case Qt.Key_Left:
            case Qt.Key_Right:
                {
                    control.keyboardNavigated = true;

                    return;
                }
            default:
                return;
            }

            if (target < 0) {
                return;
            }

            control.currentIndex = target;
            event.accepted = true;
        }

        HoverHandler {
            property real lastX: 0
            property real lastY: 0

            onHoveredChanged: {
                if (hovered || control.keyboardNavigated) {
                    return;
                }

                const currentItem = control.currentIndex < 0 ? null : (control.itemAt(control.currentIndex) as MenuItem);

                if (currentItem && currentItem.subMenu && currentItem.subMenu.opened) {
                    return;
                }

                control.currentIndex = -1;
            }
            onPointChanged: {
                if (!hovered || (point.position.x === lastX && point.position.y === lastY)) {
                    return;
                }

                lastX = point.position.x;
                lastY = point.position.y;
                control.keyboardNavigated = false;

                const index = listView.indexAt(listView.contentX + point.position.x, listView.contentY + point.position.y);

                if (index >= 0 && !control.isSelectable(index)) {
                    control.currentIndex = -1;
                }
            }
        }
    }
    delegate: MenuItem {}

    onAboutToShow: keyboardNavigated = false
}
