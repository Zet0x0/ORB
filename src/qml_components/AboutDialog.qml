import ORB.Common
import ORB.Player
import ORB.Style
import QtQuick
import QtQuick.Layouts

Dialog {
    id: root

    modal: true
    standardButtons: Dialog.Ok
    title: qsTr("About ORB")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    FontMetrics {
        id: bodyFontMetrics

        font: bodyLabel.font
    }

    RowLayout {
        Image {
            Layout.alignment: Qt.AlignTop
            Layout.margins: 8
            Layout.preferredHeight: bodyFontMetrics.height * 6
            Layout.preferredWidth: height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/icons/ORB.svg"
            sourceSize: Qt.size(width, height)
        }

        ColumnLayout {
            Label {
                text: qsTr("## ORB %0").arg(Qt.application.version)
                textFormat: Text.MarkdownText
            }

            Label {
                id: bodyLabel

                Layout.preferredWidth: Math.min(bodyFontMetrics.averageCharacterWidth * 45, (root.parent?.width ?? 480) * 0.8)
                text: qsTr("**O**nline **R**adio **B**rowser & Player (ORB) is an open-source project for browsing through and playing online radio stations from the internet.")
                textFormat: Text.MarkdownText
                wrapMode: Label.Wrap
            }

            Label {
                id: librariesLabel

                // not a regular Markdown list because by default Qt gets us a wideass
                // indent of probably around 40px, that is ugly, and we can't even change it
                //: Markdown with links. %0, %1 and %2 are the versions of Qt, mpv, and MpvQt
                text: qsTr("Powered by:\n\n\u2022 [Qt](https://www.qt.io) %0\n\n\u2022 [mpv](https://mpv.io) %1\n\n\u2022 [MpvQt](https://invent.kde.org/libraries/mpvqt) %2").arg(Utilities.qtVersion()).arg(Player.mpvVersion() || qsTr("(unable to get version)")).arg(Player.mpvQtVersion())
                textFormat: Text.MarkdownText

                onLinkActivated: link => Utilities.openUrlExternally(link)

                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                    enabled: librariesLabel.hoveredLink
                }
            }

            Label {
                text: qsTr("Copyright %0 The Creator of ORB").arg(new Date().getFullYear())
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 8
                flat: true
                icon.name: "brand-github"
                text: qsTr("View on GitHub")

                onClicked: Utilities.openUrlExternally("https://github.com/Zet0x0/ORB")
            }
        }
    }
}
