import ORB.Sources
import ORB.Style
import QtQuick

IconButton {
    readonly property bool favorited: Favorites.count >= 0 && Favorites.contains(station)
    required property station station

    ToolTip.text: favorited ? qsTr("Remove from favorites") : qsTr("Add to favorites")
    enabled: station.valid
    icon.name: favorited ? "heart-check" : "heart"

    onClicked: Favorites.toggle(station)
}
