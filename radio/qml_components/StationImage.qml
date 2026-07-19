import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: control

    required property string imageUrl

    currentIndex: stationImage.status === Image.Error ? 0 : stationImage.status

    // Image.Null
    // Image.Error
    ToolButton {
        enabled: false

        icon {
            color: palette.active.text
            height: Math.floor(parent.height / 2)
            name: "radio"
            width: Math.floor(parent.width / 2)
        }
    }

    // Image.Ready
    Image {
        id: stationImage

        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        source: control.imageUrl

        sourceSize {
            height: parent.height
            width: parent.width
        }
    }

    // Image.Loading
    BusyIndicator {
        running: StackLayout.isCurrentItem
    }
}
