import Blockstream.Green 0.1
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ListView {
    id: account_list_view
    model: wallet.accounts
    property Account currentAccount: currentItem ? currentItem.account : null
    signal clicked(Account account)
    spacing: 8
    delegate: ItemDelegate {
        id: delegate
        focusPolicy: Qt.ClickFocus

        property Account account: modelData

        onClicked: {
            account_list_view.currentIndex = index
            account_list_view.clicked(account)
        }
        background: Rectangle {
            color: delegate.highlighted ? constants.c500 : constants.c700
            radius: 8
        }

        highlighted: currentIndex === index
        leftPadding: 16
        rightPadding: 16
        topPadding: 16
        bottomPadding: 16

        width: ListView.view.width-16

        contentItem: ColumnLayout {
            spacing: 8
            Label {
                text: qsTrId('id_amp_account')
                visible: account.json.type === '2of2_no_recovery'
                font.pixelSize: 12
                font.capitalization: Font.AllUppercase
                leftPadding: 8
                rightPadding: 8
                topPadding: 4
                bottomPadding: 4
                opacity: 1
                color: 'white'
                background: Rectangle {
                    color: '#080b0e'
                    opacity: 0.2
                    radius: 4
                }
            }
            Label {
                text: qsTrId('id_2of3_account')
                visible: account.json.type === '2of3'
                font.pixelSize: 12
                font.capitalization: Font.AllUppercase
                leftPadding: 8
                rightPadding: 8
                topPadding: 4
                bottomPadding: 4
                color: 'white'
                background: Rectangle {
                    color: '#080b0e'
                    opacity: 0.2
                    radius: 4
                }
            }
            EditableLabel {
                id: name_field
                Layout.fillWidth: true
                font.styleName: 'Medium'
                font.pixelSize: 18
                leftInset: -8
                rightInset: -8
                text: accountName(account)
                enabled: delegate.ListView.isCurrentItem && account.name !== '' && !delegate.account.wallet.locked
                onEdited: {
                    account.rename(text, activeFocus)
                }

//                onTextChanged: {
//                    account.rename(text, activeFocus)
//                    if (!activeFocus) cursorPosition = 0
//                }
//                onActiveFocusChanged: {
//                    if (activeFocus) {
//                        account_list_view.currentIndex = index
//                        account_list_view.clicked(account)
//                        name_field.forceActiveFocus(Qt.MouseFocusReason)
//                    }
//                    account.rename(text, activeFocus)
//                    if (!activeFocus) cursorPosition = 0
//                }
            }
            RowLayout {
                spacing: 10
                Label {
                    text: formatAmount(account.balance)
                    font.pixelSize: 14
                    font.styleName: 'Regular'
                }
                Label {
                    font.pixelSize: 14
                    text: '≈ ' + formatFiat(account.balance)
                    font.styleName: 'Regular'
                }
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator { }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.forceActiveFocus(Qt.MouseFocusReason)
        z: -1
    }
}
