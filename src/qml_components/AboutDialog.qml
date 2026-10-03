import ORB.Common
import ORB.Player
import ORB.Style
import QtQuick
import QtQuick.Layouts

Dialog {
    id: root

    readonly property string mpvVersion: Player.mpvVersion() || qsTr("(unable to get version)")

    modal: true
    standardButtons: Dialog.Close
    title: qsTr("About ORB")
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    footer: DialogButtonBox {
        Button {
            DialogButtonBox.buttonRole: DialogButtonBox.ActionRole
            text: qsTr("Copy to Clipboard")

            //: Text with versions copied to the clipboard. %0, %1 and %2 are the versions of ORB, Qt and mpv
            onClicked: Utilities.copyToClipboard(qsTr("ORB %0\nQt %1\nmpv %2").arg(Qt.application.version).arg(Utilities.qtVersion()).arg(root.mpvVersion))
        }
    }

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
                //: Markdown with links. %0 and %1 are the versions of Qt and mpv
                text: qsTr("Powered by:\n\n\u2022 [Qt](https://www.qt.io) %0\n\n\u2022 [mpv](https://mpv.io) %1").arg(Utilities.qtVersion()).arg(root.mpvVersion)
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
