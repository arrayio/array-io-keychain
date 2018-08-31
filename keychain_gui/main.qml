import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Window {
    visible: true
    width: 640
    height: 480
    minimumWidth: 350
    minimumHeight: 350
    title: qsTr("Keychain")

    TabView {
        id: tabView
        anchors.top: parent.top
        anchors.bottom: footer.top
        width: parent.width

        style: TabViewStyle {
            tabsAlignment: Qt.AlignHCenter
            frameOverlap: 0
            tab: Rectangle {
                id: tab
                color: "#007fff"
                implicitWidth: tabView.width / 3
                implicitHeight: 50

                Rectangle {
                    id: selectedLine
                    anchors.bottom: tab.bottom
                    width: tab.width
                    height: 3
                    color: "#00008b"
                    visible: styleData.selected
                }

                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: styleData.selected ? "#00008b" : "white"
                }
            }
            frame: Rectangle {
                color: "white"
            }
        }


        Tab {
            title: qsTr("Transaction")
            TabContent {

            }
        }
        Tab {
            title: qsTr("Raw Tx")
            TabContent {

            }
        }
        Tab {
            title: qsTr("Signed Raw Tx")
            TabContent {

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
            anchors.right: buttonWallets.left
            anchors.rightMargin: 20
            height: 50
            text: qsTr("Cancel")

            onClicked: {
                Qt.quit()
            }
        }

        DropDownButton {
            id: buttonWallets
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
        }
    }
}
