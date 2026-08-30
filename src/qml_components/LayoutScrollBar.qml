import ORB.Style
import QtQuick
import QtQuick.Layouts

ScrollBar {
    required property Flickable view

    Layout.fillHeight: true
    policy: view.contentHeight > view.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
}
