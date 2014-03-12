import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MyCalendar2.Utils.Events 1.0
import "Content"

Window {
    id: float_event_edit
    flags: Qt.FramelessWindowHint
    width: 500
    height: 220
    visible: true

    property real pos_x
    property real pos_y

    property bool is_empty_event: true
    property date event_date
    property var event_item

    property real font_size: 12

    property real shadow_radius: 8
    property color base_color: Qt.lighter("lightgray", 1.15)

//    signal showAdd(date event_start_date)
//    signal showEdit(var event)
    signal close()
    signal saveEventClicked(var my_event)

//    onShowEmpty: {
//        event_date = event_start_date;
//        loader.sourceComponent = event_add;
//        //        edit_loader.item.title_text = "";
//        float_event_edit.visible = true;
//    }
//    onShowEdit: {
//        event_item = event;
//        loader.sourceComponent = event_edit;
//        float_event_edit.visible = true;
//    }

    onClose: {
        float_event_edit.visible = false;
        panelItem.hoveredCellIndex = -1;
    }

    Connections {
        target: navigationbar_mouseArea
        onClicked: {
            float_event_edit.close();
//            if (float_event_edit.visible) {
//                float_event_edit.hide();
//            }
        }
    }

    Connections {
        target: background_mouseArea
        onClicked: {
            float_event_edit.close();
//            if (float_event_edit.visible) {
//                float_event_edit.hide();
//            }
        }
    }

    RectangularGlow {
        id: border_shadow
        anchors.fill: parent
        color: "darkgray"
        cornerRadius: 6
        glowRadius: shadow_radius
        spread: 0
    }

//    MouseArea {
//        id: mouse_area
//        anchors.fill: parent
//    }

    Item {
        id: panel_item
        width: parent.width - shadow_radius
        height: parent.height - shadow_radius
        anchors.centerIn: parent

        Loader {
            id: loader
            anchors.fill: parent
            sourceComponent: is_empty_event ? event_add : event_edit
        }
    }

    property Component event_edit: Rectangle {
        color: base_color

        property real margin: 20

        Rectangle {
            id: edit_content
            width: parent.width * 0.8
            height: parent.height * 0.8
            color: "transparent"
            anchors.centerIn: parent

            Label {
                id: edit_title
                anchors.left: parent.left
                width: parent.width * 0.9
                elide: Text.ElideRight
                text: event_item ? event_item.displayLabel : ""
                font.pointSize: font_size + 2
                font.bold: true
                color: Qt.darker("blue", 1.3)
            }

            Label {
                id: edit_date_label

                anchors.left: parent.left
                anchors.top: edit_title.bottom
                anchors.topMargin: margin

                text: event_item ? event_item.startDateTime.toLocaleDateString() : ""
                font.pointSize: font_size
            }

            Rectangle {
                id: line

                anchors.bottom: edit_delete_button.top
                anchors.bottomMargin: margin
                width: parent.width
                height: 1

                color: "lightgray"
            }

            MyTextLinkButton {
                id: edit_delete_button

                anchors.left: parent.left
                anchors.bottom: parent.bottom

                text_color: Qt.lighter("blue", 1.3)
                text: qsTr("Delete")
                font_size: font_size
            }

            MyTextLinkButton {
                id: edit_detail_button
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom

                text_color: Qt.lighter("blue", 1.3)
                text: qsTr("Edit >>")
                font_size: font_size
                font_bold: true
            }
        }

        MyIconButton {
            id: edit_close_button
            width: 20
            height: 20

            anchors.left: edit_content.right
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1

            icon_source: "images/close.png"

            onClicked: float_event_edit.hide()
        }
    }

    property Component event_add: Rectangle {
        color: base_color

        //        property string event_title
        property alias title_text: title_edit.text

        Rectangle {
            id: add_content
            width: parent.width * 0.8
            height: parent.height * 0.9
            color: "transparent"
            anchors.centerIn: parent

            ColumnLayout {
                spacing: 10
                anchors.fill: parent

                Label {
                    id: title_label
                    text: qsTr("Event")
                    font.bold: true
                    font.pointSize: font_size + 2
                }

                GridLayout {
                    columns: 2
                    rowSpacing: 6
                    columnSpacing: 12

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
                        text: ""
                        Layout.fillWidth: true
                        font.pointSize: font_size
                        focus: true
                    }
                }

                RowLayout {
                    spacing: 15

                    MyTextButton {
                        id: create_button
                        color: Qt.lighter("blue", 1.3)
                        text: qsTr("Create")
                        font_size: font_size

                        width: text_width * 1.5

                        onClicked: createEmptyAllDayEvent();
                    }

                    MyTextLinkButton {
                        id: edit_button
                        text_color: Qt.lighter("blue", 1.3)
                        text: qsTr("Edit >>")
                        font_size: font_size
                    }
                }
            }
        }

        MyIconButton {
            id: close_button
            width: 20
            height: 20

            anchors.left: add_content.right
            anchors.top: parent.top
            anchors.topMargin: 15

            icon_source: "images/close.png"

            onClicked: float_event_edit.hide()
        }

        function createEmptyAllDayEvent() {
            var new_event = Qt.createQmlObject(
                        "import QtQuick 2.1; import MyCalendar2.Utils.Events 1.0; MyEvent {}", float_event_edit);
            console.log("New Event: " + new_event);

            new_event.startDateTime = event_date;
            if (new_event.startDateTime) {
                new_event.displayLabel = title_edit.text;
                var dt_end = new Date(event_date.getFullYear(),
                                      event_date.getMonth(),
                                      event_date.getDate(),
                                      23, 59, 59);
                //                    dt_end.setDate(event_date.getDate() + 1);
                //                    dt_end.setTime(event_date.getTime() + 24*60*60*1000);
                console.log("javascript dt_start: ", event_date);
                console.log("Javascript dt_end: ", dt_end);
                new_event.endDateTime = dt_end;
                //                    new_event.endDateTime = new_event.startDateTime.addDays(1);
                new_event.allDay = true;
                console.log("Start Datetime: " + new_event.startDateTime);
                console.log("End date time: ", new_event.endDateTime);
                console.log("Display Label: " + new_event.displayLabel);
                console.log("All day: " + new_event.allDay);
                control.event_model.saveEvent(new_event);
                float_event_edit.hide();
                control.refreshEvents();
            }

            new_event.destroy();
        }

        Component.onCompleted: title_edit.forceActiveFocus()
    }
}
