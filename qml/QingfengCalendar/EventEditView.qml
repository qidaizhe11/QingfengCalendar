import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
//import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import MyCalendar.Utils.Events 1.0
import "Content"

Rectangle {
    id: event_edit_view

    width: parent.width
    height: parent.height

    property real font_size: 12

    visible: true
    color: "white"

    property var event_item
    property date event_date: new Date()

    property real left_part_width: event_edit_view.width * 0.4

    Component.onCompleted: title_edit.forceActiveFocus()

    state: "add"

    signal cancel()

    onCancel: {
//        event_item = undefined;
        if (stack_view) stack_view.pop();
        event_edit_view.state = "add";
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

        Rectangle {
            id: calendar_title_rect
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: parent.top
            height: parent.height * 0.15

            Row {
                anchors.fill: parent
                spacing: 6

                MyRoundButton {
                    id: back_button

                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height * 0.45
                    height: parent.height * 0.45

                    button_color: "indigo"
                    icon_source: "images/back.png"

                    onClicked: event_edit_view.cancel()
                }

                Rectangle {
                    id: calendar_color_block

                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height * 0.5
                    width: parent.height * 0.5
                    color: "indigo"
                }

                Label {
                    id: title_text

                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("My Calendar")
                    font.pointSize: left_part.title_font_size
                }
            } // row
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

        ScrollView {
            id: event_scroll_view

            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: calendar_title_rect.bottom
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
                            selected_date: event_edit_view.state == "edit"  ?
                                               event_item.startDateTime :
                                               event_date
                        }
                    }

                    // Start Time
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing

                        Label {
                            text: "Start"
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

                                currentIndex: event_edit_view.state == "edit" ?
                                                 event_item.startDateTime.getHours() :
                                                 event_date.getHours()
                            }
                            ComboBox {
                                id: start_minute_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: minute_model

                                currentIndex:
                                    event_edit_view.state == "edit"  ?
                                        Math.floor(
                                            event_item.startDateTime.getMinutes() / 5) :
                                        Math.floor(event_date.getMinutes() / 5)
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
                            selected_date: event_edit_view.state == "edit" ?
                                               event_item.endDateTime :
                                               event_date
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

                                currentIndex: event_edit_view.state == "edit" ?
                                                 event_item.endDateTime.getHours() :
                                                 event_date.getHours()
                            }
                            ComboBox {
                                id: end_minute_combo
                                width: parent.width / 3
                                height: left_part.item_height
                                model: minute_model

                                currentIndex:
                                    event_edit_view.state == "edit"  ?
                                        Math.floor(
                                            event_item.endDateTime.getMinutes() / 5) :
                                        Math.floor(event_date.getMinutes() / 5)
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
                            // TODO: Add location in MyEvent
//                            text:
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
            anchors.rightMargin: parent.width * 0.05
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

                style: TextFieldStyle {
                    padding { top: 0; bottom: 0; left: 0; right: 0 }
                    background: Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                    }
                }
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

                // TODO: Add description in MyEvent
//                text:
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
            }

//            MyTextButton {
//                id: delete_button
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom
//                width: parent.width * parent.button_width_level
//                height: text_height * 1.6
//                text: qsTr("Delete")
//                font_size: parent.button_font_size
//                button_color: "lightgray"
//                text_color: "grey"

//                visible: event_item ? true : false
//            }
        } // right_part_content
    } // right_part
}
