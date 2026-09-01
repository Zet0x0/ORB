pragma ComponentBehavior: Bound

import ORB.Settings
import ORB.Style
import QtQuick
import QtQuick.Layouts

Dialog {
    id: root

    closePolicy: Popup.CloseOnEscape
    modal: true
    title: qsTr("Preferences")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    footer: DialogButtonBox {
        Button {
            DialogButtonBox.buttonRole: DialogButtonBox.ApplyRole
            enabled: propertyModel.hasPendingChanges
            text: qsTr("Apply")

            onClicked: propertyModel.applyChanges()
        }

        Button {
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            text: qsTr("OK")

            onClicked: {
                propertyModel.applyChanges();
                root.close();
            }
        }

        Button {
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            text: qsTr("Cancel")

            onClicked: root.close()
        }
    }

    onClosed: propertyModel.discardChanges()

    SettingsPropertyModel {
        id: propertyModel

        groups: categoryList.currentItem ? (categoryList.currentItem as CategoryDelegate).groups : []
    }

    RowLayout {
        Frame {
            Layout.fillHeight: true
            Layout.preferredWidth: categoryList.maxItemWidth + leftPadding + rightPadding + (categoryScrollBar.visible ? categoryScrollBar.width + categoryRow.spacing : 0)

            RowLayout {
                id: categoryRow

                anchors.fill: parent

                ListView {
                    id: categoryList

                    readonly property real maxItemWidth: {
                        let max = 0;

                        for (const delegateItem of contentItem.children) {
                            max = Math.max(max, delegateItem.implicitWidth);
                        }

                        return max;
                    }

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ScrollBar.vertical: categoryScrollBar
                    activeFocusOnTab: true
                    boundsBehavior: Flickable.StopAtBounds
                    cacheBuffer: contentHeight
                    clip: true
                    currentIndex: 0
                    focus: true
                    highlightFollowsCurrentItem: false
                    keyNavigationEnabled: true
                    keyNavigationWraps: true

                    delegate: CategoryDelegate {}
                    highlight: Rectangle {
                        border.color: palette.highlight
                        color: "#00000000"
                        height: ListView.view.currentItem.height
                        visible: ListView.view.activeFocus
                        width: ListView.view.currentItem.width
                        y: ListView.view.currentItem.y
                        z: 1
                    }
                    model: SettingsCategoryModel {}
                }

                LayoutScrollBar {
                    id: categoryScrollBar

                    view: categoryList
                }
            }
        }

        Frame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: Math.round(root.parent.height * 0.7)
            Layout.minimumHeight: 300
            Layout.minimumWidth: 300
            Layout.preferredHeight: Math.min(Layout.maximumHeight, settingsColumn.implicitHeight + topPadding + bottomPadding)

            RowLayout {
                anchors.fill: parent

                Flickable {
                    id: settingsFlickable

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ScrollBar.vertical: settingsScrollBar
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    contentHeight: settingsColumn.implicitHeight
                    contentWidth: width

                    ColumnLayout {
                        id: settingsColumn

                        width: parent.width

                        Repeater {
                            model: propertyModel

                            delegate: ColumnLayout {
                                id: propertyDelegate

                                required property int index
                                required property string label
                                required property int max
                                required property int min
                                readonly property bool showSubcategoryHeader: subcategory !== "" && (index === 0 || propertyModel.data(propertyModel.index(index - 1, 0), SettingsPropertyModel.SubcategoryRole) !== subcategory)
                                required property string subcategory
                                required property string type
                                required property var value

                                Layout.fillWidth: true

                                Component {
                                    id: boolComponent

                                    CheckBox {
                                        checked: propertyDelegate.value
                                        text: propertyDelegate.label

                                        onToggled: propertyModel.setValue(propertyDelegate.index, checked)
                                    }
                                }

                                Component {
                                    id: intComponent

                                    SpinBox {
                                        editable: true
                                        from: propertyDelegate.min
                                        live: true
                                        to: propertyDelegate.max
                                        value: propertyDelegate.value

                                        onValueChanged: propertyModel.setValue(propertyDelegate.index, value)
                                    }
                                }

                                Component {
                                    id: stringComponent

                                    TextField {
                                        text: propertyDelegate.value

                                        onEditingFinished: propertyModel.setValue(propertyDelegate.index, text)
                                    }
                                }

                                Label {
                                    Layout.topMargin: propertyDelegate.index === 0 ? 0 : 8
                                    font.bold: true
                                    text: propertyDelegate.subcategory
                                    textFormat: Text.PlainText
                                    visible: propertyDelegate.showSubcategoryHeader
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: propertyDelegate.label
                                        textFormat: Text.PlainText
                                        visible: propertyDelegate.type !== "bool"
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        visible: propertyDelegate.type !== "bool"
                                    }

                                    Loader {
                                        Layout.fillWidth: propertyDelegate.type === "string"
                                        sourceComponent: {
                                            switch (propertyDelegate.type) {
                                            case "bool":
                                                return boolComponent;
                                            case "int":
                                                return intComponent;
                                            case "string":
                                                return stringComponent;
                                            default:
                                                return null;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                LayoutScrollBar {
                    id: settingsScrollBar

                    view: settingsFlickable
                }
            }
        }
    }

    component CategoryDelegate: ItemDelegate {
        required property var groups
        required property int index
        required property string name

        font.bold: highlighted
        highlighted: ListView.isCurrentItem
        text: name
        width: ListView.view.width

        onClicked: ListView.view.currentIndex = index
    }
}
