import QtQuick 2.0

Rectangle {
    id: button

    property color button_color: Qt.lighter("blue", 1.3)
    property color hovered_color: Qt.lighter(button_color, 1.2)
    property color clicked_color: Qt.darker("darkgray", 1.1)
    property color text_color: "white"

    property alias text_width: button_text.width
    property alias text_height: button_text.height
    property int button_width: button_text.width + 6
    property int button_height: button_text.height * 1.5

    property string label

    property real text_size: 14

    signal clicked

    width: button_width
    height: button_height

    Text {
        id: button_text
        anchors.centerIn: parent
        text: label
        font.pointSize: font_size
//        font.bold: true
        color: text_color
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: button.color = hovered_color
//        onHoveredChanged:
        onExited: button.color = button_color

//        onPressAndHold: button_color = clicked_color

//        onClicked: {
//            button.color = clicked_color
//        }
    }

    color: mouse_area.pressed ? clicked_color : button_color
}
