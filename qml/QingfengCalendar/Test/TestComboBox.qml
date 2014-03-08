import QtQuick 2.1
import QtQuick.Controls 1.1

Rectangle {
    width: 400
    height: 100

    ListModel {
        id: hour_model
//            Component.onCompleted: {
//                for (var i = 0; i < 24; ++i) {
//                    hour_model.append({ "hour": i});
//                }
//            }
        ListElement { text: "Banama" }
        ListElement { text: "Hanama" }
        ListElement { text: "Danama" }
        ListElement { text: "Canama" }
        ListElement { text: "Ganama" }
    }

    ComboBox {
        id: start_hour_combo
        anchors.centerIn: parent
        model: hour_model
//        acceptableInput: true
//        editable: true
        currentIndex: 1
//        validator: IntValidator { bottom: 0; top: 23; }
    }
}
