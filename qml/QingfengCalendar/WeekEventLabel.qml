import QtQuick 2.1
import QtQuick.Controls 1.1
import "Private/CalendarUtils.js" as CalendarUtils

Item {
    id: week_event_label

    property var eventItem
    property int date_index
    property int stack_index
    property int max_stack
    property int start_cell_index
    property int end_cell_index

    readonly property int hours_in_day: CalendarUtils.hoursInADay
    readonly property int days_in_week: CalendarUtils.daysInAWeek

    property real cell_width: parent.width / days_in_week
    property real cell_height: parent.height / hours_in_day / 2

    property real base_width: (cell_width * 0.9) / max_stack

    width: (stack_index === max_stack) ? base_width : (base_width * 2)
    height: cell_height * (end_cell_index - start_cell_index + 1) - 2
    x: (date_index * cell_width) + (stack_index - 1) * base_width + 2
    y: (start_cell_index * cell_height) + 2

    z: stack_index

    property color base_color: {
        var the_color = Qt.darker("lightblue", 1.3);
        for( var i = 0; i < control.event_model.collections.length; ++i) {
            var collection = control.event_model.collections[i];
            if (eventItem &&
                    collection.collectionId === eventItem.collectionId) {
                the_color = collection.color;
            }
        }
        the_color;
    }

//    border.color: "lightgrey"
//    border.width: 2

    Rectangle {
        id: panel
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: resize_rectangle.top
        anchors.leftMargin: 2


        color: panel_mouse_area.pressed ? "orange" : base_color
        border.color: Qt.darker(base_color, 1.2)
        border.width: 1

        Label {
            id: time_label
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.top: parent.top
            width: parent.width * 0.8
            clip: true

            text: {
                var the_text = "";
                the_text += Qt.formatTime(eventItem.startDateTime, "hh:mm");
                the_text += " - ";
                the_text += Qt.formatTime(eventItem.endDateTime, "hh:mm");
                the_text;
            }
        }

        Label {
            id: title_label
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.top: time_label.bottom
            anchors.bottom: parent.bottom

            text: eventItem.displayLabel
            wrapMode: Text.Wrap
            width: parent.width * 0.9
        }

        property real old_x: 0
        property real old_y: 0

        MouseArea {
            id: panel_mouse_area
            anchors.fill: parent
            drag.target: week_event_label

            onPressed: {
//                old_x = week_event_label.x
//                old_y = week_event_label.y
            }
            onReleased: {

            }
        }
    }

    Rectangle {
        id: resize_rectangle
        width: parent.width
        height: 13
        //anchors.bottom: parent.bottom
        y: parent.height - 13
        anchors.leftMargin: 2

        color: resize_mouse_area.pressed ? Qt.darker(base_color, 1.3) :
                                           Qt.darker(base_color, 1.1)

        property real old_height: 0

        MouseArea {
            id: resize_mouse_area
            anchors.fill: parent
            //cursorShape: Qt.SizeVerCursor

            drag.target: resize_rectangle
            drag.axis: Drag.YAxis

            onPressed: {
                console.log("onPressed, panel.height:",
                            panel.height)
                parent.old_height = panel.height
            }
            onReleased: {
                console.log("resize_mouse_area, onReleased, panel.height",
                            panel.height)
                if (panel.height !== parent.old_height) {
                    parent.old_height = panel.height
                    resize_rectangle.y += (
                                cell_height -
                                (parent.old_height + resize_mouse_area.height) % cell_height);
                }
            }
        }
    }
}
