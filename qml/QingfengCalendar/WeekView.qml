import QtQuick 2.1
import QtQuick.Controls 1.0
import MyCalendar.Controls.Private 1.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import MyCalendar.Weeks 1.0
import "CreateObject.js" as CreateObject
import "EventJsUtils.js" as EventJs

Item {
    id: week_view

    readonly property int rows: CalendarUtils.hoursInADay * 2
    readonly property int columns: CalendarUtils.daysInAWeek

    readonly property int max_week_list_count: 3
    readonly property int middle_index_of_week_list: (max_week_list_count - 1) / 2

    property int max_shown_all_day_events: 3

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
        //title += Qt.formatDateTime(range_start_date, format);
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
                week_list_model.refresh();
                control.refreshEvents()
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
        color: my_calendar_style.base_color
        Label {
            text: {
                var the_time = new Date();
                the_time.setHours(styleData.index, 0);
                var the_text = Qt.formatTime(the_time, "hh:mm")
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

        ListModel {
            id: week_list_model

            signal refresh()
            signal insertAtBeginning()
            signal insertAtEnd()

            Component.onCompleted: refresh()

            onRefresh: {
                console.log("Week List Model refreshed.")

                for (var i = 0; i < week_list_model.count; ++i) {
                    week_list_model.remove(i);
                }
                week_list_model.clear();

                var date = control.visible_date;
                date.setDate(date.getDate() - middle_index_of_week_list * columns);

                for (i = 0; i < max_week_list_count; ++i) {
                    console.log("week_list_model, append: ", date.toLocaleDateString());

                    week_list_model.insert(i, { "week_date": date} );
                    date.setDate(date.getDate() + columns);
                }

                week_list_view.currentIndex = middle_index_of_week_list;
//                week_list_view.positionViewAtIndex(middle_index_of_week_list,
//                                                    ListView.SnapPosition);
            }

            onInsertAtBeginning: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_week_list; ++i) {
                    date.setDate(date.getDate() - columns);
                    week_list_model.insert(0, { "week_date": date});
                    week_list_model.remove(max_week_list_count);

                    console.log("OnInsertAtBeingning Insert Week date: ",
                                date.toLocaleDateString());
                    console.log("Delete Week date of index: ", max_week_list_count);
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
            }

            onInsertAtEnd: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_week_list; ++i) {
                    date.setDate(date.getDate() + columns);
                    week_list_model.append({"week_date": date});
                    week_list_model.remove(0);

                    console.log("onInsertAtEnd Insert Week date: ",
                                date.toLocaleDateString());
                    console.log("Delete Week date of index: 0");
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
            }
        } // week_list_model

        ListView {
            id: week_list_view
            anchors.fill: parent

            model: week_list_model
            delegate: week_delegate
            clip: true
            interactive: true
            focus: true
            cacheBuffer: 3000
            snapMode: ListView.SnapOneItem
            orientation: ListView.Horizontal
            currentIndex: middle_index_of_week_list
            highlightFollowsCurrentItem: true
            highlightMoveVelocity: 5000
            highlightRangeMode: ListView.StrictlyEnforceRange
            boundsBehavior: Flickable.DragOverBounds

            onDragStarted: panelItem.hideEventLabels();
        } // month_list_view
    } // week_panel_item

    Timer {
        id: insert_at_begin_timer
        interval: 500
        onTriggered: {
            week_list_model.insertAtBeginning();
        }
    }

    Timer {
        id: insert_at_end_timer
        interval: 500
        onTriggered: {
            week_list_model.insertAtEnd();
        }
    }

    Timer {
        id: refresh_events_timer
        interval: 550
        onTriggered: {
            control.refreshEvents();
        }
    }

    Connections {
        target: week_list_view

        onMovementEnded: {
            var index = week_list_view.currentIndex;

            console.log("Week_list_view, onFlickEnded.");
            console.log("Index: ", index);

            control.visible_date = week_list_model.get(index).week_date;
            console.log("Control.visible_date:", control.visible_date);

            if (index === 0) {
                week_list_model.insertAtBeginning();
            } else if (index === max_week_list_count - 1) {
                week_list_model.insertAtEnd();
            }

            control.refreshEvents();
        }
    }

    function showPreviousWeek() {
        panelItem.hideEventLabels();
        week_list_view.decrementCurrentIndex();
        var index = week_list_view.currentIndex;
        control.visible_date = week_list_model.get(index).week_date;

        if (index === 0) {
            insert_at_begin_timer.start();
            refresh_events_timer.start();
        } else {
            control.refreshEvents();
        }
    }

    function showNextWeek() {
        panelItem.hideEventLabels();
        week_list_view.incrementCurrentIndex();
        var index = week_list_view.currentIndex;
        control.visible_date = week_list_model.get(index).week_date;

        if (index === max_week_list_count - 1) {
            insert_at_end_timer.start();
            refresh_events_timer.start();
        } else {
            control.refreshEvents();
        }
    }

    Connections {
        target: navigationBarLoader.item
        onShowPrevious: {
            if (week_view.visible) {
                showPreviousWeek();
            }
        }
    }
    Connections {
        target: navigationBarLoader.item
        onShowNext: {
            if (week_view.visible) {
                showNextWeek();
            }
        }
    }

    property Component week_delegate: Item {
        id: week_delegate
        width: week_panel_item.width
        height: week_panel_item.height

        readonly property real availableWidth:
            (week_view_inner_container.width - (control.gridVisible ? __gridLineWidth : 0))
        readonly property real availableHeight:
            (week_inner_row.height - (control.gridVisible ? __gridLineWidth : 0))

        readonly property real hour_column_width: 60

        function __cellRectAt(index) {
            return CalendarUtils.cellRectAt(index,
                                            week_view.columns,
                                            week_view.rows,
                                            week_delegate.availableWidth,
                                            week_delegate.availableHeight);
        }

        MyWeekModel {
            id: my_week_model_in_delegate
            visibleDate: week_date
        }

        Row {
            id: day_header_row
            width: parent.width - week_delegate.hour_column_width
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: week_delegate.hour_column_width + __gridLineWidth
            anchors.top: parent.top

            spacing: (control.gridVisible ? __gridLineWidth : 0)

            Repeater {
                id: day_header_repeater
                model: my_week_model_in_delegate
                Loader {
                    id: dayDelegateLoader
                    sourceComponent: dayDelegate
                    width: __cellRectAt(index).width -
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
        } // day_header_row

        Rectangle {
            id: cross_day_events_panel
            width: parent.width - week_delegate.hour_column_width
            height: 18
            anchors.left: parent.left
            anchors.leftMargin: week_delegate.hour_column_width
            anchors.top: day_header_row.bottom

            border.color: my_calendar_style.gridColor
            border.width: 1
            color: "transparent"

            Repeater {
                id: line_repeater
                model: week_view.columns - 1
                delegate: Rectangle {
                    x: __cellRectAt(index + 1).x
                    y: 0
                    width: __gridLineWidth
                    height: parent.height
                    color: my_calendar_style.gridColor
                }
            }

            Repeater {
                id: cross_day_events_repeater
                model: my_week_model_in_delegate
                delegate: Rectangle {
                    x: __cellRectAt(index).x +
                       (control.gridVisible ? __gridLineWidth : 0)
                    y: __gridLineWidth
                    width: __cellRectAt(index).width -
                           (control.gridVisible ? __gridLineWidth : 0)
                    height: parent.height - __gridLineWidth * 2

                    property bool is_today:
                        date.getTime() === new Date().setHours(0, 0, 0, 0)
                    property color today_color: Qt.darker("darkgray", 1.4)

                    color: {
                        var the_color = "transparent";
                            if (is_today) {
                                the_color = today_color;
                            }
                        the_color;
                    }
                }
            }
        }

        Rectangle {
            id: line

            anchors.top: cross_day_events_panel.bottom
            anchors.topMargin: 8
            width: parent.width
            height: 1
            z: 1

            color: my_calendar_style.gridColor
        }

        Flickable {
            id: week_flickable
            anchors.top: cross_day_events_panel.bottom
            anchors.topMargin: 8

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
                            y: __cellRectAt(index * week_view.columns * 2).y +
                               (control.gridVisible ? __gridLineWidth : 0)
                            width: day_hours_column.width
                            height: __cellRectAt(index * week_view.columns).height -
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
                                   __cellRectAt(index).x :
                                   __cellRectAt(week_view.columns - 1).x +
                                   __cellRectAt(week_view.columns - 1).width
                            y: 0
                            width: __gridLineWidth
                            height: week_view_inner_container.height
                            color: my_calendar_style.gridColor
                            visible: control.gridVisible
                        }
                    }

                    Repeater {
                        id: horizontalGridLineRepeater

                        model: week_view.rows * 2
                        delegate: Rectangle {
                            property bool is_hour: index % 2 === 0

                            x: is_hour ? -day_hours_column.width : 0
                            y: index < week_view.rows ?
                                   __cellRectAt(index * week_view.columns).y :
                                   __cellRectAt((week_view.rows - 1) *
                                                week_view.columns).y +
                                   __cellRectAt((week_view.rows - 1) *
                                                week_view.columns).height
                            width: is_hour ?
                                       (week_view_inner_container.width +
                                        day_hours_column.width) :
                                       week_view_inner_container.width
                            height: __gridLineWidth
                            color: is_hour ? my_calendar_style.gridColor :
                                             Qt.lighter("lightgrey", 1.03)
                        }
                    }
                } // week_container_view


            } // week_grid_view
        } // week_flickable

        Connections {
            target: control
            onRefreshEvents: {
                if (week_view.visible && week_delegate.ListView.isCurrentItem) {
                    console.log("WeekView, onRefreshEvents.")
                    panelItem.clearLabelListModel();
                    updateEventModel();
                    createEventLabels();
                }
            }
        }

        function updateEventModel() {
            console.log("WeekView::updateEventModel.");
            console.log("startDateTime:", my_week_model_in_delegate.firstVisibleDate);
            console.log("EndDateTime:", my_week_model_in_delegate.lastVisibleDate);

            control.event_model.startDate = my_week_model_in_delegate.firstVisibleDate;
            control.event_model.endDate = my_week_model_in_delegate.lastVisibleDate;
            control.event_model.updateCollections();
            control.event_model.updateEvents();
        }

        function createEventLabels() {
            console.log("WeekView::createEventLabels");

            //
            // in day events
            //
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

            for (i = 0; i < control.event_model.events.length; ++i) {
                var event = control.event_model.events[i];

                var last_days = event_utils.lastDays(
                            event.startDateTime, event.endDateTime);
                if (last_days > 1 || event.allDay) {
                    events_cross_day.push(event);
                } else {
                    if (event.startDateTime.getDate() !== current_date.getDate()) {
                        events_of_day[date_index] = events_in_a_day;

                        var days = event_utils.lastDays(current_date, event.startDateTime) - 1;
                        date_index += days;
                        current_date.setDate(current_date.getDate() + days);
                        events_in_a_day = [];
                    }

                    events_in_a_day.push(event);
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

                for (var event_index = 0; event_index < array.length; ++event_index) {

                    var start_cell_index =
                            array[event_index].startDateTime.getHours() * 2 +
                            array[event_index].startDateTime.getMinutes() / 30;
                    var end_cell_index =
                            array[event_index].endDateTime.getHours() * 2 +
                            (array[event_index].endDateTime.getMinutes() - 1) / 30;
                    //array[event_index].endDateTime.getMinutes() / 30;

                    for (j = start_cell_index; j <= end_cell_index; ++j) {
                        ++events_count_of_cell[j];
                    }
                }

                var component = Qt.createComponent("WeekEventLabel.qml");
                for (event_index = 0; event_index < array.length; ++event_index) {
                    start_cell_index =
                            array[event_index].startDateTime.getHours() * 2 +
                            array[event_index].startDateTime.getMinutes() / 30;
                    end_cell_index =
                            array[event_index].endDateTime.getHours() * 2 +
                            (array[event_index].endDateTime.getMinutes() - 1) / 30;
                    //array[event_index].endDateTime.getMinutes() / 30;

                    var max_stack = 0;
                    for (j = start_cell_index; j <= end_cell_index; ++j) {
                        if (events_count_of_cell[j] > max_stack) {
                            max_stack = events_count_of_cell[j];
                        }
                    }

                    var stack = events_exists_of_cell[start_cell_index] + 1;

                    // create label object
                    console.log("In day Event:", array[event_index].displayLabel,
                                array[event_index].startDateTime.toLocaleString(),
                                "stack_index:", stack, max_stack);

                    var properties = {
                        "eventItem": array[event_index],
                        "date_index": i,
                        "stack_index": stack,
                        "max_stack": max_stack,
                        "start_cell_index": start_cell_index,
                        "end_cell_index": end_cell_index };
                    CreateObject.createInComponent(
                                component, week_view_inner_container, properties,
                                panelItem.labelListModelAddItem);

                    for (j = start_cell_index; j <= end_cell_index; ++j) {
                        ++events_exists_of_cell[j];
                    }
                }

                component.destroy();
            }

            //
            // all day or multi days events
            //

            // events_info_array
            // properties:
            // @count: track of the events count of this day.
            // @show_order: it's an array of length: @max_shown_all_day_events,
            //  used for putting the event to the proper position.
            var events_info_array = [];
            for (i = 0; i < week_view.columns; ++i) {
                events_info_array.push(
                            EventJs.EventsInfo.createNew(max_shown_all_day_events));
            }

            component = Qt.createComponent("WeekAllDayEventLabel.qml")
            for (i = 0; i < events_cross_day.length; ++i) {
                // represent the event that pass from C++
                var cross_day_event = events_cross_day[i];

                date_index = event_utils.daysTo(
                            my_week_model_in_delegate.firstVisibleDate,
                            cross_day_event.startDateTime);
                console.log("date_index", date_index);

                // TODO: I set the firstVisibleDate to 2013-12-02, and it gave me
                // all day events that start at 2013-12-01, end at 2013-12-02, it
                // confused me.
                if (date_index < 0)
                    continue;

                // got already existed events count of this day
                var day_event_count = events_info_array[date_index].count;
                // TODO: this should be improved. If there are too much events,
                // then they should be stored and shown in a popup view once
                // the mark is clicked.
                if (day_event_count >= week_view.max_shown_all_day_events) {
                    events_info_array[date_index].count += 1;
                    continue;
                }

                // arrange the shown order of this event. just put it to the
                // first blank place, that's ok.
                var show_order = 0;
                for (j = 0; j < max_shown_all_day_events; ++j) {
                    if (events_info_array[date_index].show_order[j] === 0) {
                        show_order = j;
                        break;
                    }
                }

                // got the ranged days of this event.
                var range_days = event_utils.lastDays(
                            cross_day_event.startDateTime, cross_day_event.endDateTime);
                if (range_days === 0 && cross_day_event.allDay) {
                    range_days = 1;
                }

                // If the event lasts outside this range, just clip it.
                if (date_index + range_days > week_view.columns) {
                    range_days -= (date_index + range_days - week_view.columns);
                }

                console.log("Cross day Event:", cross_day_event.displayLabel,
                            cross_day_event.startDateTime.toLocaleString(),
                            "stack_index:", date_index, range_days);

                properties = {
                    "eventItem": cross_day_event,
                    "show_order_in_a_day": show_order,
                    "date_index": date_index,
                    "range_days": range_days};
                CreateObject.createInComponent(component, cross_day_events_panel,
                                               properties, labelListModelAddItem);

                // after createObject, the events_info_array should be refreshed.
                for (j = 0; j < range_days; ++j) {
                    events_info_array[date_index + j].increaseInfo(show_order);
                }
            }
            component.destroy();

            // After created, the height of cross_day_events_panel should be changed.
            var max_events_count = 0;
            for (i = 0; i < week_view.columns; ++i) {
                if (events_info_array[i].count > max_events_count) {
                    max_events_count = events_info_array[i].count;
                }
            }
            max_events_count = Math.min(max_events_count, max_shown_all_day_events);
            cross_day_events_panel.height = 16 + 20 * max_events_count;
        }
    } // week_delegate
}
