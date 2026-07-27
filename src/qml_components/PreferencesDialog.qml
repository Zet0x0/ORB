pragma ComponentBehavior: Bound

import ORB.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: control

    closePolicy: Popup.CloseOnEscape
    dim: true
    focus: true
    modal: true
    title: qsTr("Preferences")
    x: Math.floor((parent.width - width) / 2)
    y: Math.floor((parent.height - height) / 2)
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
                    keyNavigationEnabled: true
                    keyNavigationWraps: true

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
                                        width: parent.width

                                        onValueModified: propertyModel.setValue(propertyDelegate.index, value)
                                    }
                                }

                                Component {
                                    id: stringComponent

                                    TextField {
                                        text: propertyDelegate.value
                                        width: parent.width

                                        onEditingFinished: propertyModel.setValue(propertyDelegate.index, text)
                                    }
                                }

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

                                    Loader {
                                        Layout.fillWidth: true
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
