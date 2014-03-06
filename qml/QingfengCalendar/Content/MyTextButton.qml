import QtQuick 2.0

Rectangle {
    id: text_button

    property color button_color: Qt.lighter("blue", 1.3)
    property color hovered_color: Qt.lighter(button_color, 1.2)
    property color clicked_color: Qt.darker("darkgray", 1.1)
    property color text_color: "white"

//    property int button_width: text_label.width * 1.2
//    property int button_height: text_label.height * 1.4

    property alias text_width: text_label.width

    property string text

    property real font_size: 12

    signal clicked()

    width: text_label.width * 1.2
    height: text_label.height * 1.4

    Text {
        id: text_label
        anchors.centerIn: parent
        text: text_button.text
        font.pointSize: font_size
//        font.bold: true
        color: text_color
    }

    MyMouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: parent.color = hovered_color;
        onExited: parent.color = button_color;

        onClicked: parent.clicked()
    }

    color: mouse_area.pressed ? clicked_color : button_color
}
