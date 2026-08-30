import ORB.Common
import ORB.Player
import ORB.Sources
import ORB.Style
import QtQuick
import QtQuick.Layouts

Dialog {
    id: control

    property station pendingStation

    function openWithText(text) {
        urlField.text = text.trim();

        open();
    }

    closePolicy: Popup.CloseOnEscape
    modal: true
    pendingStation.streamUrl: urlField.text
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: qsTr("Open Location")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    Component.onCompleted: {
        const playButton = standardButton(Dialog.Ok);

        playButton.text = qsTr("Play");
        playButton.enabled = Qt.binding(() => urlField.acceptableInput);
    }
    onAccepted: Player.setStation(pendingStation, true)
    onOpened: {
        urlField.forceActiveFocus();
        urlField.selectAll();
    }

    ColumnLayout {
        Label {
            text: qsTr("Please enter the URL or path to the media you want to play.")
            wrapMode: Label.Wrap
        }

        TextField {
            id: urlField

            Layout.fillWidth: true
            placeholderText: qsTr("Enter URL here...")

            validator: UrlValidator {}

            onAccepted: control.accept()
        }
    }
}
