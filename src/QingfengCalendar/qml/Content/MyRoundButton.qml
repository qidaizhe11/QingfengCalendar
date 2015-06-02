import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Rectangle {
    id: round_button

    implicitWidth: 80
    implicitHeight: 80

    property color button_color: "indigo"
    property color hoverd_color: Qt.lighter(button_color, 2.8)
    property color clicked_color: Qt.darker(button_color, 1.2)

    property alias icon_source: image.source

    border.color: activeFocus ? "#47b" : button_color
    border.width: activeFocus ? 2: 3

    radius: width

    color: "transparent"

    signal clicked()

    activeFocusOnTab: true

    Image {
        id: image

        width: parent.width * 0.7
        height: parent.height * 0.7
        anchors.centerIn: parent

        smooth: true
        fillMode: Image.PreserveAspectFit
        opacity: 0.8
    }

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: parent.activeFocus ? button_color : "white"
        opacity: parent.activeFocus ? 0.1 : 0
        Behavior on opacity {NumberAnimation{ duration: 100 }}
    }

    MyMouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        onEntered: parent.color = hoverd_color
        onExited: parent.color = "transparent"

        onPressed: parent.color = clicked_color
        onReleased: parent.color = "transparent"

        onClicked: parent.clicked()
    }
}
