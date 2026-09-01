import ORB.Style
import QtQuick
import QtQuick.Layouts

StackLayout {
    id: root

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
            height: Math.round(parent.height / 2)
            name: "radio"
            width: Math.round(parent.width / 2)
        }
    }

    // Image.Ready
    Image {
        id: stationImage

        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        source: root.imageUrl

        sourceSize {
            height: parent.height
            width: parent.width
        }
    }

    // Image.Loading
    Item {
        id: loadingIndicatorParent

        BusyIndicator {
            height: Math.round(parent.height / 2)
            running: loadingIndicatorParent.StackLayout.isCurrentItem
            width: Math.round(parent.width / 2)
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
        }
    }
}
