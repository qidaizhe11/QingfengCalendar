import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    width: 300
    height: 100

    Button {
        id: menu_button
        anchors.centerIn: parent
//        anchors.fill: parent

        text: "Menu button"

        menu: Menu {
            MenuItem { text: "This Button" }
            MenuItem { text: "Hello World" }
            MenuItem { text: "I don't know" }
        }
    }
}
