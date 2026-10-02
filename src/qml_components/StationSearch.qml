import ORB.Sources
import ORB.Style
import QtQuick
import QtQuick.Layouts

StackLayout {
    id: root

    readonly property bool showingStations: !SourceController.currentSourceIsNull && SourceController.searchState === SourceController.Idle && stationView.count > 0
    required property ComboBox sourceSelector

    currentIndex: showingStations ? 1 : 0

    Label {
        id: statusLabel

        horizontalAlignment: Qt.AlignHCenter
        text: {
            if (SourceController.currentSourceIsNull) {
                //: Keep "(#sourceSelector)" unchanged, it's the link target
                return qsTr("# Nothing to show\nStart by [selecting a source](#sourceSelector)");
            }

            if (SourceController.searchState === SourceController.Searching) {
                return qsTr("# Searching stations...");
            }

            if (SourceController.searchState === SourceController.Error) {
                return qsTr("# %0\n%1").arg(SourceController.error.title).arg(SourceController.error.message);
            }

            if (SourceController.canShowDefaultStations) {
                return qsTr("# Nothing found\nCheck your search query");
            }

            return qsTr("# Nothing to show\nType something in the search field");
        }
        textFormat: Text.MarkdownText
        verticalAlignment: Qt.AlignVCenter

        onLinkActivated: link => {
            if (link === "#sourceSelector") {
                root.sourceSelector.focus = true;
                root.sourceSelector.focusReason = Qt.TabFocusReason;
            }
        }

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
            enabled: statusLabel.hoveredLink
        }
    }

    RowLayout {
        ListView {
            id: stationView

            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.vertical: stationViewScrollBar
            activeFocusOnTab: true
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            keyNavigationWraps: true
            model: SourceController.stationModel()
            pixelAligned: true
            preferredHighlightBegin: 0
            preferredHighlightEnd: height
            spacing: 5

            delegate: StationDelegate {
                width: ListView.view.width
            }

            onActiveFocusChanged: {
                if (activeFocus) {
                    Qt.callLater(positionViewAtIndex, currentIndex, ListView.Contain);
                }
            }
            onCountChanged: currentIndex = count === 0 ? -1 : 0
        }

        LayoutScrollBar {
            id: stationViewScrollBar

            view: stationView
        }
    }
}
