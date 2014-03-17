import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    id: my_text_area

    property alias text: text_area.text
//    property alias font_size: text_area.font.pointSize
    property alias placeholder_text: placeholder.text

//    activeFocusOnTab: true

    MouseArea {
        visible: (!my_text_area.activeFocus)
        anchors.fill: parent
        onClicked: text_area.forceActiveFocus()
    }

    Text {
        id: placeholder

        anchors.left: text_area.left
        anchors.top: text_area.top
        anchors.leftMargin: 4
        anchors.topMargin: 4
        horizontalAlignment: text_area.horizontalAlignment
        verticalAlignment: text_area.horizontalAlignment

//        text: qsTr("Add a description")
        color: "darkgrey"
        opacity: !text_area.text.length ? 1 : 0
        renderType: Text.NativeRendering
        font.pointSize: font_size

        Behavior on opacity { NumberAnimation { duration: 250; } }
    }

    TextArea {
        id: text_area
        anchors.fill: parent

        backgroundVisible: false
        frameVisible: false

        activeFocusOnTab: true

        // TODO: seems it didn't take effect, didn't find out why yet.
        tabChangesFocus: true

//        KeyNavigation.tab: text_area.Keynavigation.tab
//        KeyNavigation.backtab: text_area.Keynavigation.backtab
    }
}
