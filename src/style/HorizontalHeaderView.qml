pragma ComponentBehavior: Bound

import QtQuick.Templates as T

T.HorizontalHeaderView {
    implicitHeight: Math.max(1, contentHeight)
    implicitWidth: syncView ? syncView.width : 0

    delegate: HorizontalHeaderViewDelegate {}
}
