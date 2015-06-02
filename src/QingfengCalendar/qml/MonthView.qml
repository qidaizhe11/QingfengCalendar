import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
//import MyCalendar.Controls.Private 1.0
//import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import "MonthViewUtils.js" as MonthViewUtils
import "CreateObject.js" as CreateObject
import "EventJsUtils.js" as EventJs

Item {
    id: month_view

    readonly property int rows: CalendarUtils.weeksOnCalendarMonth
    readonly property int columns: CalendarUtils.daysInAWeek
    readonly property int total_cells: rows * columns

    readonly property int max_show_events_of_day: 2

    readonly property int max_month_list_count: 5
    readonly property int middle_index_of_month_list: (max_month_list_count - 1) / 2

    readonly property real availableWidth:
        (viewContainer.width - (control.gridVisible ? __gridLineWidth : 0))
    readonly property real availableHeight:
        (viewContainer.height - (control.gridVisible ? __gridLineWidth : 0))

    property string calendar_title: {
        (new Date(control.visible_date.getFullYear(),
                  control.visible_date.getMonth(),
                  1).toLocaleDateString(Qt.locale(), "yyyy年")) +
                Qt.locale().standaloneMonthName(control.visible_date.getMonth());
    }

    // Copied from QtQuick.Controls.Calenadr, serviced as MonthModel here.
    // holes the model that will be used by the Calendar to populate month date
    // that available to the user.
    property CalendarModel month_model: CalendarModel {
        visibleDate: control.visible_date
    }

    Connections {
        target: tab_view
        onCurrentIndexChanged: {
            if (month_view.visible) {
                month_list_model.refresh()
                control.refreshEvents()
            }
        }
    }

    Binding {
        when: month_view.visible
        target: navigationBarLoader
        property: "styleData.title"
        value: calendar_title
    }

    function __cellRectAt(index) {
        return CalendarUtils.cellRectAt(index,
                                        month_view.columns,
                                        month_view.rows,
                                        month_view.availableWidth,
                                        month_view.availableHeight);
    }

    property Component dayDelegate: Rectangle {
        readonly property color base_color: "lightgray"

        readonly property color selected_base_color: Qt.darker("darkgray", 1.4)
        readonly property color sameMonthDateTextColor: Qt.darker("darkgray", 3.0)
        readonly property color different_month_date_color:
            Qt.darker("lightgray", 1.05)
        readonly property color selected_text_color: "white"
        readonly property color differentMonthDateTextColor:
            Qt.darker("darkgray", 1.4)

        readonly property color invalid_color: "#dddddd"

        color: {
            var the_color = invalid_color;
            if (styleData.valid) {
                the_color = styleData.visibleMonth ? base_color :
                                                     different_month_date_color;
                if (styleData.today) {
                    the_color = selected_base_color;
                }
                if (styleData.hovered) {
                    the_color = Qt.darker(the_color, 1.1);
                }
            }

            the_color;
        }

        Label {
            id: dayDelegateText
            text: styleData.date.getDate()
            font.pointSize: 12
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.05
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            horizontalAlignment: Text.AlignRight
            color: {
                var theColor = invalid_color;
                if (styleData.valid) {
                    theColor = styleData.visibleMonth ? sameMonthDateTextColor :
                                                        differentMonthDateTextColor;
                    if (styleData.today) {
                        theColor = selected_text_color;
                    }
                }
                theColor;
            }
        }
    } // dayDelegete

    property Component dayOfWeekDelegate: Rectangle {
        color: my_calendar_style.base_color
        Label {
            text: Qt.locale().dayName(styleData.dayOfWeek,
                                           control.dayOfWeekFormat)
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id: dayOfWeekHeaderRow
        width: panelItem.width
        height: panelItem.dayOfWeekHeaderRowHeight

        Row {
            spacing: (control.gridVisible ? __gridLineWidth : 0)

            Repeater {
                id: repeater
                model: CalendarHeaderModel {
                    locale: Qt.locale()
                }
                Loader {
                    id: dayOfWeekDelegateLoader
                    sourceComponent: dayOfWeekDelegate
                    width: __cellRectAt(index).width -
                           (control.gridVisible ? __gridLineWidth : 0)
                    height: dayOfWeekHeaderRow.height

                    readonly property var __dayOfWeek: dayOfWeek

                    property QtObject styleData: QtObject {
                        readonly property alias dayOfWeek:
                            dayOfWeekDelegateLoader.__dayOfWeek
                    }
                }
            }
        }
    } // day_of_week_header_row

    Item {
        id: viewContainer
        width: panelItem.width
        height: panelItem.height - navigationBarLoader.height -
                dayOfWeekHeaderRow.height
        anchors.top: dayOfWeekHeaderRow.bottom

        ListModel {
            id: month_list_model

            signal refresh()
            signal insertAtBeginning()
            signal insertAtEnd()

            Component.onCompleted: refresh()

            onRefresh: {
                console.log("Month List Model refreshed.")

                for (var i = 0; i < month_list_model.count; ++i) {
                    month_list_model.remove(i);
                }
                month_list_model.clear();

                var date = control.visible_date;
                date.setMonth(date.getMonth() - middle_index_of_month_list);

                for (i = 0; i < max_month_list_count; ++i) {
                    month_list_model.append({ "month_date": date} );
                    date.setMonth(date.getMonth() + 1);

                    console.log("month_list_model, append: ", date.toLocaleDateString());
                }

                month_list_view.currentIndex = middle_index_of_month_list;
            }

            onInsertAtBeginning: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_month_list; ++i) {
                    date.setMonth(date.getMonth() - 1);
                    month_list_model.insert(0, { "month_date": date});
                    month_list_model.remove(max_month_list_count);

                    console.log("OnInsertAtBeingning Insert Month date: ",
                                date.toLocaleDateString());
                    console.log("Delete Month date of index: ", max_month_list_count);
                }

                month_list_view.positionViewAtIndex(middle_index_of_month_list,
                                                    ListView.SnapPosition);
            }

            onInsertAtEnd: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_month_list; ++i) {
                    date.setMonth(date.getMonth() + 1);
                    month_list_model.append({"month_date": date});
                    month_list_model.remove(0);

                    console.log("onInsertAtEnd Insert Month date: ",
                                date.toLocaleDateString());
                    console.log("Delete Month date of index: 0");
                }

                month_list_view.positionViewAtIndex(middle_index_of_month_list,
                                                    ListView.SnapPosition);
            }
        } // month_list_model

        ListView {
            id: month_list_view
            anchors.fill: parent

            model: month_list_model
            delegate: month_delegate
            clip: true
            interactive: true
            focus: true
            cacheBuffer: 3000
            snapMode: ListView.SnapOneItem
            orientation: ListView.Vertical
            currentIndex: middle_index_of_month_list
            highlightFollowsCurrentItem: true
            highlightMoveVelocity: 1600
            highlightRangeMode: ListView.StrictlyEnforceRange
            boundsBehavior: Flickable.DragOverBounds

            onDragStarted: panelItem.hideEventLabels();
        } // month_list_view
    } // view_container

    Timer {
        id: insert_at_begin_timer
        interval: 500
        onTriggered: {
            month_list_model.insertAtBeginning();
        }
    }

    Timer {
        id: insert_at_end_timer
        interval: 500
        onTriggered: {
            month_list_model.insertAtEnd();
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
        target: month_list_view

        onMovementEnded: {
            var index = month_list_view.currentIndex;
            control.visible_date = month_list_model.get(index).month_date;

            if (index === 0) {
                month_list_model.insertAtBeginning();
            } else if (index === max_month_list_count - 1) {
                month_list_model.insertAtEnd();
            }

            control.refreshEvents();
        }
    }

    function showPreviousMonth() {
        panelItem.hideEventLabels();
        month_list_view.decrementCurrentIndex();
        var index = month_list_view.currentIndex;
        control.visible_date = month_list_model.get(index).month_date;

        if (index === 0) {
            insert_at_begin_timer.start();
            refresh_events_timer.start();
        } else {
            control.refreshEvents();
        }
    }

    function showNextMonth() {
        panelItem.hideEventLabels();
        month_list_view.incrementCurrentIndex();
        var index = month_list_view.currentIndex;
        control.visible_date = month_list_model.get(index).month_date;

        if (index === max_month_list_count - 1) {
            insert_at_end_timer.start();
            refresh_events_timer.start();
        } else {
            control.refreshEvents();
        }
    }

    Connections {
        target: navigationBarLoader.item
        onShowPrevious: {
            if (month_view.visible) {
                showPreviousMonth();
            }
        }
    }
    Connections {
        target: navigationBarLoader.item
        onShowNext: {
            if (month_view.visible) {
                showNextMonth();
            }
        }
    }

    property Component month_delegate: Item {
//        id: month_delegate

        width: panelItem.width
        height: panelItem.height - navigationBarLoader.height -
                dayOfWeekHeaderRow.height

        Repeater {
            id: verticalGridLineRepeater
            model: month_view.columns - 1
            delegate: Rectangle {
                x: index < month_view.columns ?
                       __cellRectAt(index).x + __cellRectAt(index).width :
                       __cellRectAt(month_view.columns - 1).x +
                       __cellRectAt(month_view.columns - 1).width
                y: 0
                width: __gridLineWidth
                height: viewContainer.height
                color: gridColor
                visible: control.gridVisible
            }
        }

        Repeater {
            id: horizontalGridLineRepeater
            model: month_view.rows
            delegate: Rectangle {
                x: 0
                y: index < month_view.columns - 1 ?
                       __cellRectAt(index * month_view.columns).y :
                       __cellRectAt((month_view.rows - 1) *
                                    month_view.columns).y +
                       __cellRectAt((month_view.rows - 1) *
                                    month_view.columns).height
                width: viewContainer.width
                height: __gridLineWidth
                color: gridColor
                visible: control.gridVisible
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            hoverEnabled: true

            function cellIndexAt(mouseX, mouseY) {
                var viewContainerPos = parent.mapFromItem(
                            mouseArea, mouseX, mouseY);
                var child = parent.childAt(viewContainerPos.x,
                                           viewContainerPos.y);
                return child && child !== mouseArea ? child.__index : -1;
            }

            onPressed: {
                var indexOfCell = cellIndexAt(mouseX, mouseY);
                if (indexOfCell !== -1) {
                    var date = view.model.dateAt(indexOfCell);
                    if (__isValidDate(date)) {
                        //control.pressed(date);
                    }
                }
            }

            onReleased: {
                var indexOfCell = cellIndexAt(mouseX, mouseY);
                if (indexOfCell !== -1) {
                    var date = view.model.dateAt(indexOfCell);
                    if (__isValidDate(date)) {
                        //control.released(date);
                    }
                }
                pressedCellIndex = -1;
            }

            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    showPreviousMonth();
                } else if (wheel.angleDelta.y < 0) {
                    showNextMonth();
                }
            }

            onClicked: {
                console.log("Mouse position: " + mouse.x + ", " + mouse.y);
                var indexOfCell = cellIndexAt(mouseX, mouseY);
                console.log("IndexOfCell: ", indexOfCell);
                if (indexOfCell !== -1) {
                    var date = view.model.dateAt(indexOfCell);
                    console.log("Date at this point: ", date.toLocaleDateString());
                    panelItem.hoveredCellIndex = indexOfCell;
                    if (__isValidDate(date)) {
                        //control.clicked(date);
                        var global_pos = viewContainer.mapToItem(null, mouseX, mouseY);

                        var show_pos_x = MonthViewUtils.getEditViewPosX(global_pos.x);
                        var show_pos_y = MonthViewUtils.getEditViewPosY(
                                    global_pos.y, indexOfCell);

                        float_edit.x = show_pos_x;
                        float_edit.y = show_pos_y;
                        float_edit.showAdd(date);
                    }
                }
            }

            onDoubleClicked: {
                var indexOfCell = cellIndexAt(mouseX, mouseY);
                if (indexOfCell !== -1) {
                    var date = view.model.dateAt(indexOfCell);
                    if (__isValidDate(date)) {
                        //control.doubleClicked(date);
                    }
                }
            }
        }

        Repeater {
            id: view

            property int currentIndex: -1

            model: CalendarModel {
                visibleDate: month_date
            }

            Component.onCompleted: selectedDateChanged()

            function selectedDateChanged() {
                if (model !== undefined && model.locale !== undefined) {
                    currentIndex = model.indexAt(control.selectedDate);
                }
            }

            delegate: Loader {
                id: delegateLoader

                x: __cellRectAt(index).x +
                   (control.gridVisible ? __gridLineWidth : 0)
                y: __cellRectAt(index).y +
                   (control.gridVisible ? __gridLineWidth : 0)
                width: __cellRectAt(index).width -
                       (control.gridVisible ? __gridLineWidth : 0)
                height: __cellRectAt(index).height -
                        (control.gridVisible ? __gridLineWidth : 0)

                sourceComponent: dayDelegate

                readonly property int __index: index
                readonly property date __date: date

                readonly property bool valid: __isValidDate(date)

                property QtObject styleData: QtObject {
                    readonly property alias index:
                        delegateLoader.__index
                    readonly property alias date: delegateLoader.__date
                    readonly property bool valid: delegateLoader.valid
                    // TODO: this will not be correct if the app is
                    // running when a new day begins.
                    readonly property bool today: date.getTime() ===
                                                  new Date().setHours(0, 0, 0, 0)
                    readonly property bool visibleMonth:
                        date.getMonth() === month_date.getMonth()
                    readonly property bool hovered:
                        panelItem.hoveredCellIndex == index
                    readonly property bool pressed:
                        panelItem.pressedCellIndex == index
                    // TODO: pressed property here, clicked and
                    // doubleClicked in the control itself
                }
            } // delegate_loader
        } // view Repeater
    } // month_delegate

    Connections {
        target: control
        onRefreshEvents: {
            if (month_view.visible) {
                console.log("MonthView, onRefreshEvents.")

                console.log("OnRefreshEvents, label_list_model count: ",
                            label_list_model.count);

                panelItem.clearLabelListModel();
                updateEventModel();
                createEventLabels();
            }
        } // onRefreshEvents
    } // connections

    property var events: []

    function updateEventModel() {
        for (var i = 0; i < events.length; ++i) {
            delete events[i];
        }

        events = control.event_model.itemsByTimePeriod(month_model.firstVisibleDate,
                                                      month_model.lastVisibleDate);
    }

    function createEventLabels() {
        if (!month_view.visible)
            return;

        var col = month_view.columns;

        // events_info_array
        // properties:
        // @count: track of the events count of this day.
        // @show_order: it's an array of length: @max_shown_all_day_events,
        //  used for putting the event to the proper position.
        var events_info_array = [];
        for (var i = 0; i < month_view.total_cells; ++i) {
            events_info_array.push(
                        EventJs.EventsInfo.createNew(max_show_events_of_day));
        }

        var component = Qt.createComponent("MonthEventLabel.qml");

        console.log("events.length:", events.length);
        for (i = 0; i < events.length; ++i) {
            // represent the event that pass from C++
            var event = events[i];

            var index_of_cell = event_utils.daysTo(
                        month_model.firstVisibleDate, event.startDateTime);

            // TODO: this should be improved. If there are too much events,
            // then they should be stored and shown in a popup view once
            // the mark is clicked.
            var day_event_count = events_info_array[index_of_cell].count;
            if (day_event_count >= month_view.max_show_events_of_day) {
                ++events_info_array[index_of_cell].count;
                continue;
            }

            // arrange the shown order of this event. just put it to the
            // first blank place, that's ok.
            var show_order_in_a_day = 0;
            for (var j = 0; j < max_show_events_of_day; ++j) {
                if (events_info_array[index_of_cell].show_order[j] === 0) {
                    show_order_in_a_day = j;
                    break;
                }
            }

            // got the ranged days of this event.
            var last_days = event_utils.lastDays(
                        event.startDateTime, event.endDateTime);
            if (last_days === 0 && event.allDay) {
                last_days = 1;
            }

            // If the event lasts outside this range, just clip it.
            if (index_of_cell + last_days > total_cells) {
                last_days -= (index_of_cell + last_days - total_cells);
            }

            // after createObject, the events_info_array should be refreshed.
            for (j = 0; j < last_days; ++j) {
                events_info_array[index_of_cell + j].increaseInfo(show_order_in_a_day);
            }

            var properties;
            // If the event lasts more than 7 days within this month view range,
            // clip it to multi labels.
            // Note that they are in fact still only one, all appear or all disappear.
            while (index_of_cell % col + last_days > col) {
                var clipped_days = col - index_of_cell % col;

                properties = {
                    "eventItem": event,
                    "show_order_in_a_day": show_order_in_a_day,
                    "grid_index": index_of_cell,
                    "last_days": clipped_days};
                CreateObject.createInComponent(
                            component, viewContainer, properties,
                            labelListModelAddItem);

                index_of_cell += clipped_days;
                last_days -= clipped_days;
            }

            console.log("Create Event:", event.displayLabel,
                        event.startDateTime.toLocaleString(),
                        event.endDateTime.toLocaleString());

            properties = {"eventItem": event, "show_order_in_a_day": show_order_in_a_day,
                "grid_index": index_of_cell, "last_days": last_days};
            CreateObject.createInComponent(
                        component, viewContainer, properties, labelListModelAddItem);
        } // for

        component.destroy();
    }
}
