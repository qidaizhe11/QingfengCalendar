import QtQuick 2.1

Rectangle {
    id: text_button

    property color button_color: Qt.lighter("blue", 1.3)
    property color hovered_color: Qt.lighter(button_color, 1.2)
    property color clicked_color: Qt.darker("darkgray", 1.5)
    property color text_color: "white"

//    property int button_width: text_label.width * 1.2
//    property int button_height: text_label.height * 1.4

    property alias text_width: text_label.width
    property alias text_height: text_label.height

    property string text

    property real font_size: 12

    signal clicked()

    activeFocusOnTab: true

//    border.color: activeFocus ? "#47b" : button_color
//    border.width: activeFocus ? 1: 0

    width: text_label.width * 1.2
    height: text_label.height * 1.4

    color: button_color

    Text {
        id: text_label
        anchors.centerIn: parent
        text: text_button.text
        font.pointSize: font_size
//        font.bold: true
        color: text_color
    }

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: parent.activeFocus ? "#47b" : "white"
        opacity: parent.activeFocus ? 0.6 : 0
        Behavior on opacity {NumberAnimation{ duration: 100 }}
    }

    MyMouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: parent.color = hovered_color;
        onExited: parent.color = button_color;

        onPressed: parent.color = clicked_color
        onReleased: parent.color = button_color

        onClicked: parent.clicked()
    }
}
