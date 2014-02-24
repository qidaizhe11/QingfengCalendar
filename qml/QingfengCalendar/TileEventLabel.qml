import QtQuick 2.1
import QtQuick.Controls 1.1
import QtOrganizer 5.0
import MyCalendar.Utils.Events 1.0
import "CalendarUtils.js" as CalendarUtils

Rectangle {
    id: event_label
    color: "lightblue"
    property variant eventItem

    property bool is_all_day: eventItem.allDay
    property int last_days: event_utils.lastDays(eventItem.startDateTime,
                                                 eventItem.endDateTime)
    property bool is_multi_days: (last_days === 1) ? true : false

    property color left_block_color: "blue"
    property color font_color: "white"

    readonly property int grid_rows: CalendarUtils.weeksOnCalendarMonth
    readonly property int grid_columns: CalendarUtils.daysInWeek
    property int grid_index: event_utils.gridIndex(eventItem.startDateTime)
    property int horizontal_index: grid_index % grid_columns
    property int vertical_index: grid_index % grid_rows

    width: parent.width / grid_columns * last_days;
    height: parent.height / ( 3 * grid_rows)
    x: parent.width / grid_columns * horizontal_index
    y: parent.height / grid_rows * vertical_index

    EventUtils {
        id: event_utils
    }

    Item {
        id: view

        Rectangle {
            id: color_block
            width: 10
            height: parent.height
            anchors.left: parent.left
            color: left_block_color
        }
        Label {
            id: title_label
            text: eventItem.description
            clip: true
            color: font_color
            anchors.left: color_block.right
            anchors.leftMargin: 6
            anchors.right: parent.right
        }
    }
}
