import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import orb.common
import orb.player
import orb.sources

Dialog {
    id: control

    property station pendingStation

    function openWithText(text) {
        urlField.text = text;

        open();
    }

    anchors.centerIn: parent
    closePolicy: Popup.CloseOnEscape
    modal: true
    pendingStation.streamUrl: urlField.text
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: qsTr("Open Location")

    Component.onCompleted: {
        const playButton = standardButton(Dialog.Ok);

        playButton.text = qsTr("Set && Play");
        playButton.enabled = Qt.binding(() => urlField.acceptableInput);
    }
    onAccepted: {
        Player.setStation(pendingStation, true);
    }
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
        }
    }
}
