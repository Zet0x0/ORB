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

    enabled: sourceSelector.currentIndex > 0
    rightPadding: leftPadding + clearSearchFieldButton.width

    onAccepted: engageSearch()

    IconButton {
        id: clearSearchFieldButton

        ToolTip.text: qsTr("Clear")
        enabled: control.enabled
        icon.name: "x"
        implicitHeight: parent.height
        z: 1

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
