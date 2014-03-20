import QtQuick 2.1
import QtQuick.Controls 1.1
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils

Item {
    id: week_view

    readonly property int weeksToshow: CalendarUtils.weeksOnCalendarMonth
    readonly property int rows: weeksToshow
    readonly property int columns: CalendarUtils.daysInAWeek

    readonly property int total_cells: rows * columns


    readonly property int max_week_list_count: 3
    readonly property int middle_index_of_week_list: (max_week_list_count - 1) / 2

    property int week_list_index: middle_index_of_week_list

    readonly property real availableWidth:
        (viewContainer.width - (control.gridVisible ? __gridLineWidth : 0))
    readonly property real availableHeight:
        (viewContainer.height - (control.gridVisible ? __gridLineWidth : 0))

    function __cellRectAt(index) {
        return CalendarUtils.cellRectAt(index,
                                        week_view.columns,
                                        week_view.rows,
                                        week_view.availableWidth,
                                        week_view.availableHeight);
    }

    property Component dayDelegate: Rectangle {
        //        color: styleData.date !== undefined &&
        //               styleData.selected ? selectedDateColor : "orangle"
        readonly property color base_color: "lightgray"

        readonly property color selectedDateColor: Qt.darker("darkgray", 1.4)
        readonly property color sameMonthDateTextColor: Qt.darker("darkgray", 3.0)
        readonly property color different_month_date_color:
            Qt.darker("lightgray", 1.05)

        readonly property color selectedDateTextColor: "white"
        readonly property color differentMonthDateTextColor:
            Qt.darker("darkgray", 1.4)

        readonly property color invalidDateColor: "#dddddd"

        color: {
            var the_color = invalidDateColor;
            if (styleData.valid) {
                the_color = styleData.visibleMonth ? base_color :
                                                     different_month_date_color;
                if (styleData.selected) {
                    the_color = selectedDateColor;
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
                var theColor = invalidDateColor;
                if (styleData.valid) {
                    theColor = styleData.visibleMonth ? sameMonthDateTextColor :
                                                        differentMonthDateTextColor;
                    if (styleData.selected) {
                        theColor = selectedDateTextColor;
                    }
                }
                theColor;
            }
        }
    } // dayDelegete

    property Component dayOfWeekDelegate: Rectangle {
//        color: Qt.darker("darkgray", 1.5)
        color: base_color
        Label {
            text: Qt.locale().dayName(styleData.dayOfWeek,
                                           control.dayOfWeekFormat)
//            text: styleData.dayOfWeek
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item {
        id: dayOfWeekHeaderRow
        width: panelItem.width
        height: panelItem.dayOfWeekHeaderRowHeight
//        anchors.top: parent.top

        Row {
            spacing: (control.gridVisible ? __gridLineWidth : 0)

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

//        Repeater {
//            id: header_grid_line_repeater
//            model: month_view.columns - 1
//            delegate: Rectangle {
//                x: __cellRectAt(index).x + __cellRectAt(index).width
//                y: 10
//                width: __gridLineWidth
//                height: dayOfWeekHeaderRow.height
//                color: gridColor
//                visible: control.gridVisible
//            }
//        }
    } // day_of_week_header_row

    Item {
        id: viewContainer
        width: panelItem.width
        height: panelItem.height - navigationBarLoader.height -
                dayOfWeekHeaderRow.height
        anchors.top: dayOfWeekHeaderRow.bottom

        ListModel {
            id: week_list_model

            signal refreshWeekList()
            signal insertAtBeginning()
            signal insertAtEnd()

            Component.onCompleted: {
                console.log("List Model onCompleted.")
                var date = control.visible_date;
                date.setMonth(date.getMonth() - middle_index_of_week_list);

                for (var i = 0; i < max_week_list_count; ++i) {
                    week_list_model.append({ "month_date": date} );
                    console.log("month_list_model, append: ", date.toLocaleDateString());
                    date.setMonth(date.getMonth() + 1);
                }
                week_list_view.currentIndex = middle_index_of_week_list;
                //            control.refreshEvents();
            }

            onInsertAtBeginning: {
                var date = control.visible_date;
                for (var i = 0; i < middle_index_of_month_list; ++i) {
                    date.setMonth(date.getMonth() - 1);
                    week_list_model.insert(0, { "month_date": date});
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
                    week_list_model.append({"month_date": date});
                    console.log("onInsertAtEnd Insert Month date: ",
                                date.toLocaleDateString());
                    week_list_model.remove(0);
                    console.log("Delete Month date of index: 0");
                }

                week_list_view.positionViewAtIndex(middle_index_of_week_list,
                                                    ListView.SnapPosition);
                //            control.refreshEvents();
            }
        } // month_list_model

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
        width: panelItem.width
        height: panelItem.height - navigationBarLoader.height -
                dayOfWeekHeaderRow.height

        Flickable {
            id: week_flickable
            anchors.centerIn: parent
            width: parent.width
            height: parent.height < week_item_rectangle.height ?
                        parent.height : week_item_rectangle.height
            flickableDirection: Flickable.VerticalFlick
            contentHeight: 1000
            clip: true

            Rectangle {
                id: week_item_rectangle
                anchors.fill: parent
                color: "lightsteelblue"
            }

            Text {
                anchors.centerIn: parent
                text: month_date.toLocaleDateString()
            }
        }
    }

//    property Component week_delegate: Item {
//    Item {
//        id: week_delegate

//        width: panelItem.width
//        height: panelItem.height - navigationBarLoader.height -
//                dayOfWeekHeaderRow.height

//        Repeater {
//            id: verticalGridLineRepeater
//            model: month_view.columns - 1
//            delegate: Rectangle {
//                x: index < month_view.columns ?
//                       __cellRectAt(index).x + __cellRectAt(index).width :
//                       __cellRectAt(month_view.columns - 1).x +
//                       __cellRectAt(month_view.columns - 1).width
////                y: -dayOfWeekHeaderRow.height + 5
//                y: 0
//                width: __gridLineWidth
////                height: viewContainer.height + dayOfWeekHeaderRow.height
//                height: viewContainer.height
//                color: gridColor
//                visible: control.gridVisible
//            }
//        }

//        Repeater {
//            id: horizontalGridLineRepeater
//            model: month_view.rows
//            delegate: Rectangle {
//                x: 0
//                y: index < month_view.columns - 1 ?
//                       __cellRectAt(index * month_view.columns).y :
//                       __cellRectAt((month_view.rows - 1) *
//                                    month_view.columns).y +
//                       __cellRectAt((month_view.rows - 1) *
//                                    month_view.columns).height
//                width: viewContainer.width
//                height: __gridLineWidth
//                color: gridColor
//                visible: control.gridVisible
//            }
//        }

//        MouseArea {
//            id: mouseArea
//            anchors.fill: parent

//            hoverEnabled: true

//            function cellIndexAt(mouseX, mouseY) {
//                var viewContainerPos = parent.mapFromItem(
//                            mouseArea, mouseX, mouseY);
//                var child = parent.childAt(viewContainerPos.x,
//                                           viewContainerPos.y);
////                console.log("X: " + mouseX + ", Y: " +
////                            mouseY);
////                console.log("viewcontainerPos, X: " + viewContainerPos.x
////                            + ", Y: " + viewContainerPos.y);
////                var parentPos = viewContainer.mapToItem(null, mouseX, mouseY);
////                console.log("Parent pos, X: " + parentPos.x + ", Y: " + parentPos.y);
////                console.log("Child: " + child);
//                return child && child !== mouseArea ? child.__index : -1;
//            }

//            onPressed: {
//                var indexOfCell = cellIndexAt(mouseX, mouseY);
//                if (indexOfCell !== -1) {
//                    var date = view.model.dateAt(indexOfCell);
////                    pressedCellIndex = indexOfCell;
//                    if (__isValidDate(date)) {
////                        control.selectedDate = date;
////                        control.pressed(date);
//                    }
//                }
//            }

//            onReleased: {
//                var indexOfCell = cellIndexAt(mouseX, mouseY);
//                if (indexOfCell !== -1) {
//                    var date = view.model.dateAt(indexOfCell);
//                    if (__isValidDate(date)) {
////                        control.released(date);
//                    }
//                }
//                pressedCellIndex = -1;
//            }

//            onWheel: {
//                if (wheel.angleDelta.y > 0) {
//                    showPreviousMonth();
//                } else if (wheel.angleDelta.y < 0) {
//                    showNextMonth();
//                }
//            }

//            onClicked: {
//                console.log("Mouse position: " + mouse.x + ", " + mouse.y);
//                var indexOfCell = cellIndexAt(mouseX, mouseY);
//                console.log("IndexOfCell: ", indexOfCell);
//                if (indexOfCell !== -1) {
//                    var date = view.model.dateAt(indexOfCell);
//                    console.log("Date at this point: ", date.toLocaleDateString());
//                    panelItem.hoveredCellIndex = indexOfCell;
//                    if (__isValidDate(date)) {
////                        control.clicked(date);
//                        var global_pos = viewContainer.mapToItem(null, mouseX, mouseY);

////                        console.log("Global position: " + global_pos.x + ", " + global_pos.y);

//                        var show_pos_x = MonthViewUtils.getEditViewPosX(global_pos.x);
//                        var show_pos_y = MonthViewUtils.getEditViewPosY(
//                                    global_pos.y, indexOfCell);
//                        console.log("Shown Pos: " + show_pos_x + ", " + show_pos_y);
//                        float_edit.x = show_pos_x;
//                        float_edit.y = show_pos_y;
//                        float_edit.showAdd(date);
//                    }
//                }
//            }

//            onDoubleClicked: {
//                var indexOfCell = cellIndexAt(mouseX, mouseY);
//                if (indexOfCell !== -1) {
//                    var date = view.model.dateAt(indexOfCell);
//                    if (__isValidDate(date)) {
////                        control.doubleClicked(date);
//                    }
//                }
//            }
//        }

//        Connections {
//            target: control
//            onSelectedDateChanged: view.selectedDateChanged()
//        }

//        Repeater {
//            id: view

//            property int currentIndex: -1

//            model: CalendarModel {
//                visibleDate: month_date
//            }

//            Component.onCompleted: selectedDateChanged()

//            function selectedDateChanged() {
//                if (model !== undefined && model.locale !== undefined) {
//                    currentIndex = model.indexAt(control.selectedDate);
//                }
//            }

//            delegate: Loader {
//                id: delegateLoader

//                x: __cellRectAt(index).x +
//                   (control.gridVisible ? __gridLineWidth : 0)
//                y: __cellRectAt(index).y +
//                   (control.gridVisible ? __gridLineWidth : 0)
//                width: __cellRectAt(index).width -
//                       (control.gridVisible ? __gridLineWidth : 0)
//                height: __cellRectAt(index).height -
//                        (control.gridVisible ? __gridLineWidth : 0)

//                sourceComponent: dayDelegate

//                readonly property int __index: index
//                readonly property date __date: date

//                readonly property bool valid: __isValidDate(date)

//                property QtObject styleData: QtObject {
//                    readonly property alias index:
//                        delegateLoader.__index
//                    readonly property bool selected:
//                        control.selectedDate.getTime() ===
//                        date.getTime()
//                    readonly property alias date: delegateLoader.__date
//                    readonly property bool valid: delegateLoader.valid
//                    // TODO: this will not be correct if the app is
//                    // running when a new day begins.
//                    readonly property bool today: date.getTime() ===
//                                                  new Date().setHours(0, 0, 0, 0)
//                    readonly property bool visibleMonth:
//                        date.getMonth() === month_date.getMonth()
//                    readonly property bool hovered:
//                        panelItem.hoveredCellIndex == index
//                    readonly property bool pressed:
//                        panelItem.pressedCellIndex == index
//                    // TODO: pressed property here, clicked and
//                    // doubleClicked in the control itself
//                }
//            } // delegate_loader
//        } // view Repeater
//    } // month_delegate
}
