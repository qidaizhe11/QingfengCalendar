import QtQuick 2.2
import QtQuick.Controls 1.0
//import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MyCalendar.Utils.Events 1.0
import "Content"

Item {
    id: float_event_edit
    width: 500
    height: 220

    z: 1

    property date event_date
    property var event_item

    property real font_size: 12

    property real shadow_radius: 8
    property color base_color: Qt.lighter("lightgray", 1.15)

    signal showAdd(date event_start_date)
    signal showEdit(var event)
    signal hide()
    signal saveEventClicked(var my_event)

    onShowAdd: {
        event_date = event_start_date;
        loader.sourceComponent = event_add;
        loader.item.title_text = "";
        float_event_edit.visible = true;
    }
    onShowEdit: {
        event_item = event;
        loader.sourceComponent = event_edit_show;
        float_event_edit.visible = true;
    }

    onHide: {
        float_event_edit.visible = false;
        panelItem.hoveredCellIndex = -1;
    }

    Connections {
        target: navigationbar_mouseArea
        onClicked: {
            if (float_event_edit.visible) {
                float_event_edit.hide();
            }
        }
    }

    Connections {
        target: background_mouseArea
        onClicked: {
            if (float_event_edit.visible) {
                float_event_edit.hide();
            }
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

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
    }

    Item {
        id: panel_item
        width: parent.width - shadow_radius
        height: parent.height - shadow_radius
        anchors.centerIn: parent

        Loader {
            id: loader
            anchors.fill: parent
            sourceComponent: event_add
        }
    }

    property Component event_edit_show: Rectangle {
        color: base_color

        property real margin: 20

        Rectangle {
            id: edit_content
            width: parent.width * 0.85
            height: parent.height * 0.8
            color: "transparent"
            anchors.centerIn: parent

            Rectangle {
                id: edit_title
                anchors.left: parent.left
                width: parent.width
                height: edit_title_text.height
                color: "transparent"

                Label {
                    id: edit_title_text
//                    anchors.fill: parent
                    width: parent.width

                    elide: Text.ElideRight
                    text: event_item ? event_item.displayLabel : ""
                    font.pointSize: font_size + 2
                    font.bold: true
                    color: "indigo"
                }

                MouseArea {
                    id: edit_title_mousearea
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        parent.color = "powderblue";
//                        cursorShape = Qt.IBeamCursor;
                        cursorShape = Qt.PointingHandCursor;
                    }
                    onExited: {
                        parent.color = "transparent";
                    }

                    onClicked: {
                        loader.sourceComponent = event_edit_enter;
                    }
                }
            }

            Label {
                id: edit_date_label

                anchors.left: parent.left
                anchors.top: edit_title.bottom
                anchors.topMargin: margin

                text: event_item ?
                          event_item.startDateTime.toLocaleDateString() : ""
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

                onClicked: deleteEvent();
            }

            MyTextLinkButton {
                id: edit_detail_button
                anchors.right: parent.right
                anchors.rightMargin: parent.width * 0.1
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

        function deleteEvent() {
            control.event_model.deleteEvent(event_item);
            float_event_edit.hide();
            control.refreshEvents();
        }
    }

    property Component event_edit_enter: Rectangle {
        color: base_color

        property real margin: 20

        Rectangle {
            id: edit_enter_content
            width: parent.width * 0.85
            height: parent.height * 0.8
            color: "transparent"
            anchors.centerIn: parent

            TextField {
                id: edit_enter_title
                anchors.left: parent.left
                width: parent.width

                text: event_item ? event_item.displayLabel : ""
                font.pointSize: font_size + 2
                font.bold: true
                textColor: "indigo"
                focus: true
            }

            Label {
                id: edit_enter_date

                anchors.left: parent.left
                anchors.top: edit_enter_title.bottom
                anchors.topMargin: margin

                text: event_item ?
                          event_item.startDateTime.toLocaleDateString() : ""
                font.pointSize: font_size
            }

            Rectangle {
                id: edit_enter_line

                anchors.bottom: edit_discard_button.top
                anchors.bottomMargin: margin
                width: parent.width
                height: 1

                color: "lightgray"
            }

            MyTextButton {
                id: edit_discard_button
                anchors.left: parent.left
                anchors.bottom: parent.bottom

                button_color: "lightgrey"
                hovered_color: Qt.lighter(button_color, 1.05)
                text_color: "black"
                text: qsTr("Discard changes")
                font_size: float_event_edit.font_size
            }

            MyTextButton {
                id: edit_save_button
                anchors.right: parent.right
//                anchors.rightMargin: parent.width * 0.1
                anchors.bottom: parent.bottom

                width: text_width * 2.0

                text: qsTr("Save")
                font_size: float_event_edit.font_size

                onClicked: saveEventTitle();
            }
        }

        MyIconButton {
            id: edit_exit_button
            width: 20
            height: 20

            anchors.left: edit_enter_content.right
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1

            icon_source: "images/close.png"

            onClicked: float_event_edit.hide()
        }

        function saveEventTitle() {
            event_item.displayLabel = edit_enter_title.text;
            control.event_model.saveEvent(event_item);
            float_event_edit.hide();
            control.refreshEvents();
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
                    id: header_label
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

                        button_color: Qt.lighter("blue", 1.3)
                        font_size: float_event_edit.font_size
                        text: qsTr("Create")

                        width: text_width * 1.5

                        onClicked: createAllDayEvent();
                    }

                    MyTextLinkButton {
                        id: edit_button
                        text_color: Qt.lighter("blue", 1.3)
                        text: qsTr("Edit >>")
                        font_size: float_event_edit.font_size
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

        function createAllDayEvent() {
            if (__isValidDate(event_date)) {
                var dt_start = new Date(event_date.getFullYear(),
                                        event_date.getMonth(),
                                        event_date.getDate(), 0, 0, 0);
                var dt_end = new Date();
                dt_end.setTime(dt_start.getTime() + 24*60*60*1000);

                var new_event = Qt.createQmlObject(
                            "import QtQuick 2.1; import MyCalendar.Utils.Events 1.0; MyEvent {}", float_event_edit);

                new_event.startDateTime = dt_start;
                new_event.displayLabel = title_edit.text;
                new_event.endDateTime = dt_end;
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
