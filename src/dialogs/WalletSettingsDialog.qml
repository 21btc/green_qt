import Blockstream.Green 0.1
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12
import ".."

Dialog {
    anchors.centerIn: Overlay.overlay
    modal: true
    parent: Overlay.overlay
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: qsTr('RENAME ACCOUNT')

    ScrollView {
        id: scroll_view
        anchors.fill: parent
        clip: true

        Column {
            padding: 8
            spacing: 16
            width: parent.width - 16

            Panel {
                title: qsTr('NETWORK')
                icon: '../assets/svg/network.svg'
                width: scroll_view.width - 16
            }

            WalletSecuritySettingsView {
                width: scroll_view.width - 16
            }

            Panel {
                width: scroll_view.width - 16
                title : qsTr('ADVANCED')
                icon: '../assets/svg/advanced.svg'
            }
        }
    }
}
