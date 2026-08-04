import ORB.Sources
import QtQuick
import QtQuick.Controls

TextField {
    id: control

    required property ComboBox sourceSelector

    function engageSearch() {
        const query = text.trim();

        if (query.length === 0) {
            SourceController.showDefaultStations();

            return;
        }

        SourceController.search(text);
    }

    enabled: !SourceController.currentSourceIsNull
    rightPadding: leftPadding + clearSearchFieldButton.width

    onAccepted: engageSearch()

    IconButton {
        id: clearSearchFieldButton

        ToolTip.text: qsTr("Clear")
        enabled: control.enabled
        flat: true
        icon.name: "circle-x"
        implicitHeight: parent.height

        onClicked: {
            control.clear();
            control.engageSearch();
        }

        anchors {
            right: parent.right
            top: parent.top
        }
    }
}
