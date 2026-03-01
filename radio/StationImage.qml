import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: control

    required property string imageUrl

    Layout.fillHeight: false
    Layout.fillWidth: false
    currentIndex: stationImage.status === Image.Error ? 0 : stationImage.status

    // Image.Null
    // Image.Error
    Image {
        fillMode: Image.PreserveAspectCrop
        // TODO: use real placeholder image
        source: "https://picsum.photos/256/128"

        sourceSize {
            height: control.height
            width: control.width
        }
    }

    // Image.Ready
    Image {
        id: stationImage

        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        source: control.imageUrl

        sourceSize {
            height: control.height
            width: control.width
        }
    }

    // Image.Loading
    BusyIndicator {
        running: StackLayout.isCurrentItem
    }
}
