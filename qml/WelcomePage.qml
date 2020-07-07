import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Column {
    property string title: qsTrId('id_welcome_to') + ' ' + qsTrId('Blockstream Green')
    property list<Action> actions: [
        Action {
            text: qsTrId('id_continue')
            enabled: agreeWithTermsOfService
            onTriggered: next()
        }
    ]
    property bool agreeWithTermsOfService: checkbox.checked
    signal next()

    Image {
        fillMode: Image.PreserveAspectFit
        source: 'qrc:/svg/onboarding_illustration.svg'
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -checkbox.width * (1 - checkbox.opacity)
        CheckBox {
            id: checkbox
            focus: true
            anchors.verticalCenter: parent.verticalCenter
        }
        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: qsTrId('id_i_agree_to_the') + ' ' + '<a href="https://blockstream.com/green/terms/">' + qsTrId('id_terms_of_service') + '</a>'
            onLinkActivated: Qt.openUrlExternally(link)
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }
    }
}
