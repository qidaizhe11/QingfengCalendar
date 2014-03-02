import QtQuick 2.1
import QtQuick.Controls 1.1
import QtOrganizer 5.0
import MyCalendar.Utils.Events 1.0
import "Private/CalendarUtils.js" as CalendarUtils

Rectangle {
    id: event_label
    color: base_color
    property var eventItem

    property int show_flag_of_day: 0
    property int grid_index
    property int last_days: 1

    property int __index: -1

    property bool is_all_day: eventItem.allDay
//    property int last_days: event_utils.lastDays(eventItem.startDateTime,
//                                                 eventItem.endDateTime)
    property bool is_multi_days: (last_days === 1) ? true : false

    property color base_color: Qt.darker("lightblue", 1.3)
    property color left_block_color: "yellow"
    property color font_color: "white"

    readonly property int display_weeks: CalendarUtils.weeksOnCalendarMonth
    readonly property int days_in_week: CalendarUtils.daysInWeek
//    property int grid_index: event_utils.gridIndex(eventItem.startDateTime)
    property int horizontal_index: grid_index % days_in_week
    property int vertical_index: grid_index / days_in_week

    width: parent.width / days_in_week * last_days;
    height: parent.height / ( 3 * display_weeks)
    x: parent.width / days_in_week * horizontal_index
    y: parent.height / display_weeks * vertical_index + height * (show_flag_of_day + 1)

    EventUtils {
        id: event_utils
    }

    Item {
        id: view
        height: parent.height
        width: parent.width

        Rectangle {
            id: color_block
            width: 8
            height: parent.height
            anchors.left: parent.left
            color: left_block_color
        }
        Label {
            id: title_label
            text: eventItem.displayLabel
            font.pointSize: 12
            clip: true
            color: font_color
            anchors.left: color_block.right
            anchors.leftMargin: 6
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            id: mask_block
            width: parent.height
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
//                GradientStop { position: 0.0; color: "white" }
                GradientStop { position: 1.0; color: base_color }
            }
            rotation: -90
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            event_label.color = Qt.darker(base_color, 1.5);
        }
    }

//    Component.onCompleted: event_label.destroy();
}
