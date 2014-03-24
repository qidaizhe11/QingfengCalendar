import QtQuick 2.1
import QtQuick.Controls 1.0
import MyCalendar.Controls.Private 1.0
import "../Private"
import "../Private/CalendarUtils.js" as CalendarUtils
import MyCalendar.Weeks 1.0
import "../CreateObject.js" as CreateObject

Item {
    id: week_view

    readonly property int rows: CalendarUtils.hoursInADay * 2
    readonly property int columns: CalendarUtils.daysInAWeek

    // holes the model that will be used by the Calendar to populate week date
    // that available to the user.
    property MyWeekModel week_model: MyWeekModel {
        visibleDate: control.visible_date
    }

    // The title shown in the navigation bar, it's format various from view
    // to view.
    property string calendar_title: {
        var title = "";
        var range_start_date = week_model.firstVisibleDate;
        var format = qsTr("yyyy年M月d日");
        title += range_start_date.toLocaleDateString(Qt.locale(), format);
        title += " - ";
        var range_end_date = week_model.lastVisibleDate;
        if (range_start_date.getMonth() === range_end_date.getMonth()) {
            format = qsTr("d日");
            title += range_end_date.toLocaleDateString(Qt.locale(), format);
        } else if (range_start_date.getFullYear() ===
                   range_end_date.getFullYear()) {
            format = qsTr("M月d日");
            title += range_end_date.toLocaleDateString(Qt.locale(), format);
        } else {
            title += range_end_date.toLocaleDateString(Qt.locale(), format);
        }
        title;
    }

    // Once the tab_view index changed, the model should be refreshed,
    // since the visible date may be changed already.
    Connections {
        target: tab_view
        onCurrentIndexChanged: {
            if (week_view.visible) {
                control.refreshEvents();
                console.log("WeekView, onCurrentIndexChanged.");
            }
        }
    }

    // Bind the title to navigationbar, only take over the title
    // when this tab in tab_view is active.
    Binding {
        when: week_view.visible
        target: navigationBarLoader
        property: "styleData.title"
        value: calendar_title
    }

    Connections {
        target: navigationBarLoader.item
        onShowPrevious: {
            if (week_view.visible) {
                var date = control.visible_date;
                date.setDate(date.getDate() - week_view.columns);
                control.visible_date = date;
                control.refreshEvents();
            }
        }
    }
    Connections {
        target: navigationBarLoader.item
        onShowNext: {
            if (week_view.visible) {
                var date = control.visible_date;
                date.setDate(date.getDate() + week_view.columns);
                control.visible_date = date;
                control.refreshEvents();
            }
        }
    }

    property Component dayDelegate: Rectangle {
        readonly property color base_color: "transparent"
        readonly property color text_color: Qt.darker("darkgray", 3.0)

        readonly property color selected_base_color: Qt.darker("darkgray", 1.4)
        readonly property color selected_text_color: "white"
        readonly property color invalid_color: "#dddddd"

        color: {
            var the_color = invalid_color;
            if (styleData.valid) {
                the_color = base_color
                if (styleData.today) {
                    the_color = selected_base_color;
                }
            }

            the_color;
        }

        Label {
            text: styleData.date.toLocaleDateString(Qt.locale(), "M/d (ddd)")
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            anchors.verticalCenter: parent.verticalCenter

            color: {
                var theColor = invalid_color;
                if (styleData.valid) {
                    theColor = text_color;
                    if (styleData.today) {
                        theColor = selected_text_color;
                    }
                }
                theColor;
            }
        }
    }

    property Component dayHourDelegate: Rectangle {
//    Rectangle {
        color: my_calendar_style.base_color
        Label {
            text: {
                var the_text;
                var the_time = new Date();
                the_time.setHours(styleData.index);
                the_time.setMinutes(0);
                the_text = the_time.toLocaleTimeString(Qt.locale(), "hh:mm");
                the_text;
            }

            anchors.right: parent.right
            anchors.rightMargin: parent.width * 0.05
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1
        }
    }

    Item {
        id: week_panel_item
        anchors.fill: parent

//        width: week_panel_item.width
//        height: week_panel_item.height

        readonly property real availableWidth:
            (week_view_inner_container.width - (control.gridVisible ? __gridLineWidth : 0))
        readonly property real availableHeight:
            (week_inner_row.height - (control.gridVisible ? __gridLineWidth : 0))

        readonly property real hour_column_width: 60

        function __cellRectAt(index) {
            return CalendarUtils.cellRectAt(index,
                                            week_view.columns,
                                            week_view.rows,
                                            week_panel_item.availableWidth,
                                            week_panel_item.availableHeight);
        }

        Item {
            id: day_header_row
            width: parent.width - parent.hour_column_width
            height: panelItem.dayOfWeekHeaderRowHeight
            anchors.left: parent.left
            anchors.leftMargin: parent.hour_column_width + __gridLineWidth
            anchors.top: parent.top

            Row {
                spacing: (control.gridVisible ? __gridLineWidth : 0)

                Repeater {
                    id: day_header_repeater
                    model: week_model
                    Loader {
                        id: dayDelegateLoader
                        sourceComponent: dayDelegate
                        width: week_panel_item.__cellRectAt(index).width -
                               (control.gridVisible ? __gridLineWidth : 0)
                        height: day_header_row.height

                        readonly property int __index: index
                        readonly property date __date: date
                        readonly property bool valid: __isValidDate(date)

                        property QtObject styleData: QtObject {
                            readonly property alias index: dayDelegateLoader.__index
                            readonly property alias date: dayDelegateLoader.__date
                            readonly property bool valid: dayDelegateLoader.valid
                            // TODO: this will not be correct if the app is
                            // running when a new day begins.
                            readonly property bool today:
                                date.getTime() === new Date().setHours(0, 0, 0, 0)
                        }
                    }
                }
            } // row
        } // day_header_row

        Flickable {
            id: week_flickable
            anchors.top: day_header_row.bottom
            width: parent.width
            height: (parent.height - day_header_row.height) < week_inner_row.height ?
                        (parent.height - day_header_row.height) : week_inner_row.height
            flickableDirection: Flickable.VerticalFlick
            contentHeight: 1300
            clip: true

            Row {
                id: week_inner_row
                anchors.fill: parent

                Item {
                    id: day_hours_column
                    width: 60
                    height: parent.height

                    Repeater {
                        id: day_time_repeater
                        model: week_view.rows / 2

                        Loader {
                            id: day_time_delegate_loader
                            y: week_panel_item.__cellRectAt(index * week_view.columns * 2).y +
                               (control.gridVisible ? __gridLineWidth : 0)
                            width: day_hours_column.width
                            height: week_panel_item.__cellRectAt(index * week_view.columns).height -
                                    (control.gridVisible ? __gridLineWidth : 0)
                            sourceComponent: dayHourDelegate

                            readonly property int __index: index

                            property QtObject styleData: QtObject {
                                readonly property alias index:
                                    day_time_delegate_loader.__index
                            }
                        }
                    }
                } // day_hours_column

                Item {
                    id: week_view_inner_container

                    width: parent.width - day_hours_column.width
                    height: parent.height

                    Repeater {
                        id: verticalGridLineRepeater
                        model: week_view.columns
                        delegate: Rectangle {
                            x: index < week_view.columns ?
                                   week_panel_item.__cellRectAt(index).x :
                                   week_panel_item.__cellRectAt(week_view.columns - 1).x +
                                   week_panel_item.__cellRectAt(week_view.columns - 1).width
                            y: 0
                            width: __gridLineWidth
                            height: week_view_inner_container.height
                            color: my_calendar_style.gridColor
                            visible: control.gridVisible
                        }
                    }

                    Repeater {
                        id: horizontalGridLineRepeater

                        model: week_view.rows * 2 + 1
                        delegate: Rectangle {
                            property bool is_hour: index % 2 === 0

                            x: is_hour ? -day_hours_column.width : 0
                            y: index < week_view.rows ?
                                   week_panel_item.__cellRectAt(index * week_view.columns).y :
                                   week_panel_item.__cellRectAt((week_view.rows - 1) *
                                                week_view.columns).y +
                                   week_panel_item.__cellRectAt((week_view.rows - 1) *
                                                week_view.columns).height
                            width: is_hour ?
                                       (week_view_inner_container.width +
                                        day_hours_column.width) :
                                       week_view_inner_container.width
                            height: __gridLineWidth
                            color: is_hour ? my_calendar_style.gridColor :
                                             Qt.lighter("lightgrey", 1.03)
                            visible: control.gridVisible
                        }
                    }
                } // week_container_view
            } // week_grid_view
        } // week_flickable

    } // week_panel_item

    Connections {
        target: control
        onRefreshEvents: {
            if (week_view.visible) {
                panelItem.clearLabelListModel();
                updateEventModel();
                createEventLabels();
            }
        }
    }

    function updateEventModel() {
        control.event_model.startDate = week_model.firstVisibleDate;
        control.event_model.endDate = week_model.lastVisibleDate;
        control.event_model.updateCollections();
        control.event_model.updateEvents();
    }

    function createEventLabels() {
        console.log("WeekView::createEventLabels");
        var events_of_day = [];
        var events_cross_day = [];
        var current_date = control.event_model.startDate;
        var date_index = 0;
        var events_in_a_day = [];

        if (control.event_model.events.length === 0) {
            return;
        }

        for (var i = 0; i < week_view.columns; ++i) {
            events_of_day.push(0);
        }

        console.log("Events length:", control.event_model.events.length);
        console.log("Range start date:", current_date.toLocaleString());
        for (i = 0; i < control.event_model.events.length; ++i) {
            var event = control.event_model.events[i];

            //            console.log("for loop, events:", event.displayLabel,
            //                        "current_date:", current_date.toLocaleString());

            var last_days = event_utils.lastDays(
                        event.startDateTime, event.endDateTime);
            if (last_days > 1 || event.allDay) {
                events_cross_day.push(event);
            } else {
                //                console.log("in a day events:", event.displayLabel,
                //                            "current_date:", current_date.toLocaleString());

                console.log("event.startDateTime:", event.startDateTime.getDate(),
                            "current_date:", current_date.getDate());

                if (event.startDateTime.getDate() !== current_date.getDate()) {
                    events_of_day[date_index] = events_in_a_day;

                    var days = event_utils.lastDays(current_date, event.startDateTime) - 1;
                    console.log("days: ", days);
                    date_index += days;
                    current_date.setDate(current_date.getDate() + days);
                    events_in_a_day = [];
                }

                events_in_a_day.push(event);
                console.log("events_in_a_day, push:", event.displayLabel);
            }
        }
        events_of_day[date_index] = events_in_a_day;

        for (i = 0; i < week_view.columns; ++i) {
            var array = events_of_day[i];
            if (array === 0) {
                continue;
            }

            var events_count_of_cell = [];
            for (var j = 0; j < 24 * 2; ++j) {
                events_count_of_cell.push(0);
            }
            var events_exists_of_cell = [];
            for (j = 0; j < 24 * 2; ++j) {
                events_exists_of_cell.push(0);
            }

            console.log("week: ", i, "events count:", array.length);
            for (var event_index = 0; event_index < array.length; ++event_index) {

                var start_cell_index =
                        array[event_index].startDateTime.getHours() * 2 +
                        array[event_index].startDateTime.getMinutes() / 30;
                var end_cell_index =
                        array[event_index].endDateTime.getHours() * 2 +
                        (array[event_index].endDateTime.getMinutes() - 1) / 30;
                //                        array[event_index].endDateTime.getMinutes() / 30;

                for (j = start_cell_index; j <= end_cell_index; ++j) {
                    ++events_count_of_cell[j];
                }
            }

            var component = Qt.createComponent("../WeekEventLabel.qml");
            for (event_index = 0; event_index < array.length; ++event_index) {
                start_cell_index =
                        array[event_index].startDateTime.getHours() * 2 +
                        array[event_index].startDateTime.getMinutes() / 30;
                end_cell_index =
                        array[event_index].endDateTime.getHours() * 2 +
                        (array[event_index].endDateTime.getMinutes() - 1) / 30;
                //                        array[event_index].endDateTime.getMinutes() / 30;

                var max_stack = 0;
                for (j = start_cell_index; j <= end_cell_index; ++j) {
                    if (events_count_of_cell[j] > max_stack) {
                        max_stack = events_count_of_cell[j];
                    }
                }

                var stack = events_exists_of_cell[start_cell_index] + 1;

                // create label object
                console.log("Event:", array[event_index].displayLabel,
                            array[event_index].startDateTime.toLocaleString(),
                            "stack_index:", stack, max_stack);

//                var rect = week_panel_item.__cellRectAt(
//                            start_cell_index * week_view.columns + i);

                var properties = {
                    "eventItem": array[event_index],
                    "date_index": i,
                    "stack_index": stack,
                    "max_stack": max_stack,
                    "start_cell_index": start_cell_index,
                    "end_cell_index": end_cell_index };
//                    "base_rect": rect,
//                    "ranged_cells": (end_cell_index - start_cell_index + 1)};
                CreateObject.createInComponent(
                            component, week_view_inner_container, properties,
                            labelListModelAddItem);

                for (j = start_cell_index; j <= end_cell_index; ++j) {
                    ++events_exists_of_cell[j];
                }
            }
        }
    }
}

