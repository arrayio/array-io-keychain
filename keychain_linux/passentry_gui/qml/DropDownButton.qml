import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

ComboBox {
    id: comboBox
    height: 50
    width: 200
    displayText: "Sign with " + currentText
    model: ["wallet1.dat", "wallet2.dat"]

    indicator: Canvas {
        id: canvas
        x: comboBox.width - width - comboBox.rightPadding
        y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
        width: 15
        height: 10
        contextType: "2d"

        Connections {
            target: comboBox
            onPressedChanged: canvas.requestPaint()
        }

        onPaint: {
            context.reset();
            context.moveTo(width / 2, 0);
            context.lineTo(width, height);
            context.lineTo(0, height);
            context.closePath();
            context.fillStyle = "white";
            context.fill();
        }
    }

    background: Rectangle {
        id: background
        implicitWidth: 200
        implicitHeight: 50
        color: "#87cefa"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            background.color = 'red'
        }
    }

    MouseArea {
        anchors.fill: canvas
        onClicked: {
            parent.popup.visible = parent.popup.visible ? false : true
        }
    }
}
