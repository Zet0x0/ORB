pragma ComponentBehavior: Bound

import ORB.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: control

    closePolicy: Popup.CloseOnEscape
    dim: true
    modal: true
    standardButtons: Dialog.Close
    title: qsTr("Preferences")
    x: Math.floor((parent.width - width) / 2)
    y: Math.floor((parent.height - height) / 2)
    z: 1

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
                    boundsBehavior: Flickable.StopAtBounds
                    cacheBuffer: contentHeight
                    clip: true
                    currentIndex: 0

                    delegate: CategoryDelegate {}
                    model: SettingsCategoryModel {}
                }

                ScrollBar {
                    id: categoryScrollBar

                    Layout.fillHeight: true
                    minimumSize: 0.1
                    padding: 0
                    policy: visible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    visible: categoryList.contentHeight > categoryList.height

                    contentItem: Rectangle {
                        color: categoryScrollBar.pressed ? palette.midlight : palette.light
                        implicitHeight: 100
                        implicitWidth: 6
                        radius: Math.floor(width / 2)
                    }
                }
            }
        }

        Frame {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.maximumHeight: Math.floor(control.parent.height * 0.7)
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

                                Label {
                                    Layout.topMargin: propertyDelegate.index === 0 ? 0 : 8
                                    font.bold: true
                                    text: propertyDelegate.subcategory
                                    visible: propertyDelegate.showSubcategoryHeader
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        text: propertyDelegate.label
                                        visible: propertyDelegate.type !== "bool"
                                    }

                                    CheckBox {
                                        checked: propertyDelegate.value
                                        text: propertyDelegate.label
                                        visible: propertyDelegate.type === "bool"

                                        onToggled: propertyModel.setValue(propertyDelegate.index, checked)
                                    }

                                    SpinBox {
                                        Layout.fillWidth: true
                                        value: propertyDelegate.value
                                        visible: propertyDelegate.type === "int"

                                        onValueModified: propertyModel.setValue(propertyDelegate.index, value)
                                    }

                                    TextField {
                                        Layout.fillWidth: true
                                        text: propertyDelegate.value
                                        visible: propertyDelegate.type === "string"

                                        onEditingFinished: propertyModel.setValue(propertyDelegate.index, text)
                                    }
                                }
                            }
                        }
                    }
                }

                ScrollBar {
                    id: settingsScrollBar

                    Layout.fillHeight: true
                    minimumSize: 0.1
                    padding: 0
                    policy: visible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    visible: settingsFlickable.contentHeight > settingsFlickable.height

                    contentItem: Rectangle {
                        color: settingsScrollBar.pressed ? palette.midlight : palette.light
                        implicitHeight: 100
                        implicitWidth: 6
                        radius: Math.floor(width / 2)
                    }
                }
            }
        }
    }

    component CategoryDelegate: ItemDelegate {
        id: categoryDelegate

        required property var groups
        required property int index
        required property string name

        highlighted: ListView.isCurrentItem
        text: name
        width: ListView.view.width

        background: Rectangle {
            color: categoryDelegate.highlighted ? palette.active.highlight : (categoryDelegate.hovered ? palette.button : "transparent")
        }

        onClicked: ListView.view.currentIndex = index
    }
}
