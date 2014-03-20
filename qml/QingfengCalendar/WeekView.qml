import QtQuick 2.1
import QtQuick.Controls 1.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import MyCalendar.Weeks 1.0

Item {
    id: week_view

//    readonly property int weeksToshow: CalendarUtils.weeksOnCalendarMonth
    readonly property int rows: 24 * 2
    readonly property int columns: CalendarUtils.daysInAWeek

    readonly property int total_cells: rows * columns

    readonly property int max_week_list_count: 3
    readonly property int middle_index_of_week_list: (max_week_list_count - 1) / 2

    property int week_list_index: middle_index_of_week_list

//    property string show: navigationBarLoader.styleData.title

    property MyWeekModel week_model: MyWeekModel {
        visibleDate: control.visible_date
    }

    property string calendar_title: {
        var title = "";
        var range_start_date = week_model.firstVisibleDate;
        title += range_start_date.toLocaleDateString(Qt.locale(),"yyyy M d");
        title += " - ";
        var range_end_date = week_model.lastVisibleDate;
        if (range_start_date.getMonth() === range_end_date.getMonth()) {
            title += range_end_date.toLocaleDateString(Qt.locale(), "d");
        } else if (range_start_date.getFullYear() ===
                   range_end_date.getFullYear()) {
            title += range_end_date.toLocaleDateString(Qt.locale(), "M d");
        } else {
            title += range_end_date.toLocaleDateString();
        }
        title;
    }

    Component.onCompleted: {
        navigationBarLoader.styleData.title = calendar_title;
    }

    onCalendar_titleChanged: navigationBarLoader.styleData.title = calendar_title;

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

    property Component dayTimeDelegate: Rectangle {
//    Rectangle {
//        color: "white"
        color: base_color
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
        id: week_central_view
//        width: panelItem.width
//        height: panelItem.height - navigationBarLoader.height
//        anchors.top: dayHeaderRow.bottom
        anchors.fill: parent

        ListModel {
            id: week_list_model

            signal refreshWeekList()
            signal insertAtBeginning()
            signal insertAtEnd()

            Component.onCompleted: {
                console.log("List Model onCompleted.")
                var date = control.visible_date;
                date.setDate(date.getDate() - middle_index_of_week_list * 7);

                for (var i = 0; i < max_week_list_count; ++i) {
                    week_list_model.append({ "week_date": date} );
                    console.log("week_list_model, append: ", date.toLocaleDateString());
                    date.setDate(date.getDate() + 7);
                }
                week_list_view.currentIndex = middle_index_of_week_list;
                //            control.refreshEvents();
            }

            onInsertAtBeginning: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_month_list; ++i) {
                    date.setMonth(date.getMonth() - 1);
                    week_list_model.insert(0, { "week_date": date});
                    console.log("OnInsertAtBeingning Insert Month date: ",
                                date.toLocaleDateString());

                    week_list_model.remove(max_week_list_count);
                    console.log("Delete Month date of index: ", max_week_list_count);
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
                //            control.refreshEvents();
            }

            onInsertAtEnd: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_week_list; ++i) {
                    date.setMonth(date.getMonth() + 1);
                    week_list_model.append({"week_date": date});
                    console.log("onInsertAtEnd Insert Month date: ",
                                date.toLocaleDateString());
                    week_list_model.remove(0);
                    console.log("Delete Month date of index: 0");
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
                //            control.refreshEvents();
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
            currentIndex: middle_index_of_month_list
            //            flickDeceleration: 0
            highlightFollowsCurrentItem: true
            highlightMoveVelocity: 1600
            //            maximumFlickVelocity: 1000
            highlightRangeMode: ListView.StrictlyEnforceRange
            boundsBehavior: Flickable.DragOverBounds
            //            preferredHighlightBegin: 0
            //            preferredHighlightEnd: 0
            //            keyNavigationWraps: true

            Component.onCompleted: {
                console.log("List View onCompleted.")
                //                currentIndex = middle_index_of_month_list
            }

            //            onDragStarted: hideEventLabels();
//            onDragStarted: hideEventLabels();
            //            onFlickEnded: console.log("MovementEnded.");
            //            onDragEnded: showEventLabels();
        } // month_list_view
    } // view_container

    property Component week_delegate: Item {
//    Item {
        id: week_delegate
        width: panelItem.width
        height: panelItem.height - navigationBarLoader.height

        readonly property real availableWidth:
            (week_container_item.width - (control.gridVisible ? __gridLineWidth : 0))
        readonly property real availableHeight:
            (week_grid_view.height - (control.gridVisible ? __gridLineWidth : 0))

        function __cellRectAt(index) {
            return CalendarUtils.cellRectAt(index,
                                            week_view.columns,
                                            (week_view.rows),
                                            week_delegate.availableWidth,
                                            week_delegate.availableHeight);
        }

        Item {
            id: day_header_row
            width: panelItem.width - 60
            height: panelItem.dayOfWeekHeaderRowHeight
            anchors.left: parent.left
            anchors.leftMargin: 60 + __gridLineWidth
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
//                        width: day_header_row.width / 7
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

//            Repeater {
//                id: header_grid_line_repeater
//                model: month_view.columns - 1
//                delegate: Rectangle {
//                    x: __cellRectAt(index).x + __cellRectAt(index).width
//                    y: 10
//                    width: __gridLineWidth
//                    height: dayOfWeekHeaderRow.height
//                    color: gridColor
//                    visible: control.gridVisible
//                }
//            }
        } // day_of_week_header_row

        Flickable {
            id: week_flickable
            anchors.top: day_header_row.bottom
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.centerIn: parent
            width: parent.width
            height: (parent.height - day_header_row.height) < week_grid_view.height ?
                        (parent.height - day_header_row.height) : week_grid_view.height
            flickableDirection: Flickable.VerticalFlick
            contentHeight: 1300
            clip: true

            Row {
                id: week_grid_view
//                height: 1300
                anchors.fill: parent

                Item {
                    id: day_time_item
                    width: 60
                    height: parent.height

                    Repeater {
                        id: day_time_repeater
                        model: week_view.rows / 2

                        Loader {
                            id: day_time_delegate_loader
                            y: __cellRectAt(index * week_view.columns * 2).y +
                               (control.gridVisible ? __gridLineWidth : 0)
                            width: day_time_item.width
                            height: __cellRectAt(index * week_view.columns).height -
                                    (control.gridVisible ? __gridLineWidth : 0)
                            sourceComponent: dayTimeDelegate

                            readonly property int __index: index

                            property QtObject styleData: QtObject {
                                readonly property alias index:
                                    day_time_delegate_loader.__index
                            }

                            Component.onCompleted: {
//                                console.log("day_time_delegate_loader: ",
//                                            day_time_delegate_loader.width,
//                                            day_time_delegate_loader.height);
                            }
                        } // loader
                    } // repeater
                } // day_time_item

                Item {
                    id: week_container_item

                    width: parent.width - day_time_item.width
                    height: parent.height

                    Component.onCompleted: {
//                        console.log("week_container_item, width:",
//                                    week_container_item.width);
                    }

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
                            height: week_container_item.height
                            color: gridColor
                            visible: control.gridVisible
                        }
                    }

                    Repeater {
                        id: horizontalGridLineRepeater

                        model: week_view.rows * 2 + 1
                        delegate: Rectangle {
                            property bool is_hour: index % 2 === 0

                            x: is_hour ? -day_time_item.width : 0
                            y: index < week_view.rows ?
                                   __cellRectAt(index * week_view.columns).y :
                                   __cellRectAt((week_view.rows - 1) *
                                                week_view.columns).y +
                                   __cellRectAt((week_view.rows - 1) *
                                                week_view.columns).height
//                            width: week_container_item.width + day_time_item.width
                            width: is_hour ?
                                       (week_container_item.width + day_time_item.width) :
                                       week_container_item.width
                            height: __gridLineWidth
                            color: is_hour ? gridColor : Qt.lighter("lightgrey", 1.03)
                            visible: control.gridVisible
                            Component.onCompleted: {
//                                console.log("y:", y);
                            }
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
                } // month_delegate
            } // row
        }
    }
}
