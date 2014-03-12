import QtQuick 2.1
import QtQuick.Controls 1.0
//import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import MyCalendar2.Utils.Events 1.0
import "Content"

Item {
    id: float_edit
    width: 500
    height: 220

    z: 1

    property date event_date
    property var event_item

    property real font_size: 12

    property real shadow_radius: 8
    property color base_color: Qt.lighter("lightgray", 1.15)

    signal showAdd(date dt_start)
    signal showEdit(var event)
    signal hide()
//    signal saveClicked(var my_event)
    signal editDetails()
    signal addDetails()

    onShowAdd: {
        event_date = dt_start;
        loader.sourceComponent = event_add_panel;
        loader.item.title_text = "";
        loader.item.forceActiveFocus();
        float_edit.visible = true;
    }
    onShowEdit: {
        event_item = event;
        loader.sourceComponent = event_edit_panel;
        float_edit.visible = true;
    }

    onHide: {
        if (loader.item.state && loader.item.state !== "show") {
            loader.item.state = "show";
        }

        float_edit.visible = false;
        panelItem.hoveredCellIndex = -1;
    }

    onAddDetails: {
//        stack_view.push({ item: Qt.resolvedUrl("EventEditWindow.qml"),
//                        properties: {event_date: event_date} });
        stack_view.push({item: main_window.edit_view,
                            properties: {event_date: event_date, state: "add"}});
        float_edit.hide();
    }

    onEditDetails: {
        stack_view.push({item: main_window.edit_view,
                            properties: {event_item: event_item, state: "edit"}});
        float_edit.hide();

        console.log("Is allday: ", event_item.allDay);
    }

    Connections {
        target: navigationbar_mouseArea
        onClicked: {
            if (float_edit.visible) {
                float_edit.hide();
            }
        }
    }

    Connections {
        target: background_mouseArea
        onClicked: {
            if (float_edit.visible) {
                float_edit.hide();
            }
        }
    }

    Connections {
        target: control
        onRefreshEvents: {
            if (float_edit.visible) {
                float_edit.hide();
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
            sourceComponent: event_add_panel
        }
    }

    property Component event_edit_panel: Rectangle {
//    Rectangle {
        id: event_edit_panel
        color: base_color

        state: "show"

        property real vertical_margin: 20

        Rectangle {
            id: edit_content
            width: parent.width * 0.85
            height: parent.height * 0.8
            color: "transparent"
            anchors.centerIn: parent

            Item {
                id: edit_title
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.15
                width: parent.width
                height: event_edit_panel.state === "show" ?
                            edit_title_text.height :
                            edit_title_enter.height
//                visible: true

                Rectangle {
                    id: edit_title_show
                    anchors.fill: parent
                    color: "transparent"

                    visible: event_edit_panel.state === "show" ? true : false

                    Label {
                        id: edit_title_text
    //                    anchors.fill: parent
                        width: parent.width

                        elide: Text.ElideRight
                        text: event_item ? event_item.displayLabel : ""
                        font.pointSize: float_edit.font_size + 2
                        font.bold: true
                        color: "indigo"
                    }

                    MouseArea {
                        id: edit_title_mousearea
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            parent.color = "powderblue";
                            cursorShape = Qt.PointingHandCursor;
                        }
                        onExited: {
                            parent.color = "transparent";
                        }

                        onClicked: {
//                            loader.sourceComponent = event_edit_enter;
                            event_edit_panel.state = "edit";
                            edit_title_enter.forceActiveFocus();
                        }
                    }
                } // edit_title_show

                MyTextField {
                    id: edit_title_enter
                    width: parent.width

                    visible: event_edit_panel.state === "edit" ? true : false

                    text: event_item ? event_item.displayLabel : ""
                    font_size: float_edit.font_size + 2
//                    font.bold: true
                    textColor: "indigo"
                    border_radius: 0
                    focus: true
                }
            } // edit_title

            Label {
                id: edit_date_label

                anchors.left: parent.left
                anchors.top: edit_title.bottom
                anchors.topMargin: vertical_margin

                text: event_item ?
                          event_item.startDateTime.toLocaleDateString() : ""
                font.pointSize: float_edit.font_size
            }

            Rectangle {
                id: line

                anchors.bottom: left_bottom_button.top
                anchors.bottomMargin: vertical_margin / 2
                width: parent.width
                height: 1

                color: "lightgray"
            }

            Item {
                id: right_bottom_button
                anchors.right: parent.right
//                anchors.rightMargin: parent.width * 0.1
                anchors.bottom: parent.bottom
                height: event_edit_panel.state === "show" ?
                            edit_detail_button.height :
                            edit_save_button.height
                width: event_edit_panel.state === "show" ?
                           edit_detail_button.width :
                           edit_save_button.width

                MyTextLinkButton {
                    id: edit_detail_button
                    anchors.centerIn: parent

                    visible: event_edit_panel.state === "show" ? true : false

                    text_color: Qt.lighter("blue", 1.3)
                    text: qsTr("Edit >>")
                    font_size: float_edit.font_size + 2
                    font_bold: true

                    onClicked: float_edit.editDetails()
                }

                MyTextButton {
                    id: edit_save_button
                    anchors.centerIn: parent

                    visible: event_edit_panel.state === "edit" ? true : false

                    width: text_width * 2.0

                    button_color: "indigo"
                    text: qsTr("Save")
                    font_size: float_edit.font_size + 2

                    onClicked: {
                        event_item.displayLabel = edit_title_enter.text;
                        control.event_model.saveEvent(event_item);
                        float_edit.hide();
                        control.refreshEvents();
                    }
                }
            } // right_bottom_button

            Item {
                id: left_bottom_button
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                height: event_edit_panel.state === "show" ?
                            edit_delete_button.height :
                            edit_discard_button.height
                width: event_edit_panel.state === "show" ?
                           edit_delete_button.width :
                           edit_discard_button.width

                MyTextLinkButton {
                    id: edit_delete_button
                    anchors.centerIn: parent

                    visible: event_edit_panel.state === "show" ? true : false

                    text_color: Qt.lighter("blue", 1.3)
                    text: qsTr("Delete")
                    font_size: float_edit.font_size + 2

                    onClicked: deleteEvent();
                }

                MyTextButton {
                    id: edit_discard_button
                    anchors.centerIn: parent

                    visible: event_edit_panel.state === "edit" ? true : false

                    button_color: "lightgrey"
                    hovered_color: Qt.lighter(button_color, 1.05)
                    text_color: Qt.darker("darkgrey", 1.1)
                    text: qsTr("Discard changes")
                    font_size: float_edit.font_size
                    width: text_width * 1.2
                    height: text_height * 1.8

                    onClicked: float_edit.hide();
                }
            } // left_bottom_button
        } // edit_content

        MyIconButton {
            id: edit_close_button
            width: 20
            height: 20

            anchors.left: edit_content.right
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1

            icon_source: "images/close.png"

            onClicked: float_edit.hide()
        }

        function deleteEvent() {
            control.event_model.deleteEvent(event_item);
            float_edit.hide();
            control.refreshEvents();
        }
    } // event_edit_panel

    property Component event_add_panel: Rectangle {
        color: base_color

//        property string event_title
        property alias title_text: title_edit.text
        signal forceActiveFocus()

        onForceActiveFocus: title_edit.forceActiveFocus()

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
                    font.pointSize: float_edit.font_size + 2
                }

                GridLayout {
                    columns: 2
                    rowSpacing: 6
                    columnSpacing: 12

                    Label {
                        id: date_tip_label
                        text: qsTr("When: ")
                        font.pointSize: float_edit.font_size
                    }
                    Label {
                        id: date_show_label
                        text: event_date.toLocaleDateString(Qt.locale(), "MMMM dd  (ddd)")
                        font.pointSize: float_edit.font_size
                    }

                    Label {
                        id: title_tip
                        text: qsTr("What: ")
                        font.pointSize: float_edit.font_size
                    }
                    MyTextField {
                        id: title_edit
                        text: ""
                        Layout.fillWidth: true
                        font_size: float_edit.font_size
                        border_radius: 2
                        focus: true
                    }
                } // grid_layout

                RowLayout {
                    spacing: 20

                    MyTextButton {
                        id: create_button

                        button_color: Qt.lighter("blue", 1.3)
                        font_size: float_edit.font_size + 2
                        text: qsTr("Create")

                        width: text_width * 1.5

                        onClicked: createAllDayEvent();
                    }

                    MyTextLinkButton {
                        id: edit_button
                        text_color: Qt.lighter("blue", 1.3)
                        text: qsTr("Edit >>")
                        font_size: float_edit.font_size + 2

                        onClicked: float_edit.addDetails()
                    }
                } // button_layout
            } // column_layout
        } // add_content

        MyIconButton {
            id: close_button
            width: 20
            height: 20

            anchors.left: add_content.right
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1

            icon_source: "images/close.png"

            onClicked: float_edit.hide()
        }

        function createAllDayEvent() {
            if (__isValidDate(event_date)) {
                var dt_start = new Date(event_date.getFullYear(),
                                        event_date.getMonth(),
                                        event_date.getDate(), 0, 0, 0);
                var dt_end = new Date();
                dt_end.setTime(dt_start.getTime() + 24*60*60*1000);

                var new_event = Qt.createQmlObject(
                            "import QtQuick 2.1; import MyCalendar2.Utils.Events 1.0; MyEvent {}",
                            float_edit);

                new_event.startDateTime = dt_start;
                new_event.displayLabel = title_edit.text;
                new_event.endDateTime = dt_end;
                new_event.allDay = true;

                console.log("Start Datetime: " + new_event.startDateTime);
                console.log("End date time: ", new_event.endDateTime);
                console.log("Display Label: " + new_event.displayLabel);
                console.log("All day: " + new_event.allDay);
                control.event_model.saveEvent(new_event);

                float_edit.hide();
                control.refreshEvents();
            }

            new_event.destroy();
        } // createAllDayEvent
    } // event_add_panel
}
