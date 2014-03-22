import QtQuick 2.1
import QtQuick.Controls 1.1
import "Private/CalendarUtils.js" as CalendarUtils

Rectangle {
    id: root

//    color: "transparent"

    property var eventItem
    property int date_index
    property int stack_index
    property int max_stack
    property int start_cell_index
    property int end_cell_index

//    property rect base_rect: Qt.rect()
//    property int ranged_cells

    readonly property int hours_in_day: CalendarUtils.hoursInADay
    readonly property int days_in_week: CalendarUtils.daysInAWeek

    property real cell_width: parent.width / days_in_week
    property real cell_height: parent.height / hours_in_day / 2

    property real base_width: (cell_width * 0.9) / max_stack

    width: (stack_index === max_stack) ? base_width : (base_width * 2)
    height: cell_height * (end_cell_index - start_cell_index + 1)
//    x: base_rect.x + (stack_index - 1) * base_width
//    y: base_rect.y
    x: (date_index * cell_width) + (stack_index - 1) * base_width
    y: (start_cell_index * cell_height)

    z: stack_index

    border.color: "lightgrey"
    border.width: 2

    Rectangle {
        id: week_event_label
        anchors.fill: parent
        anchors.margins: 2

        //    width: (stack_index === max_stack) ? base_width : (base_width * 2)
        //    height: cell_height * (end_cell_index - start_cell_index + 1)
        //    x: (date_index * cell_width) + stack_index * base_width
        //    y: (start_cell_index * cell_height)

        //    opacity: 0

        Component.onCompleted: {
            console.log("WeekEventLabel, onCompleted.");
            console.log("width:", width, "height", height, "x:", x, "y:", y);
        }

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
        color: base_color
        border.color: Qt.darker(base_color, 1.2)
        border.width: 1

        //    property color font_color: "black"

        Item {
            id: panel
            anchors.fill: parent
            anchors.margins: 2

            Label {
                id: time_label
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.top: parent.top
                width: parent.width * 0.8
                //            height: 20
                clip: true

                text: {
                    var the_text = "";
                    the_text += eventItem.startDateTime.toLocaleTimeString(
                                Qt.locale(), "hh:mm");
                    the_text += " - ";
                    the_text += eventItem.endDateTime.toLocaleTimeString(
                                Qt.locale(), "hh:mm");
                    the_text;
                }
            }

            Label {
                id: title_label
                text: eventItem.displayLabel
                //            font.pointSize: 12
                //            clip: true
                wrapMode: Text.Wrap
                width: parent.width * 0.9
                //            color: font_color
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.top: time_label.bottom
                anchors.bottom: parent.bottom
            }
        }
    }

    //    NumberAnimation on opacity { from: 0; to: 1; duration: 200; }
}
