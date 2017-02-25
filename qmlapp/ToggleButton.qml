import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: container
    property bool isDown: false
    property var downIcon
    property var upIcon

    width: 50
    height: 50
    color: isDown ? "#30000000" :  "#30FFFFFF"
    border.color: isDown ? "#80ff80" : "#FF8080"
    border.width: 2
    radius: 5

    Item {
        anchors.fill: parent
        anchors.margins: 10

        Image {
          visible: parent.isDown
          anchors.fill: parent
          source: container.downIcon
        }
        Image {
          visible: !parent.isDown
          anchors.fill: parent
          source: container.upIcon
        }
    }

    MouseArea {
        property bool isClick: true
        Timer {
            id: clickTimer
            interval: 150
            repeat: false
            onTriggered: {
                clickTimer.start();
                parent.isClick = false;
            }
        }

        anchors.fill: parent

        onPressed: {
            isClick = true;
            parent.isDown = !parent.isDown;
            clickTimer.start();
        }

        onReleased: {
            clickTimer.stop()
            if (!isClick) {
                parent.isDown = !parent.isDown
            }
        }
    }
}
