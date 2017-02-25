import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Dilay 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SculptView {
        Column {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            spacing: 20
            ToggleButton {
                width: 50
                height: 50

            }

            ToggleButton {
                width: 50
                height: 50

            }
        }
    }
}
