
import QtQuick 2.0

Rectangle {
    id: text_button

    property color hovered_color: Qt.lighter(text_color, 1.2)
//    property color clicked_color: Qt.darker("darkgray", 1.3)
    property color text_color: "blue"

    property alias text_width: button_text.width
    property alias text_height: button_text.height
    property int button_width: button_text.width
    property int button_height: button_text.height

    property string label

    property real text_size: 14

    signal clicked

    width: button_width
    height: button_height
    color: "transparent"

    Text {
        id: button_text
        anchors.centerIn: parent
        text: label
        font.pointSize: font_size
        color: text_color
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: button_text.color = hovered_color
        onExited: button_text.color = text_color
    }
}
