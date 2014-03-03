import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
//import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MyCalendar.Utils.Events 1.0
import "Content"

Item {
    id: float_event_edit
    width: 500
    height: 220

//    color: "white"

    property date event_date: new Date()
    property string event_title

    property int left_margin: 30
    property real font_size: 14

    property real shadow_radius: 8

    signal show(date event_start_date)
    signal hide()
    signal saveEventClicked(MyEvent my_event)

    MyEvent {
        id: my_event
    }

    onShow: {
        event_date = event_start_date;
        title_edit.text = "";
        float_event_edit.visible = true;
    }
    onHide: {
        float_event_edit.visible = false;
    }

    RectangularGlow {
        id: border_shadow
        anchors.fill: parent
        color: "darkgray"
        cornerRadius: 6
        glowRadius: shadow_radius
        spread: 0
    }

    Rectangle {
        id: edit_rectangle
//        anchors.fill: parent
        width: parent.width - shadow_radius
        height: parent.height - shadow_radius
        anchors.centerIn: parent
        color: Qt.lighter("lightgray", 1.15)

        MouseArea {
            id: mouse_area
            anchors.fill: parent
        }

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
            width: 20
            height: 20

            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 15

//            opacity: 0.6
            iconSource: "images/close.png"

            style: ButtonStyle {
                background: Item {
                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                }
                label: Item {
                    Image {
                        anchors.centerIn: parent
                        source: control.iconSource
                        anchors.verticalCenter: parent.verticalCenter
                        opacity: 0.6
                    }
                }
            }

            MyMouseArea {
                anchors.fill: parent
                onClicked: float_event_edit.hide()
            }
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
                id: title_tip
                text: qsTr("What: ")
                font.pointSize: font_size
            }
            TextField {
                id: title_edit
                width: 360
                text: event_title
                font.pointSize: font_size
                focus: true
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
            anchors.bottomMargin: 16

            onClicked: {
                my_event.startDateTime = event_date;
                my_event.displayLabel = title_edit.text;
                my_event.allDay = true;
                float_event_edit.saveEventClicked(my_event);
            }
        }

        MyTextLinkButton {
            id: edit_button
            text_color: Qt.lighter("blue", 1.3)
            label: qsTr("Edit >>")
            text_size: font_size

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16 + edit_button.height * 0.25
            anchors.left: create_button.right
            anchors.leftMargin: 12
        }
    }

    Component.onCompleted: title_edit.forceActiveFocus()
}
