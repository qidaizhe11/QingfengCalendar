import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0
//import org.kde.pim.calendar 1.0

Rectangle {
    width: 640
    height: 480

    GridLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 0
        columns: 7
        flow: GridLayout.LeftToRight

//        Repeater {
//            model: calendar.model
//            Loader {
//                Layout.fillWidth: true
//                Layout.fillHeight: true
//                Layout.minimumWidth: 60
//                Layout.minimumHeight: 60
//                Layout.rowSpan: 3

//                property int value: index
//                sourceComponent: dateDelegate
//            }
//        }

        Repeater {
            model: 7 * 6
            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
                property int value: index
                sourceComponent: dateDelegate
            }
        }
    }

    Rectangle {
        z: 1
        width: parent.width / 7 * 3
        height: parent.height / 18
        x: parent.width / 7 * 2
        y: parent.height / 18 * 7
        color: "lightblue"
        Text {
            text: "I am here, always."
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Component {
        id: dateDelegate
        Rectangle {
            color: "darkgray"
            Text {
                text: parent.parent.value
                color: "white"
                font.pixelSize: 16
                anchors {
                    left: parent.left; top: parent.top
                    leftMargin: 6; topMargin: 6
                }
            }
        }
    }

//    Calendar {
//        id: calendar
//        weeks: 5
//    }
}
