import QtQuick
import QtQuick.Controls
import orb.sources

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

        enabled: control.enabled
        icon.name: "x"
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
