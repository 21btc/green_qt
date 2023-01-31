import Blockstream.Green
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property Device device
    ColumnLayout {

        anchors.centerIn: parent

        RowLayout {
            Image {
                sourceSize.width: 64
                sourceSize.height: 64
                source: 'qrc:/svg/ledger.svg'
            }
            Label {
                text: `LEDGER NANO X - ${device.objectName}`
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: {
                if (!device || !device.properties.connected) return 'WAITING FOR DEVICE'
                if (!device.properties.app) return 'WAITING FOR APP'
                return 'OK!'
            }
        }
        ProgressBar {
            Layout.alignment: Qt.AlignHCenter
            indeterminate: true
            visible: !device || !device.properties.connected || !device.properties.app
        }
    }
}
