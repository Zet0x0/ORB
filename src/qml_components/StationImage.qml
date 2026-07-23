import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

StackLayout {
    id: control

    required property string imageUrl

    currentIndex: {
        switch (stationImage.status) {
        case Image.Ready:
            return 1;
        case Image.Loading:
            return 2;
        default:
            return 0; // Image.Null, Image.Error, or any other unexpected value
        }
    }

    // Image.Null
    // Image.Error
    ToolButton {
        enabled: false

        icon {
            color: palette.active.buttonText
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
