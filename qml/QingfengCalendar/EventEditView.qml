import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
//import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import MyCalendar2.Events 1.0
import "Content"

Rectangle {
    id: event_edit_view

    width: parent.width
    height: parent.height

    visible: true
    color: "white"

    state: "add"

    property var event_item
    property date event_date: new Date()

    signal cancel()
    signal saveEvent()
    signal deleteEvent()

    onCancel: {
        if (stack_view) stack_view.pop();
    }

    onDeleteEvent: {
        calendar.event_model.deleteEvent(event_item);
        calendar.refreshEvents();
        state = "add";
        if (stack_view) stack_view.pop();
    }

    property date start_date: state === "edit" ? event_item.startDateTime :
                                                 event_date
    property date end_date: state === "edit" ? event_item.endDateTime :
                                               event_date

//    property real left_part_width: width * 0.4

    property real font_size: 12

    property bool is_date_error: false

    Component.onCompleted: {
        title_edit.forceActiveFocus()
    }

    property date start_time: {
        var the_date;
        if (state === "add") {
            the_date = new Date();
            var hour = the_date.getHours();
            var minute = the_date.getMinutes();

            if (minute >= 30) {
                the_date.setHours(hour + 1);
                the_date.setMinutes(0);
            } else {
                the_date.setHours(hour);
                the_date.setMinutes(30);
            }
        } else {
            the_date = event_item.startDateTime;
        }

        the_date;
    }

    property date end_time: {
        var the_date;
        if (state === "add") {
            the_date = start_time;
            the_date.setHours(the_date.getHours() + 1);
        } else {
            the_date = event_item.endDateTime;
        }

//        console.log("End_time, add hour failed? ", the_date);

        the_date;
    }

    Rectangle {
        id: left_part

//        color: "#36383c"
        color: "lightgrey"
        width: parent.width * 0.4
        height: parent.height
        anchors.left: parent.left
        property real title_font_size: 16
        property real item_height: left_part.height * 0.05

        Item {
            id: calendar_title_rect
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: parent.top
            height: parent.height * 0.15

            MyRoundButton {
                id: back_button

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.height * 0.45
                height: parent.height * 0.45

                button_color: "indigo"
                icon_source: "images/back.png"

                onClicked: event_edit_view.cancel()
            }

            Item {
                id: calendar_title
                anchors.left: back_button.right
                anchors.leftMargin: 15
                height: parent.height

                Rectangle {
                    id: calendar_color_block

                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height * 0.5
                    height: parent.height * 0.5
                    color: "indigo"
                }

                Label {
                    id: title_text

                    anchors.left: calendar_color_block.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter

//                    text: event_collection.name
                    text: "MyCallendar"
                    font.pointSize: left_part.title_font_size
                }
            }
        } // calendar_title_rect

        ListModel {
            id: hour_model
            Component.onCompleted: {
                for (var i = 0; i < 24; ++i) {
                    if (i < 10) {
                        hour_model.append({ "text": '0' + i.toString()});
                    } else {
                        hour_model.append({ "text": i.toString() });
                    }
                }
            }
        }

        ListModel {
            id: minute_model
            Component.onCompleted: {
                for (var i = 0; i < 60; i += 5) {
                    if (i < 10) {
//                        var str = ("0" + i).toString();
                        minute_model.append({ "text": '0' + i.toString()});
                    } else {
                        minute_model.append({ "text": i.toString() });
                    }
                }
            }
        }

        Label {
            id: error_label

            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: calendar_title_rect.bottom

            height: is_date_error ? (parent.height * 0.05) : 0
            opacity: is_date_error ? 1 : 0

            text: qsTr("End time is earlier than start time.")
            color: "red"
            font.pointSize: font_size

            NumberAnimation on opacity { duration: 500; }
        }

        ScrollView {
            id: event_scroll_view

            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: error_label.bottom
            height: parent.height * 0.85
            width: parent.width * 0.93

            flickableItem.interactive: true

            Flickable {
                id: event_edit_flickable
//                anchors.fill: parent
                anchors.centerIn: parent
                width: parent.width
                height: parent.height < edit_layout.height ?
                            parent.height : edit_layout.height
//                contentHeight: edit_layout.height
//                contentWidth: edit_layout.width
                flickableDirection: Flickable.VerticalFlick
                clip: true

                Column {
                    id: edit_layout
                    anchors.fill: parent
                    anchors.rightMargin: parent.width * 0.1
//                    width: parent.width * 0.9

                    spacing: 10

                    property real inside_spacing: 3

                    // Start Date
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing

                        Label {
                            text: qsTr("When")
                        }
                        MyDateEdit {
                            id: start_date_edit
                            width: parent.width
                            height: left_part.item_height
                            selected_date: start_date
                        }
                    }

                    // Start Time
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing

                        Label {
                            text: "Start"
                            enabled: allday_checkbox.checked ? false : true
                        }

                        Row {
                            width: parent.width
                            height: left_part.item_height
                            spacing: height * 0.2

                            ComboBox {
                                id: start_hour_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: hour_model

                                enabled: allday_checkbox.checked ? false : true
                                currentIndex: 0
                            }
                            ComboBox {
                                id: start_minute_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: minute_model

                                currentIndex: 0
                                enabled: allday_checkbox.checked ? false : true
                            }

                            CheckBox {
                                id: allday_checkbox
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width / 3
                                text: qsTr("All day")
                                checked: true
                            }
                        }
                    } // dt_start

                    // End DateTime
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing

                        Label {
                            text: qsTr("End")
                        }
                        MyDateEdit {
                            id: end_date_edit
                            width: parent.width
                            height: left_part.item_height
                            selected_date: end_date
                        }
                        Row {
                            width: parent.width
                            height: left_part.item_height
                            spacing: height * 0.2

                            ComboBox {
                                id: end_hour_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: hour_model

                                currentIndex: 0
                                enabled: allday_checkbox.checked ? false : true
                            }
                            ComboBox {
                                id: end_minute_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: minute_model

                                enabled: allday_checkbox.checked ? false : true
                                currentIndex: 0
                            }
                        }
                    } // dt_end

                    // Location
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing
                        Label {
                            text: qsTr("Where")
                        }
                        // 注：系统原生TextField，在我的MyDateEdit中日历展开时，
                        // MouseEvent将被莫名其妙地获取。
                        MyTextField {
                            id: location_label
                            width: parent.width
                            height: left_part.item_height
                            font_size: event_edit_view.font_size

                            text: event_edit_view.state === "edit" ?
                                      event_item.location : ""
                        }
                    }

                    // Show more button
                    MyTextLinkButton {
                        id: show_more_button
                        text: qsTr("More details")
                        font_size: event_edit_view.font_size
                        text_color: "indigo"
                    }

                } // column layout

            } // flickable
        } // scroll view
    } // left part

    Rectangle {
        id: right_part

//        width: parent.width - parent.left_part_width
        width: parent.width * 0.6
        height: parent.height
//        anchors.left: left_part.right
        anchors.right: parent.right
        anchors.top: parent.top

        Rectangle {
            id: right_part_content
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.05
            anchors.rightMargin: parent.width * 0.04
            anchors.topMargin: parent.height * 0.05
            anchors.bottomMargin: parent.height * 0.05

            TextField {
                id: title_edit
                anchors.top: parent.top
                height: parent.height * 0.1
                width: parent.width
                font.pointSize: 18
                placeholderText: qsTr("Add a subject")

                text: event_edit_view.state == "edit" ?
                          event_item.displayLabel : ""

                focus: true

                style: TextFieldStyle {
                    padding { top: 0; bottom: 0; left: 0; right: 0 }
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                }

                horizontalAlignment: Text.AlignLeft

                onFocusChanged: {
                    if (!focus) {
                        cursorPosition = 0;
                    }
                }

//                onActiveFocusChanged: {
//                    if (activeFocus) {
//                        selectAll();
//                    }
//                }
            }

            Rectangle {
                id: line
                anchors.top: title_edit.bottom
//                anchors.topMargin: parent.height * 0.02
                width: parent.width
                height: 1
                color: "lightgrey"
            }

            MyTextArea {
                id: description_edit
                anchors.top: line.bottom
                anchors.topMargin: parent.height * 0.02
                height: parent.height * 0.75
                width: parent.width

                font_size: 12
                placeholder_text: qsTr("Add a description")

                text: event_edit_view.state === "edit" ?
                          event_item.description : ""
            }

            property real button_width_level: 0.18
            property real button_font_size: 12

            MyButton {
                id: save_button
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: qsTr("Save")
                width: parent.width * parent.button_width_level
                height: parent.height * 0.08
                button_color: "indigo"
                font_size: parent.button_font_size
                font_bold: true
                border_width: 2

                onClicked: event_edit_view.saveEvent();
            }

            MyButton {
                id: cancel_button
                anchors.left: save_button.right
                anchors.leftMargin: parent.width * 0.05
                anchors.bottom: parent.bottom
                width: parent.width * parent.button_width_level
                height: parent.height * 0.08

                text: qsTr("Cancel")
                font_size: parent.button_font_size
                button_color: Qt.darker("lightgray", 1.6)
//                font_bold: true

                onClicked: event_edit_view.cancel();
            }

            MyButton {
                id: delete_button
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width * parent.button_width_level
                height: parent.height * 0.08

                text: qsTr("Delete")
                font_size: parent.button_font_size
                button_color: Qt.darker("lightgray", 1.6)

                visible: event_edit_view.state === "edit" ? true : false

                onClicked: event_edit_view.deleteEvent();
            }
        } // right_part_content
    } // right_part

    states: [
        State {
            name: "edit"
            PropertyChanges { target: allday_checkbox;
                checked: event_item.allDay; }
        },
        State {
            name: "add"
            PropertyChanges { target: allday_checkbox; checked: true; }
        }
    ]

    property alias selected_start_date: start_date_edit.selected_date
    property alias selected_end_date: end_date_edit.selected_date

    Connections {
        target: allday_checkbox
        onCheckedChanged: {
            if (!allday_checkbox.checked) {
                end_hour_combo.currentIndex = end_time.getHours();
                end_minute_combo.currentIndex =
                        Math.floor(end_time.getMinutes() / 5);
                start_hour_combo.currentIndex = start_time.getHours();
                start_minute_combo.currentIndex =
                        Math.floor(start_time.getMinutes() / 5);
            }
            if (allday_checkbox.checked) {
                var date = start_time;
                date.setHours(start_hour_combo.currentIndex);
                date.setMinutes(start_minute_combo.currentIndex * 5);
                start_time = date;

                date = end_time;
                date.setHours(end_hour_combo.currentIndex);
                date.setMinutes(end_minute_combo.currentIndex * 5);
                end_time = date;

                start_hour_combo.currentIndex = 0;
                start_minute_combo.currentIndex = 0;
                end_hour_combo.currentIndex = 0;
                end_minute_combo.currentIndex = 0;
            }
        }
    }

    property date selected_start_time: {
        var the_time = selected_start_date;

        the_time.setHours(start_hour_combo.currentIndex);
        the_time.setMinutes(start_minute_combo.currentIndex * 5);

        the_time;
    }

    property date selected_end_time: {
        var the_time = selected_end_date;

        the_time.setHours(end_hour_combo.currentIndex);
        the_time.setMinutes(end_minute_combo.currentIndex * 5);

        the_time;
    }

    onSelected_start_dateChanged: {
        if (selected_end_date < selected_start_date) {
            selected_end_date = selected_start_date;
        }
    }

    onSelected_start_timeChanged: {
        if (selected_end_time.getTime() < selected_start_time.getTime()) {
            end_hour_combo.currentIndex = start_hour_combo.currentIndex;
            end_minute_combo.currentIndex = start_minute_combo.currentIndex;

//            console.log("Come here?")
        } else if (is_date_error) {
            is_date_error = false;
        }

        console.log("Start time changed.")
        console.log("Start time: ", selected_start_time.toLocaleString());
        console.log("End time: ", selected_end_time.toLocaleString());
    }

    onSelected_end_timeChanged: {
        if (selected_end_time.getTime() < selected_start_time.getTime()) {
            is_date_error = true;
        } else {
            is_date_error = false;
        }

        console.log("End time changed.")
        console.log("Start time: ", selected_start_time.toLocaleString());
        console.log("End time: ", selected_end_time.toLocaleString());
    }

    onSaveEvent: {
        var new_event = Qt.createQmlObject(
                    "import QtQuick 2.1; import MyCalendar2.Events 1.0; MyEvent {}",
                    event_edit_view);
        new_event.startDateTime = selected_start_time;
        new_event.allDay = allday_checkbox.checked;
        new_event.endDateTime = selected_end_time;
        new_event.displayLabel = title_edit.text;
        new_event.description = description_edit.text;
        new_event.location = location_label.text;
        calendar.event_model.saveEvent(new_event);
        calendar.refreshEvents();
        if (stack_view) stack_view.pop();
    }
}
