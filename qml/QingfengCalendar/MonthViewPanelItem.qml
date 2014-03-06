import QtQuick 2.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import "EventJsUtils.js" as EventJsUtils
import "CreateObject.js" as CreateObject

Item {
    id: panelItem
    implicitHeight: 200
    implicitWidth: 200

    property alias navigationBarItem: navigationBarLoader.item

    readonly property real dayOfWeekHeaderRowHeight: 30

    readonly property int weeksToshow: CalendarUtils.weeksOnCalendarMonth
    readonly property int rows: weeksToshow
    readonly property int columns: CalendarUtils.daysInWeek

    readonly property int total_cells: rows * columns

    readonly property int max_show_events_of_day: 2

    readonly property real availableWidth:
        (viewContainer.width - (control.gridVisible ? __gridLineWidth : 0))
    readonly property real availableHeight:
        (viewContainer.height - (control.gridVisible ? __gridLineWidth : 0))

    property int hoveredCellIndex: -1
    property int pressedCellIndex: -1

    Loader {
        id: backgroundLoader
        anchors.fill: parent
        sourceComponent: background

        MouseArea {
            id: background_mouseArea
            anchors.fill: parent
        }
    }

    Loader {
        id: navigationBarLoader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        sourceComponent: navigationBar

        property QtObject styleData: QtObject {
            readonly property string title:
//                control.__locale.standaloneMonthName(control.visibleMonth) +
//                new Date(control.visibleYear, control.visibleMonth, 1). toLocaleDateString(control.__locale, " yyyy")
//            Qt.locale().standaloneMonthName(control.visibleMonth) +
//            new Date(control.visibleYear, control.visibleMonth, 1).
//            toLocaleDateString(Qt.locale(), " yyyy")
                (new Date(control.visibleYear, control.visibleMonth, 1).toLocaleDateString(Qt.locale(), "yyyy  ")) +
                Qt.locale().standaloneMonthName(control.visibleMonth)
        }

        MouseArea {
            id: navigationbar_mouseArea
            anchors.fill: parent
        }
    }

//    Loader {
//        id: event_editor_loader
//        sourceComponent: float_event_edit
//    }

    Row {
        id: dayOfWeekHeaderRow
        spacing: (control.gridVisible ? __gridLineWidth : 0)
        anchors.top: navigationBarLoader.bottom
        anchors.left: parent.left
        anchors.leftMargin: (control.weekNumbersVisible ?
                                 weekNumbersItem.width : 0) +
                            (control.gridVisible ? __gridLineWidth : 0)
        anchors.right: parent.right
        height: dayOfWeekHeaderRowHeight

        Repeater {
            id: repeater
            model: CalendarHeaderModel {
//                locale: control.__locale
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

    Row {
        id: gridRow
        width: weekNumbersItem.width + viewContainer.width
        height: viewContainer.height
        anchors.top: dayOfWeekHeaderRow.bottom

        Item {
            id: weekNumbersItem
            visible: control.weekNumbersVisible
            width: 30
            height: viewContainer.height

            Repeater {
                id: weekNumberRepeater
                model: panelItem.weeksToshow

                Loader {
                    id: weekNumberDelegateLoader
                    y: __cellRectAt(index * panelItem.columns).y +
                       (control.gridVisible ? __gridLineWidth : 0)
                    width: weekNumbersItem.width
                    height: __cellRectAt(index * panelItem.columns).height -
                            (control.gridVisible ? __gridLineWidth : 0)
                    sourceComponent: weekNumberDelegate

                    readonly property int __index: index
                    property int __weekNumber:
                        control.__model.weekNumberAt(index)

                    Connections {
                        target: control
                        onVisibleMonthChanged: __weekNumber =
                                               control.__model.weekNumberAt(index)
                        onVisibleYearChanged: __weekNumber =
                                              control.__model.weekNumberAt(index)
                    }

                    Connections {
                        target: control.__model
                        onCountChanged: __weekNumber =
                                        control.__model.weekNumberAt(index)
                    }

                    property QtObject styleData: QtObject {
                        readonly property alias index:
                            weekNumberDelegateLoader.__index
                        readonly property int weekNumber:
                            weekNumberDelegateLoader.__weekNumber
                    }
                }
            }
        }

        Item {
            id: viewContainer
            width: panelItem.width - (control.weekNumbersVisible ?
                                          weekNumbersItem.width : 0)
            height: panelItem.height - navigationBarLoader.height -
                    dayOfWeekHeaderRow.height

            Repeater {
                id: verticalGridLineRepeater
                model: panelItem.columns -1
                delegate: Rectangle {
                    x: index < panelItem.columns ?
                           __cellRectAt(index).x + __cellRectAt(index).width :
                           __cellRectAt(panelItem.columns - 1).x +
                           __cellRectAt(panelItem.columns - 1).width
                    y: -dayOfWeekHeaderRow.height + 5
                    width: __gridLineWidth
                    height: viewContainer.height + dayOfWeekHeaderRow.height
                    color: gridColor
                    visible: control.gridVisible
                }
            }

            Repeater {
                id: horizontalGridLineRepeater
                model: panelItem.rows
                delegate: Rectangle {
                    x: 0
                    y: index < panelItem.columns - 1 ?
                           __cellRectAt(index * panelItem.columns).y :
                           __cellRectAt((panelItem.rows - 1) *
                                        panelItem.columns).y +
                           __cellRectAt((panelItem.rows - 1) *
                                        panelItem.columns).height
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
                    var viewContainerPos = viewContainer.mapFromItem(
                                mouseArea, mouseX, mouseY);
                    var child = viewContainer.childAt(viewContainerPos.x,
                                                      viewContainerPos.y);
//                    console.log("X: " + mouseX + ", Y: " +
//                                mouseY);
//                    console.log("viewcontainerPos, X: " + viewContainerPos.x
//                                + ", Y: " + viewContainerPos.y);
//                    var parentPos = viewContainer.mapToItem(null, mouseX, mouseY);
//                    console.log("Parent pos, X: " + parentPos.x + ", Y: " + parentPos.y);
//                    console.log("Child: " + child);
                    return child && child !== mouseArea ? child.__index : -1;
                }

//                onEntered: {
//                    hoveredCellIndex = cellIndexAt(mouseX, mouseY);
//                    if (hoveredCellIndex === undefined) {
//                        hoveredCellIndex = cellIndexAt(mouseX, mouseY);
//                    }

//                    var date = view.model.dateAt(hoveredCellIndex);
//                    if (__isValidDate(date)) {
//                        control.hovered(date);
//                    }
//                }

//                onExited: {
//                    hoveredCellIndex = -1;
//                }

//                onPositionChanged: {
//                    var indexOfCell = cellIndexAt(mouseX, mouseY);
//                    var previousHoveredCellIndex = hoveredCellIndex;
//                    hoveredCellIndex = indexOfCell;
//                    if (indexOfCell !== -1) {
//                        var date = view.model.dateAt(indexOfCell);
//                        if (__isValidDate(date)) {
//                            if (hoveredCellIndex !==
//                                    previousHoveredCellIndex) {
//                                //                                    control.hovered(date);
//                            }

//                            if (pressed && date.getTime() !==
//                                    control.selectedDate.getTime()) {
//                                //                                    control.selectedDate = date;
//                                pressedCellIndex = indexOfCell;
//                                //                                    control.pressed(date);
//                            }
//                        }
//                    }
//                }

                onPressed: {
                    var indexOfCell = cellIndexAt(mouseX, mouseY);
                    if (indexOfCell !== -1) {
                        var date = view.model.dateAt(indexOfCell);
                        pressedCellIndex = indexOfCell;
                        if (__isValidDate(date)) {
//                            control.selectedDate = date;
//                            control.pressed(date);
                        }
                    }
                }

                onReleased: {
                    var indexOfCell = cellIndexAt(mouseX, mouseY);
                    if (indexOfCell !== -1) {
                        var date = view.model.dateAt(indexOfCell);
                        if (__isValidDate(date)) {
//                            control.released(date);
                        }
                    }
                    pressedCellIndex = -1;
                }

                onClicked: {
//                    console.log("Mouse position: " + mouse.x + ", " + mouse.y);
                    var indexOfCell = cellIndexAt(mouseX, mouseY);
                    if (indexOfCell !== -1) {
                        var date = view.model.dateAt(indexOfCell);
                        hoveredCellIndex = indexOfCell;
                        if (__isValidDate(date)) {
//                            control.clicked(date);
                            var global_pos = viewContainer.mapToItem(null, mouseX, mouseY);

    //                        console.log("Global position: " + global_pos.x + ", " + global_pos.y);

                            var show_pos_x = EventJsUtils.getEditViewPosX(global_pos.x);
                            var show_pos_y = EventJsUtils.getEditViewPosY(
                                        global_pos.y, indexOfCell);
                            console.log("Shown Pos: " + show_pos_x + ", " + show_pos_y);
                            float_event_edit.x = show_pos_x;
                            float_event_edit.y = show_pos_y;
                            float_event_edit.showAdd(date);
                        }

//                        var properties = {is_empty_event: true, event_date: date};
//                        var float_event = CreateObject.create(
//                                    "FloatEventEditWindow.qml", viewContainer,
//                                    properties);
//                        console.log("Float_event_edit: ", float_event);

                    }
//                    console.log("OnClicked, indexOfCell: " + indexOfCell);
//                    console.log("Date at this point: " + date);
                }

                onDoubleClicked: {
                    var indexOfCell = cellIndexAt(mouseX, mouseY);
                    if (indexOfCell !== -1) {
                        var date = view.model.dateAt(indexOfCell);
                        if (__isValidDate(date)) {
//                            control.doubleClicked(date);
                        }
                    }
//                    console.log("OnDoubleClicked, indexOfCell: " + indexOfCell);
//                    console.log("Date at this point: " + date);
                }
            }

            Connections {
                target: control
                onSelectedDateChanged: view.selectedDateChanged()
            }

            Repeater {
                id: view

                property int currentIndex: -1

                model: control.__model

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
                        readonly property bool selected:
                            control.selectedDate.getTime() ===
                            date.getTime()
                        readonly property alias date: delegateLoader.__date
                        readonly property bool valid: delegateLoader.valid
                        // TODO: this will not be correct if the app is
                        // running when a new day begins.
                        readonly property bool today: date.getTime() ===
                                                      new Date().setHours(0, 0, 0, 0)
                        readonly property bool visibleMonth:
                            date.getMonth() === control.visibleMonth
                        readonly property bool hovered:
                            panelItem.hoveredCellIndex == index
                        readonly property bool pressed:
                            panelItem.pressedCellIndex == index
                        // TODO: pressed property here, clicked and
                        // doubleClicked in the control itself
                    }
                }
            }
        }
    }

    FloatEventEditView {
        id: float_event_edit
        visible: false
        z: 1
    }

    function clearLabelListModel() {
        while (label_list_model.count > 0) {
            console.log("Label list model, try to destroy.");
            label_list_model.get(0).obj.destroy();
            label_list_model.remove(0);
        }
    }

    function updateEventModel() {
        control.event_model.startDate = control.__model.firstVisibleDate;
        control.event_model.endDate = control.__model.lastVisibleDate;
        control.event_model.updateEvents();
    }

    function labelListModelAddItem(obj, source) {
        label_list_model.append({"obj": obj, "source": source});
    }

    Connections {
        target: control
        onRefreshEvents: {
            console.log("Come to RefreshEvents function, Label model count: " + label_list_model.count);

            clearLabelListModel();

            // TODO: 这里的重复事件未经扩展，
            // 一旦有天天重复之类的，这个逻辑立马崩掉
            updateEventModel();

            var event_counts_of_day = [];
            var show_flags_of_day = [];
            EventJsUtils.initEventCountArray(event_counts_of_day);
            EventJsUtils.initShowFlagsArray(show_flags_of_day);

            var component = Qt.createComponent("TileEventLabel.qml");


//            console.log("control.event_list.events.length: " + control.event_model.events.length);

            console.log("Come to create object.");

            for (var i = 0; i < control.event_model.events.length; ++i) {
                var event = control.event_model.events[i];

//                var index_of_cell = event_utils.gridIndex(event.startDateTime,
//                visible_date);
                var index_of_cell = event_utils.daysTo(
                            control.__model.firstVisibleDate, event.startDateTime);
//                console.log("Index of cell: ", index_of_cell);

                var day_event_count = event_counts_of_day[index_of_cell];

                if (day_event_count >= panelItem.max_show_events_of_day) {
                    event_counts_of_day[index_of_cell] += 1;
                    continue;
                }

                var show_flag_of_day = EventJsUtils.calculateShowFlag(
                            show_flags_of_day, index_of_cell);

                var last_days = event_utils.lastDays(
                            event.startDateTime, event.endDateTime);
                if (last_days === 0 && event.allDay) {
                    last_days = 1;
                }

//                console.log("Last days: ", last_days);

                if (index_of_cell + last_days > total_cells) {
                    last_days -= (index_of_cell + last_days - total_cells);
                }

                EventJsUtils.increaseEventCount(event_counts_of_day, index_of_cell, last_days);
                EventJsUtils.increaseShowFlag(show_flags_of_day,
                                              show_flag_of_day,
                                              index_of_cell,
                                              last_days);


//                console.log("Event start: " + event.startDateTime + " " + event.displayLabel);
//                console.log("Event end: ", event.endDateTime, " ");

                var col = panelItem.columns;
                while (index_of_cell % col + last_days > col) {
                    var clipped_days = col - index_of_cell % col;

                    if (component.status === Component.Ready) {
                        var clipped_label = component.createObject(viewContainer, {"eventItem" : event, "show_flag_of_day": show_flag_of_day, "grid_index": index_of_cell, "last_days": clipped_days});

                        if (clipped_label === null) {
                            console.log("Error creating object");
                        }

                        label_list_model.append({"obj": clipped_label,
                                                    "source":"TileEventLabel.qml"});
                        index_of_cell += clipped_days;
                        last_days -= clipped_days;
                    } else if (component.status === Component.Error) {
                        console.log("Error loading component: ", component.errorString());
                    }
                }

                if (component.status === Component.Ready) {
                    var label = component.createObject(viewContainer, {"eventItem" : event, "show_flag_of_day": show_flag_of_day, "grid_index": index_of_cell, "last_days": last_days});

                    if (label === null) {
                        console.log("Error creating object");
                    } else if (component.status === Component.Error) {
                        console.log("Error loading component: ", component.errorString());
                    }

                    label_list_model.append({"obj": label,
                                                "source":"TileEventLabel.qml"});
                }
            }

            component.destroy();
        }
    }
}
