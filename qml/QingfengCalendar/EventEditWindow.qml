import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import MyCalendar.Utils.Events 1.0

Window {
    id: event_edit_window

    property real window_width: 960
    property real window_height: 600

    property real pos_x
    property real pos_y

    visible: true
    color: "white"

    width: window_width
    height: window_height

    MyEvent {
        id: my_event_item
    }

    property real left_part_width: event_edit_window.width * 0.4

    Rectangle {
        id: left_part

        color: "lightgrey"
        width: left_part_width
        height: parent.height
        anchors.left: parent.left

//        property real left_margin: left_part.width * 0.1
        property real title_font_size: 16

        Rectangle {
            id: calendar_title_rect
            color: "lightgrey"
            anchors.right: parent.right
            width: parent.width * 0.9
            anchors.top: parent.top
            height: parent.height * 0.1

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
                    hour_model.append({ "text": i.toString()});
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

//        Rectangle {
//            anchors.right: parent.right
//            width: parent.width * 0.9
//            anchors.bottom: parent.bottom
//            height: parent.height * (1 - 0.1)

        ScrollView {
            id: event_scroll_view
//            anchors.fill: parent
//            color: "transparent"
            anchors.right: parent.right
            width: parent.width * 0.9
            anchors.bottom: parent.bottom
            height: parent.height * (1 - 0.1)

            flickableItem.interactive: true

            Flickable {
                id: event_edit_flickable
                anchors.fill: parent
                contentHeight: 1000
                clip: true

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width * 0.9

                    spacing: 6

                    // Date
                    Label {
                        text: qsTr("When")
                    }
                    TextField {
                        id: start_date
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "Start"
                    }

                    RowLayout {
                        spacing: parent.width * 0.05

                        ComboBox {
                            id: start_hour_combo
                            model: hour_model
//                            acceptableInput: true
                            editable: true
                            currentIndex: 1
//                            validator: IntValidator { bottom: 0; top: 23; }
                        }
                        ComboBox {
                            id: start_minute_combo
                            model: minute_model
                            editable: true
//                            validator: IntValidator { bottom: 0; top: 59; }
                        }
                    }

                    Label {
                        text: "End"
                    }
                    TextField {
                        id: end_date
                        Layout.fillWidth: true
                    }

                } // column layout

            } // flickable
        } // scroll view
    }
}
