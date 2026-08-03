pragma ComponentBehavior: Bound

import ORB.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: control

    closePolicy: Popup.CloseOnEscape
    modal: true
    title: qsTr("Preferences")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    z: 1

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
                control.close();
            }
        }

        Button {
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            text: qsTr("Cancel")

            onClicked: control.close()
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
                        z: 2
                    }
                    model: SettingsCategoryModel {}
                }

                ScrollBar {
                    id: categoryScrollBar

                    Layout.fillHeight: true
                    policy: categoryList.contentHeight > categoryList.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                }
            }
        }

        Frame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: Math.round(control.parent.height * 0.7)
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
                                        value: propertyDelegate.value

                                        onValueModified: propertyModel.setValue(propertyDelegate.index, value)
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

                ScrollBar {
                    id: settingsScrollBar

                    Layout.fillHeight: true
                    policy: settingsFlickable.contentHeight > settingsFlickable.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
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
