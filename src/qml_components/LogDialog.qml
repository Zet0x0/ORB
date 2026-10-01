pragma ComponentBehavior: Bound

import ORB.Common
import ORB.Logging
import ORB.Style
import QtQuick
import QtQuick.Controls.impl
import QtQuick.Layouts

Dialog {
    id: root

    function levelColor(level: int): color {
        switch (level) {
        case Logger.Debug:
            return AppColors.semantic.neutral;
        case Logger.Info:
            return AppColors.semantic.info;
        case Logger.Warning:
            return AppColors.semantic.warning;
        case Logger.Critical:
        case Logger.Fatal:
            return AppColors.semantic.danger;
        default:
            return palette.text;
        }
    }

    closePolicy: Popup.CloseOnEscape
    modal: true
    standardButtons: Dialog.Close
    title: qsTr("Log Messages")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    onOpened: {
        Qt.callLater(logTable.fitColumns);
        Qt.callLater(logTable.scrollToTail);
    }

    LogFilterModel {
        id: filterModel

        query: searchField.text.trim()
    }

    Connections {
        function onCountChanged() {
            if (!root.visible) {
                return;
            }

            const stickToTail = logTable.atYEnd && logTable.tailIsNewest;

            Qt.callLater(logTable.fitColumns);

            if (stickToTail) {
                Qt.callLater(logTable.scrollToTail);
            }
        }

        function onSortChanged() {
            Qt.callLater(logTable.fitColumns);
        }

        target: filterModel
    }

    ColumnLayout {
        RowLayout {
            Layout.fillWidth: true

            Button {
                id: levelButton

                down: pressed || levelMenu.visible
                icon.name: "filter"
                rightPadding: padding + spacing + levelChevron.width
                text: filterModel.hiddenLevelCount > 0 ? qsTr("Levels (%0 hidden)").arg(filterModel.hiddenLevelCount) : qsTr("Levels")

                onClicked: levelMenu.popup(levelButton, 0, levelButton.height)

                IconImage {
                    id: levelChevron

                    color: levelButton.palette.buttonText
                    height: 16
                    name: levelMenu.visible ? "chevron-up" : "chevron-down"
                    sourceSize: Qt.size(width, height)
                    width: 16
                    y: Math.round((parent.height - height) / 2)

                    anchors {
                        right: parent.right
                        rightMargin: levelButton.padding
                    }
                }

                Menu {
                    id: levelMenu

                    MenuItem {
                        checkable: true
                        checked: true
                        text: qsTr("Debug")

                        onToggled: filterModel.setShowsLevel(Logger.Debug, checked)
                    }

                    MenuItem {
                        checkable: true
                        checked: true
                        text: qsTr("Info")

                        onToggled: filterModel.setShowsLevel(Logger.Info, checked)
                    }

                    MenuItem {
                        checkable: true
                        checked: true
                        text: qsTr("Warning")

                        onToggled: filterModel.setShowsLevel(Logger.Warning, checked)
                    }

                    MenuItem {
                        checkable: true
                        checked: true
                        text: qsTr("Critical")

                        onToggled: filterModel.setShowsLevel(Logger.Critical, checked)
                    }

                    MenuItem {
                        checkable: true
                        checked: true
                        text: qsTr("Fatal")

                        onToggled: filterModel.setShowsLevel(Logger.Fatal, checked)
                    }
                }
            }

            SearchTextField {
                id: searchField

                Layout.fillWidth: true
                placeholderText: qsTr("Filter messages...")
            }

            IconButton {
                ToolTip.text: qsTr("Copy all shown messages")
                enabled: filterModel.count > 0
                icon.name: "copy"

                onClicked: Utilities.copyToClipboard(filterModel.formattedText())
            }

            IconButton {
                ToolTip.text: qsTr("Open logs folder")
                icon.name: "folder-open"

                onClicked: Utilities.openUrlExternally(Logger.directoryUrl)
            }
        }

        Frame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: Math.round((root.parent?.height ?? 720) * 0.75)
            Layout.minimumHeight: 240
            Layout.minimumWidth: 520
            Layout.preferredHeight: Math.round((root.parent?.height ?? 720) * 0.6)
            Layout.preferredWidth: Math.round((root.parent?.width ?? 960) * 0.75)

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                HorizontalHeaderView {
                    id: logHeader

                    Layout.fillWidth: true
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    interactive: false
                    resizableColumns: false
                    syncView: logTable
                    visible: filterModel.count > 0

                    delegate: Rectangle {
                        id: headerCell

                        required property string display
                        required property int index
                        readonly property bool sortedHere: filterModel.sortedColumn === index

                        color: palette.button.darker(headerTap.pressed ? 1.2 : (enabled && (headerHover.hovered || sortedHere) ? 0.8 : 1.0))
                        implicitHeight: 26
                        implicitWidth: headerText.implicitWidth

                        onImplicitWidthChanged: Qt.callLater(logTable.fitColumns)

                        Label {
                            id: headerText

                            anchors.fill: parent
                            color: palette.buttonText
                            elide: Text.ElideRight
                            font.bold: true
                            padding: 7
                            rightPadding: padding * 2 + headerSortIndicator.width
                            text: headerCell.display
                            textFormat: Text.PlainText
                            verticalAlignment: Qt.AlignVCenter

                            IconImage {
                                id: headerSortIndicator

                                color: palette.buttonText
                                height: 16
                                name: headerCell.sortedHere ? (filterModel.sortedOrder === Qt.AscendingOrder ? "arrow-up" : "arrow-down") : ""
                                sourceSize: Qt.size(width, height)
                                visible: headerCell.sortedHere
                                width: 16
                                y: Math.round((parent.height - height) / 2)

                                anchors {
                                    right: parent.right
                                    rightMargin: 7
                                }
                            }
                        }

                        Rectangle {
                            color: palette.mid
                            visible: headerCell.index < logTable.columns - 1
                            width: 1

                            anchors {
                                bottom: parent.bottom
                                right: parent.right
                                top: parent.top
                            }
                        }

                        TapHandler {
                            id: headerTap

                            gesturePolicy: TapHandler.WithinBounds

                            onTapped: filterModel.cycleSort(headerCell.index)
                        }

                        HoverHandler {
                            id: headerHover
                        }
                    }
                }

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    TableView {
                        id: logTable

                        readonly property bool tailIsNewest: filterModel.sortedColumn === -1 || (filterModel.sortedColumn === Logger.TimeColumn && filterModel.sortedOrder === Qt.AscendingOrder)

                        // Explicit widths rather than a columnWidthProvider
                        function fitColumns(): void {
                            forceLayout();

                            let othersWidth = 0;

                            for (let column = 0; column < columns; ++column) {
                                if (column === Logger.MessageColumn) {
                                    continue;
                                }

                                let fittedWidth = Math.ceil(Math.max(implicitColumnWidth(column), logHeader.implicitColumnWidth(column)));

                                // long categories elide instead of squeezing the message column
                                if (column === Logger.CategoryColumn) {
                                    fittedWidth = Math.min(fittedWidth, Math.max(Math.ceil(logHeader.implicitColumnWidth(column)), Math.round(width * 0.2)));
                                }

                                setColumnWidth(column, fittedWidth);
                                othersWidth += Math.max(0, fittedWidth);
                            }

                            setColumnWidth(Logger.MessageColumn, Math.max(logHeader.implicitColumnWidth(Logger.MessageColumn), width - othersWidth));
                        }

                        function scrollToTail(): void {
                            if (rows > 0) {
                                positionViewAtRow(rows - 1, TableView.AlignBottom);
                            }
                        }

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ScrollBar.vertical: logTableScrollBar
                        animate: false
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        model: filterModel
                        pointerNavigationEnabled: false

                        delegate: Rectangle {
                            id: cell

                            required property int column
                            required property string display
                            required property int level
                            required property int row

                            ToolTip.text: display
                            ToolTip.visible: cellLabel.truncated && cellHover.hovered
                            color: row % 2 === 0 ? palette.base : palette.alternateBase
                            implicitHeight: cellLabel.implicitHeight
                            implicitWidth: cellLabel.implicitWidth

                            Label {
                                id: cellLabel

                                color: cell.column === Logger.LevelColumn ? root.levelColor(cell.level) : (cell.column === Logger.TimeColumn ? AppColors.semantic.neutral : palette.text)
                                elide: cell.column === Logger.CategoryColumn ? Text.ElideRight : Text.ElideNone
                                font.bold: cell.column === Logger.LevelColumn
                                padding: 5
                                text: cell.display
                                textFormat: Text.PlainText
                                verticalAlignment: Text.AlignTop
                                width: cell.width
                                wrapMode: cell.column === Logger.MessageColumn ? Text.Wrap : Text.NoWrap
                            }

                            HoverHandler {
                                id: cellHover
                            }

                            TapHandler {
                                acceptedButtons: Qt.RightButton
                                gesturePolicy: TapHandler.WithinBounds

                                onTapped: {
                                    rowMenu.lineText = filterModel.data(filterModel.index(cell.row, 0), Logger.LineTextRole);
                                    rowMenu.popup();
                                }
                            }
                        }

                        onWidthChanged: Qt.callLater(fitColumns)

                        Menu {
                            id: rowMenu

                            property string lineText

                            MenuItem {
                                text: qsTr("Copy line")

                                onTriggered: Utilities.copyToClipboard(rowMenu.lineText)
                            }
                        }
                    }

                    LayoutScrollBar {
                        id: logTableScrollBar

                        view: logTable
                    }
                }
            }

            Label {
                horizontalAlignment: Qt.AlignHCenter
                text: Logger.count > 0 ? qsTr("# Nothing matches the filter") : qsTr("# No log messages yet")
                textFormat: Text.MarkdownText
                visible: filterModel.count === 0
                x: Math.round((parent.width - width) / 2)
                y: Math.round((parent.height - height) / 2)
            }
        }
    }
}
