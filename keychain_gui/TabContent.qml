import QtQuick 2.11
import QtWebEngine 1.7
import QtQuick.Controls 1.4

Rectangle {
    TextArea {
        property string htmlGravatar: null
        property string htmlQr: null
        anchors.fill: parent
        anchors.bottomMargin: 50
        anchors.rightMargin: rightColumn.width

        onEditingFinished: {
            htmlGravatar = '<script src="https://cdn.jsdelivr.net/npm/jdenticon@2.1.0" async></script><canvas width="80" height="80" data-jdenticon-value=' + text + '></canvas>'
            gravatar.loadHtml(htmlGravatar);

            htmlQr = '<img src="https://api.qrserver.com/v1/create-qr-code/?data=' + text + '&amp;size=80x80" alt="" title="" />'
            qr.loadHtml(htmlQr);
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
