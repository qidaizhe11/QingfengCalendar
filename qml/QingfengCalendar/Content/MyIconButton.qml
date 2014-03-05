import QtQuick 2.1

Rectangle {
    id: icon_button

    width: 20
    height: 20
    color: "transparent"

    property url icon_source: ""

    signal clicked

    Image {
        anchors.centerIn: parent
        source: icon_source
        opacity: 0.6
    }

    MyMouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}

//Button {
//    id: icon_button

//    width: 20
//    height: 20

////            opacity: 0.6

//    style: ButtonStyle {
//        background: Item {
//            Rectangle {
//                anchors.fill: parent
//                color: "transparent"
//            }
//        }
//        label: Item {
//            Image {
//                anchors.centerIn: parent
//                source: control.iconSource
//                anchors.verticalCenter: parent.verticalCenter
//                opacity: 0.6
//            }
//        }
//    }

//    MyMouseArea {
//        anchors.fill: parent
////        clicked: clicked();
//    }
//}
