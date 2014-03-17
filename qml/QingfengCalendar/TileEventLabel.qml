import QtQuick 2.1
import QtQuick.Controls 1.0
import QtOrganizer 5.0
import MyCalendar2.Events 1.0
import "Private/CalendarUtils.js" as CalendarUtils
import "EventJsUtils.js" as EventJsUtils
import "Content"

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

//    property color base_color: Qt.darker("lightblue", 1.3)
    property color left_block_color: "yellow"
    property color font_color: "white"

    readonly property int display_weeks: CalendarUtils.weeksOnCalendarMonth
    readonly property int days_in_week: CalendarUtils.daysInAWeek
//    property int grid_index: event_utils.gridIndex(eventItem.startDateTime)
    property int horizontal_index: grid_index % days_in_week
    property int vertical_index: Math.floor(grid_index / days_in_week)

    width: parent.width / days_in_week * last_days;
    height: parent.height / ( 3 * display_weeks)
    x: parent.width / days_in_week * horizontal_index
    y: parent.height / display_weeks * vertical_index + height * (show_flag_of_day + 1)

//    Component.onCompleted: {
//        console.log("In TileEventlabel.")
//        console.log(eventItem.startDateTime, ", ", eventItem.displayLabel);
//        console.log("Index: ", grid_index)
//        console.log("h_index: ", horizontal_index);
//        console.log("v_index: ", vertical_index);
//    }

    opacity: 0

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

    MyMouseArea {
        anchors.fill: parent
        hoverEnabled: true

        propagateComposedEvents: false
        preventStealing: true

        onEntered: parent.color = Qt.lighter(base_color, 1.1);
        onExited: parent.color = base_color;
        onPressed: parent.color = Qt.darker(base_color, 1.5);
        onClicked: {
            var date = control.__model.dateAt(grid_index);
            if (__isValidDate(date)) {
//                            control.clicked(date);
            }

            var global_pos = event_label.mapToItem(null, mouseX, mouseY);
            console.log("Global position: " + global_pos.x + ", " + global_pos.y);

            var show_pos_x = EventJsUtils.getEditViewPosX(global_pos.x);
            var show_pos_y = EventJsUtils.getEditViewPosY(
                        global_pos.y, grid_index);
            console.log("Shown Pos: " + show_pos_x + ", " + show_pos_y);
            float_edit.x = show_pos_x;
            float_edit.y = show_pos_y;
            float_edit.showEdit(eventItem);
        }
    }

    NumberAnimation on opacity { from: 0; to: 1; duration: 200; }

//    Component.onCompleted: event_label.destroy();
}
