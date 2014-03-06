
import QtQuick 2.0

Rectangle {
    id: text_link_button

//    property color hovered_color: Qt.lighter(text_color, 1.2)
    property color clicked_color: Qt.darker("darkgray", 1.3)
    property color text_color: "blue"

    property int button_width: text_label.width
    property int button_height: text_label.height

    property string text

    property real font_size: 12
    property bool font_bold: false

    signal clicked

    width: button_width
    height: button_height
    color: "transparent"

    Text {
        id: text_label
        anchors.centerIn: parent
        text: text_link_button.text
        font.pointSize: font_size
        font.bold: font_bold
        color: text_color
    }

    MyMouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: text_label.color = Qt.lighter(text_color, 1.2)
        onExited: text_label.color = text_color
        onClicked: parent.onClicked()
    }
}
