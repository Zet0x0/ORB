import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: control

    dim: true
    modal: true
    standardButtons: Dialog.Ok
    title: qsTr("About %0").arg(Qt.application.name)
    x: Math.floor((parent.width - width) / 2)
    y: Math.floor((parent.height - height) / 2)
    z: 1

    FontMetrics {
        id: bodyFontMetrics

        font: bodyLabel.font
    }

    RowLayout {
        Image {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.margins: 8
            Layout.preferredWidth: height
            fillMode: Image.PreserveAspectFit
            source: "qrc:/icons/ORB.svg"
            sourceSize: Qt.size(width, height)
        }

        ColumnLayout {
            Label {
                text: qsTr("## %0 %1").arg(Qt.application.name).arg(Qt.application.version)
                textFormat: Text.MarkdownText
            }

            Label {
                id: bodyLabel

                Layout.preferredWidth: Math.min(bodyFontMetrics.averageCharacterWidth * 45, (control.parent?.width ?? 480) * 0.8)
                text: qsTr("**O**nline **R**adio **B**rowser & Player (ORB) is an open-source project for browsing through and playing online radio stations from the internet.")
                textFormat: Text.MarkdownText
                wrapMode: Label.Wrap
            }

            Label {
                text: qsTr("Copyright %0 The Creator of ORB").arg(new Date().getFullYear())
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 8
                flat: true
                text: qsTr("View on GitHub")

                onClicked: Qt.openUrlExternally("https://github.com/Zet0x0/ORB")

                icon {
                    color: palette.buttonText
                    name: "brand-github"
                }
            }
        }
    }
}
