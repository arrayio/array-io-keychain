import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import Controller 1.0

Window {
    visible: true
    width: 640
    height: 480
    minimumWidth: 350
    minimumHeight: 350
    title: qsTr("Keychain")

    Controller {
        id: controller

        onClose: {
            Qt.quit()
        }

        onPasswordChanged: {
            password.text = controller.password()
        }

        onMRawTxChanged: {
            rawTxContent.rawTx = controller.rawTx()
        }
    }

    Item {
        id: itemPassword
        anchors.top: parent.top
        height: parent.height / 7
        width: parent.width

        Column {
            anchors.fill: parent
            anchors.leftMargin: 10
            Label {
                id: labelPassword
                text: qsTr("Password")
            }

            TextField {
                id: password
                width: parent.width - parent.width / 4
                height: 30
                echoMode: TextInput.Password
                background: Rectangle {
                    border.width: 1
                }
            }
        }
    }

    Item {
        id: itemRawTx
        anchors.top: itemPassword.bottom
        anchors.bottom: footer.top
        width: parent.width

        Column {
            anchors.fill: parent
            anchors.leftMargin: 10
            Label {
                id: labelRawTx
                text: qsTr("RawTx")
            }

            RawTxContent {
                id: rawTxContent
                width: parent.width
                height: parent.height - labelRawTx.height
            }
        }
    }

    Item {
        id: footer
        anchors.bottom: parent.bottom
        height: parent.height / 5
        width: parent.width

        Button {
            id: buttonCancel
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: buttonOk.left
            anchors.rightMargin: 20
            height: 50
            text: qsTr("Cancel")

            onClicked: {
                controller.cancel()
                Qt.quit()
            }
        }

        Button {
            id: buttonOk
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            height: 50
            text: qsTr("Ok")

            onClicked: {
                controller.ok()
            }
        }
    }
}
