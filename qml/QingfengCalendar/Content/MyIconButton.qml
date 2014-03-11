import QtQuick 2.1

Rectangle {
    id: icon_button

    width: 20
    height: 20
    color: "transparent"

    property url icon_source: ""

    signal clicked()

    activeFocusOnTab: true

    Image {
        anchors.centerIn: parent
        source: icon_source
        opacity: 0.6
    }

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: parent.activeFocus ? "#47b" : "white"
        opacity: parent.activeFocus ? 0.3 : 0
        Behavior on opacity {NumberAnimation{ duration: 100 }}
    }

    MyMouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
