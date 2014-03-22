import QtQuick 2.1
import QtQuick.Controls 1.0
import MyCalendar.Controls.Private 1.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import MyCalendar.Weeks 1.0

Item {
    id: week_view

    readonly property int rows: CalendarUtils.hoursInADay * 2
    readonly property int columns: CalendarUtils.daysInAWeek

//    readonly property int total_cells: rows * columns

    readonly property int max_week_list_count: 3
    readonly property int middle_index_of_week_list: (max_week_list_count - 1) / 2

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
                week_list_model.refresh();
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
        readonly property color dateTextColor: Qt.darker("darkgray", 3.0)

        readonly property color selectedDateColor: Qt.darker("darkgray", 1.4)
        readonly property color selectedDateTextColor: "white"
        readonly property color invalidDateColor: "#dddddd"

        color: {
            var the_color = invalidDateColor;
            if (styleData.valid) {
                the_color = base_color
                if (styleData.today) {
                    the_color = selectedDateColor;
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
                var theColor = invalidDateColor;
                if (styleData.valid) {
                    theColor = dateTextColor;
                    if (styleData.today) {
                        theColor = selectedDateTextColor;
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
                    week_list_model.insert(i, { "week_date": date} );
                    console.log("week_list_model, append: ", date.toLocaleDateString());
                    date.setDate(date.getDate() + columns);
                }
                console.log("week_list_model, count:", week_list_model.count);

                week_list_view.currentIndex = middle_index_of_week_list;
//                week_list_view.positionViewAtIndex(middle_index_of_week_list,
//                                                    ListView.SnapPosition);
            }

            onInsertAtBeginning: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_week_list; ++i) {
                    date.setDate(date.getDate() - columns);
                    week_list_model.insert(0, { "week_date": date});
                    console.log("OnInsertAtBeingning Insert Month date: ",
                                date.toLocaleDateString());

                    week_list_model.remove(max_week_list_count);
                    console.log("Delete Month date of index: ", max_week_list_count);
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
            }

            onInsertAtEnd: {
                console.log("WeekView, onInsertAtEnd");
                console.log("control.visible_date:", control.visible_date);
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_week_list; ++i) {
                    date.setDate(date.getDate() + columns);
                    week_list_model.append({"week_date": date});
                    console.log("onInsertAtEnd Insert Month date: ",
                                date.toLocaleDateString());
                    week_list_model.remove(0);
                    console.log("Delete Month date of index: 0");
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
        console.log("showPreviousWeek, index:", index);
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
//    Item {
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
                                            (week_view.rows),
                                            week_delegate.availableWidth,
                                            week_delegate.availableHeight);
        }

        Item {
            id: day_header_row
            width: parent.width - week_delegate.hour_column_width
            height: panelItem.dayOfWeekHeaderRowHeight
            anchors.left: parent.left
            anchors.leftMargin: week_delegate.hour_column_width + __gridLineWidth
            anchors.top: parent.top

            Row {
                spacing: (control.gridVisible ? __gridLineWidth : 0)

                Repeater {
                    id: day_header_repeater
                    model: MyWeekModel {
                        visibleDate: week_date
                    }
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

                        model: week_view.rows * 2 + 1
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
                            visible: control.gridVisible
                        }
                    }

//                    MouseArea {
//                        id: mouseArea
//                        anchors.fill: parent

//                        hoverEnabled: true

//                        function cellIndexAt(mouseX, mouseY) {
//                            var viewContainerPos = parent.mapFromItem(
//                                        mouseArea, mouseX, mouseY);
//                            var child = parent.childAt(viewContainerPos.x,
//                                                       viewContainerPos.y);
//                            return child && child !== mouseArea ? child.__index : -1;
//                        }

//                        onPressed: {
//                            var indexOfCell = cellIndexAt(mouseX, mouseY);
//                            if (indexOfCell !== -1) {
//                                var date = view.model.dateAt(indexOfCell);
//                                //                    pressedCellIndex = indexOfCell;
//                                if (__isValidDate(date)) {
//                                    //                        control.selectedDate = date;
//                                    //                        control.pressed(date);
//                                }
//                            }
//                        }

//                        onReleased: {
//                            var indexOfCell = cellIndexAt(mouseX, mouseY);
//                            if (indexOfCell !== -1) {
//                                var date = view.model.dateAt(indexOfCell);
//                                if (__isValidDate(date)) {
//                                    //                        control.released(date);
//                                }
//                            }
//                            pressedCellIndex = -1;
//                        }

//                        onWheel: {
//                            if (wheel.angleDelta.y > 0) {
//                                showPreviousMonth();
//                            } else if (wheel.angleDelta.y < 0) {
//                                showNextMonth();
//                            }
//                        }

//                        onClicked: {
//                            console.log("Mouse position: " + mouse.x + ", " + mouse.y);
//                            var indexOfCell = cellIndexAt(mouseX, mouseY);
//                            console.log("IndexOfCell: ", indexOfCell);
//                            if (indexOfCell !== -1) {
//                                var date = view.model.dateAt(indexOfCell);
//                                console.log("Date at this point: ", date.toLocaleDateString());
//                                panelItem.hoveredCellIndex = indexOfCell;
//                                if (__isValidDate(date)) {
//                                    //                        control.clicked(date);
//                                    var global_pos = week_central_view.mapToItem(null, mouseX, mouseY);

//                                    var show_pos_x = MonthViewUtils.getEditViewPosX(global_pos.x);
//                                    var show_pos_y = MonthViewUtils.getEditViewPosY(
//                                                global_pos.y, indexOfCell);
//                                    console.log("Shown Pos: " + show_pos_x + ", " + show_pos_y);
//                                    float_edit.x = show_pos_x;
//                                    float_edit.y = show_pos_y;
//                                    float_edit.showAdd(date);
//                                }
//                            }
//                        }

//                        onDoubleClicked: {
//                            var indexOfCell = cellIndexAt(mouseX, mouseY);
//                            if (indexOfCell !== -1) {
//                                var date = view.model.dateAt(indexOfCell);
//                                if (__isValidDate(date)) {
//                                    //                        control.doubleClicked(date);
//                                }
//                            }
//                        }
//                    }
                } // week_container_view
            } // week_grid_view
        } // week_flickable
    } // week_delegate

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

                for (j = start_cell_index; j <= end_cell_index; ++j) {
                    ++events_exists_of_cell[j];
                }
            }
        }
    }
}
