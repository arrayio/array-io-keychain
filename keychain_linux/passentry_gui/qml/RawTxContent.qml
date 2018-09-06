import QtQuick 2.11
import QtWebEngine 1.7
import QtQuick.Controls 2.2

Item {
    property alias rawTx: textArea.text

    Rectangle {
        anchors.fill: parent
        anchors.rightMargin: rightColumn.width
        border.width: 1
        TextArea {
            id: textArea
            property string htmlGravatar: null
            property string htmlQr: null
            readOnly: true
            font.pointSize: 14

            onTextChanged: {
                if(text == "")
                    return

                htmlGravatar = '<script src="https://cdn.jsdelivr.net/npm/jdenticon@2.1.0" async></script><canvas width="80" height="80" data-jdenticon-value=' + text + '></canvas>'
                gravatar.loadHtml(htmlGravatar);

                htmlQr = '<img src="https://api.qrserver.com/v1/create-qr-code/?data=' + text + '&amp;size=80x80" alt="" title="" />'
                qr.loadHtml(htmlQr);
            }
        }
    }

    Column {
        id: rightColumn
        anchors.right: parent.right
        width: 100

        WebEngineView {
            id: gravatar
            width: parent.width
            height: 100
        }

        WebEngineView {
            id: qr
            width: parent.width
            height: 100
        }
    }
}
