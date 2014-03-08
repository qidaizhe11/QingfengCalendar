import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import MyCalendar.Utils.Events 1.0
import "Content"

Window {
    id: event_edit_window

    property real window_width: 960
    property real window_height: 600

    property real pos_x
    property real pos_y

    property real font_size: 12

    visible: true
    color: "white"

    width: window_width
    height: window_height

    minimumHeight: 560
    minimumWidth: 800

//    MyEvent {
//        id: my_event_item
//    }

    property var event_item
    property date event_date: new Date()

    property real left_part_width: event_edit_window.width * 0.4

    Rectangle {
        id: left_part

//        color: "#36383c"
        color: "lightgrey"
        width: parent.width * 0.4
        height: parent.height
        anchors.left: parent.left

//        property real left_margin: left_part.width * 0.1
        property real title_font_size: 16

        Rectangle {
            id: calendar_title_rect
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.07
            anchors.top: parent.top
            height: parent.height * 0.15
//            width: parent.width

            Rectangle {
                id: calendar_color_block
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.5
                width: parent.height * 0.5
                color: "indigo"
            }
            Rectangle {
                id: calendar_title_content
                anchors.left: calendar_color_block.right
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.8
//                width: 300
//                color: "yellow"

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 1

                    Label {
                        id: title_text
                        text: qsTr("My Calendar")
                        font.pointSize: left_part.title_font_size
                    }
//                    Rectangle {
//                        color: "indigo"
//                        height: 1
//                        width: title_text.width
//                    }
//                    Label {
//                        id: email_text
//                        text: ""
//                    }
                }
            } // calendar_title_content
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
                        Button {
                            id: start_date_edit
                            width: parent.width
                            text: event_item ?
                                      event_item.startDateTime.toLocaleDateString() :
                                      event_date.toLocaleDateString()
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
                            spacing: 3

                            ComboBox {
                                id: start_hour_combo
                                width: parent.width / 3
                                model: hour_model
                            }
                            ComboBox {
                                id: start_minute_combo
                                width: parent.width / 3
                                model: minute_model
                            }

                            CheckBox {
                                id: allday_checkbox
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width / 3
                                text: qsTr("All day")
                                checked: true
                            }
                        }
                    }

                    // End DateTime
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing

                        Label {
                            text: qsTr("End")
                        }
                        Button {
                            id: end_date_edit
                            width: parent.width
                            text: event_item ?
                                      event_item.startDateTime.toLocaleDateString() :
                                      event_date.toLocaleDateString()
                        }
                        Row {
                            width: parent.width
                            spacing: 3

                            ComboBox {
                                id: end_hour_combo
                                width: parent.width / 3
                                model: hour_model
                            }
                            ComboBox {
                                id: end_minute_combo
                                width: parent.width / 3
                                model: minute_model
                            }
                        }
                    }

                    // Location
                    Column {
                        width: parent.width
                        spacing: parent.inside_spacing
                        Label {
                            text: qsTr("Where")
                        }
                        TextField {
                            id: location_label
                            width: parent.width
                        }
                    }

                    // Show more button
                    MyTextLinkButton {
                        id: show_more_button
                        text: qsTr("More details")
                        font_size: event_edit_window.font_size
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

            TextArea {
                id: description_edit
                anchors.top: line.bottom
                anchors.topMargin: parent.height * 0.02
                height: parent.height * 0.75
                width: parent.width

                text: qsTr("Add a description")
                font.pointSize: 12

                backgroundVisible: false
                frameVisible: false
            }

            property real button_width_level: 0.18
            property real button_font_size: 14

            MyTextButton {
                id: save_button
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: qsTr("Save")
                font_size: parent.button_font_size
                button_color: "indigo"
                width: parent.width * parent.button_width_level
                height: text_height * 1.6
            }

            MyTextButton {
                id: cancel_button
                anchors.left: save_button.right
                anchors.leftMargin: parent.width * 0.05
                anchors.bottom: parent.bottom
                width: parent.width * parent.button_width_level
                height: text_height * 1.6
                text: qsTr("Cancel")
                font_size: parent.button_font_size
                button_color: "lightgray"
                text_color: Qt.darker("darkgrey", 2.0)
                hovered_color: Qt.lighter(button_color, 1.1)
            }

            MyTextButton {
                id: delete_button
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width * parent.button_width_level
                height: text_height * 1.6
                text: qsTr("Delete")
                font_size: parent.button_font_size
                button_color: "lightgray"
                text_color: "grey"

                visible: event_item ? true : false
            }
        } // right_part_content
    } // right_part
}
