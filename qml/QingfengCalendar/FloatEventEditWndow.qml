import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
//import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MyCalendar.Utils.Events 1.0
import "Content"

Window {
    id: float_event_edit
    flags: Qt.FramelessWindowHint
    width: 500
    height: 220

    property date event_date: new Date()
//    property string event_title

    property int left_margin: 30
    property real font_size: 14

    Rectangle {
        id: rectangle

        color: "white"
        anchors.fill: parent

    //    MouseArea {
    //        id: mouse_area
    //        anchors.fill: parent
    //    }

        Label {
            id: view_title
            text: qsTr("Event")
            font.bold: true
            font.pointSize: font_size

            anchors.left: parent.left
            anchors.leftMargin: left_margin
            anchors.top: parent.top
            anchors.topMargin: 15
        }

        Button {
            id: close_button
            width: 30
            height: 30

            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 15

            opacity: 0.6
            iconSource: "images/close.png"

            onClicked: float_event_edit.visible = false
        }

        Grid {
            columns: 2
            columnSpacing: 16
            rowSpacing: 10

            anchors.left: parent.left
            anchors.leftMargin: left_margin
            anchors.right: parent.right
            anchors.top: view_title.bottom
            anchors.topMargin: 20

            Label {
                id: date_tip_label
                text: qsTr("When: ")
                font.pointSize: font_size
            }
            Label {
                id: date_show_label
                text: event_date.toLocaleDateString(Qt.locale(), "MMMM dd  (ddd)")
                font.pointSize: font_size
            }

            Label {
                id: event_title_tip
                text: qsTr("What: ")
                font.pointSize: font_size
            }
            Rectangle {
                id: title_edit_rectangle
                border.color: Qt.darker("lightblue", 1.1)
                border.width: 2
                width: 360
                height: title_edit.height * 1.3
    //            focus: true
                TextInput {
                    id: title_edit
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: font_size
                    focus: true
    //                cursorVisible: cur
    //                activeFocus: true
                    KeyNavigation.tab: close_button
                    KeyNavigation.backtab: close_button
                    wrapMode: TextEdit.Wrap
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: title_edit.selectAll()
                }
            }
        }

        MyButton {
            id: create_button
            color: Qt.lighter("blue", 1.3)
            label: qsTr("Create")
            text_size: font_size

            width: text_width * 1.5
            anchors.left: parent.left
            anchors.leftMargin: left_margin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20

        }

        MyTextLinkButton {
            id: edit_button
            text_color: Qt.lighter("blue", 1.3)
            label: qsTr("Edit >>")
            text_size: font_size

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20 + edit_button.height * 0.25
            anchors.left: create_button.right
            anchors.leftMargin: 12
        }
    }
}
