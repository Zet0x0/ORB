import ORB.Player
import ORB.Sources
import ORB.Style
import QtQuick
import QtQuick.Layouts

FocusScope {
    id: root

    required property int index
    readonly property bool isCurrentStation: Player.station.streamUrl === station.streamUrl
    property bool keyPressed: false
    readonly property bool navFocused: mainArea.activeFocus && mainArea.focusReason !== Qt.MouseFocusReason
    required property station station

    function handleInteraction(): void {
        if (!isCurrentStation) {
            Player.setStation(station, true);
        } else if (Player.state === Player.Stopped) {
            Player.play();
        } else {
            Player.stop();
        }
    }

    implicitHeight: body.implicitHeight
    implicitWidth: body.implicitWidth

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Down) {
            root.ListView.view.incrementCurrentIndex();
            event.accepted = true;
        } else if (event.key === Qt.Key_Up) {
            root.ListView.view.decrementCurrentIndex();
            event.accepted = true;
        }
    }

    Control {
        id: body

        anchors.fill: parent
        padding: 8

        background: Rectangle {
            color: palette.base.darker(tapHandler.pressed || root.keyPressed ? 1.2 : (rowHover.hovered || root.navFocused || favoriteButton.visualFocus ? 0.8 : 1.0))

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                color: palette.accent
                visible: root.isCurrentStation
                width: 3
            }
        }
        contentItem: RowLayout {
            Control {
                id: mainArea

                Layout.fillHeight: true
                Layout.fillWidth: true
                focus: true
                focusPolicy: root.ListView.isCurrentItem || activeFocus ? Qt.TabFocus : Qt.NoFocus

                contentItem: RowLayout {
                    StationImage {
                        Layout.fillHeight: false
                        Layout.fillWidth: false
                        Layout.preferredHeight: 48
                        Layout.preferredWidth: 48
                        imageUrl: root.station.imageUrl
                    }

                    Label {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.maximumHeight: 48
                        ToolTip.text: root.station.name
                        ToolTip.visible: truncated && (mainArea.hovered || mainArea.activeFocus)
                        elide: Text.ElideRight
                        font.bold: root.isCurrentStation
                        text: root.station.name
                        textFormat: Text.PlainText
                        verticalAlignment: Qt.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }

                Keys.onPressed: event => {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
                        root.keyPressed = true;

                        root.handleInteraction();

                        event.accepted = true;
                    }
                }
                Keys.onReleased: event => {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
                        root.keyPressed = false;

                        event.accepted = true;
                    }
                }
                onActiveFocusChanged: {
                    if (!activeFocus) {
                        root.keyPressed = false;
                    }
                }
            }

            FavoriteButton {
                id: favoriteButton

                Layout.alignment: Qt.AlignVCenter
                flat: true
                focusPolicy: root.ListView.isCurrentItem || activeFocus ? Qt.StrongFocus : Qt.NoFocus
                opacity: favorited || hovered || rowHover.hovered || visualFocus || root.navFocused ? 1 : 0
                station: root.station
            }
        }
    }

    Rectangle {
        border.color: body.palette.highlight
        color: "#00000000"
        height: root.height
        visible: root.navFocused
        width: body.leftPadding + mainArea.width
    }

    HoverHandler {
        id: rowHover

        cursorShape: Qt.PointingHandCursor
    }

    TapHandler {
        id: tapHandler

        onTapped: {
            root.ListView.view.currentIndex = root.index;
            mainArea.forceActiveFocus(Qt.MouseFocusReason);
            mainArea.focusReason = Qt.MouseFocusReason;
            root.handleInteraction();
        }
    }
}
