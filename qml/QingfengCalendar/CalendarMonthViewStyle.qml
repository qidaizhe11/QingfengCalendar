import QtQuick 2.1
import QtQuick.Controls 1.1
import MyCalendar.Controls.Private 1.0
import "CalendarUtils.js" as CalendarUtils

Style {
    id: calendarMonthViewStyle

    property Calendar control: __control

    property color gridColor: "#f0f0f0"

    property real __gridLineWidth: 1

    function __cellRectAt(index) {
        return CalendarUtils.cellRectAt(index,
                                        control.__panel.columns,
                                        control.__panel.rows,
                                        control.__panel.availableWidth,
                                        control.__panel.availableHeight);
    }

    function __isValidDate(date) {
        return date !== undefined &&
                date.getTime() >= control.minimumDate.getTime() &&
                date.getTime() <= control.maximumDate.getTime();
    }

    property Component background: Rectangle {
        color: "#fff"
    }

    property Component navigationBar: Item {
        height: 80

        readonly property real bottom_margin: 10

        Button {
            id: previousMonth
            width: parent.height * 0.5
            height: width
            anchors.left: parent.left
            anchors.bottom: parent.bottom
//            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: (parent.height - height) / 2
            anchors.bottomMargin: bottom_margin
//            iconSource: "qrc:///images/arrow-left.png"
            iconSource: "images/arrow-left-mine.png"

            onClicked: control.showPreviousMonth()
        }
        Label {
            id: dateText
            text: styleData.title
            elide: Text.ElideRight
//            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 36
//            anchors.verticalCenter: parent.verticalCenter
            anchors.left: previousMonth.right
            anchors.leftMargin: 26
            anchors.bottom: parent.bottom
            anchors.bottomMargin: bottom_margin
//            anchors.right: nextMonth.left
//            anchors.rightMargin: 2
        }
        Button {
            id: nextMonth
            width: parent.height * 0.5
            height: width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
//            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: (parent.height - height) / 2
            anchors.bottomMargin: bottom_margin
//            iconSource: "qrc:///images/arrow-right.png"
            iconSource: "images/arrow-right-mine.png"

            onClicked: control.showNextMonth()
        }
    }

    property Component dayDelegate: Rectangle {
        color: styleData.date !== undefined &&
               styleData.selected ? selectedDateColor : "white"
        readonly property color sameMonthDateTextColor: "black"
        readonly property color selectedDateColor: __syspal.highlight
        readonly property color selectedDateTextColor: "white"
        readonly property color differentMonthDateTextColor:
            Qt.darker("darkgray", 1.4);
        readonly property color invalidDateColor: "#dddddd"

        Label {
            id: dayDelegateText
            text: styleData.date.getDate()
            font.pixelSize: 14
            //            anchors.centerIn: parent
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.1
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
                        horizontalAlignment: Text.AlignRight
            color: {
                var theColor = invalidDateColor;
                if (styleData.valid) {
                    theColor = styleData.visibleMonth ? sameMonthDateTextColor :
                                                        differentMonthDateTextColor;
                    if (styleData.selected) {
                        theColor = selectedDateColor;
                    }
                    theColor;
                }
            }
        }
    }

    property Component dayOfWeekDelegate: Rectangle {
        color: "white"
        Label {
            text: control.__locale.dayName(styleData.dayOfWeek,
                                           control.dayOfWeekFormat)
//            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    property Component weekNumberDelegate: Rectangle {
        color: "white"
        Label {
            text: styleData.weekNumber
            anchors.centerIn: parent
        }
    }


    property Component panel: Item {
        id: panelItem
        implicitHeight: 200
        implicitWidth: 200

        property alias navigationBarItem: navigationBarLoader.item

        readonly property real dayOfWeekHeaderRowHeight: 40

        readonly property int weeksToshow: 6
        readonly property int rows: weeksToshow
        readonly property int columns: CalendarUtils.daysInWeek

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
        }

        Loader {
            id: navigationBarLoader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            sourceComponent: navigationBar

            property QtObject styleData: QtObject {
                readonly property string title:
                    control.__locale.standaloneMonthName(control.visibleMonth) +
                    new Date(control.visibleYear, control.visibleMonth, 1). toLocaleDateString(control.__locale, " yyyy")
            }
        }

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
                    locale: control.__locale
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
                    model: panelItem.columns + 1
                    delegate: Rectangle {
                        x: index < panelItem.columns ?
                               __cellRectAt(index).x :
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
                    model: panelItem.rows + 1
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
                        var child = viewContainer.childAt(viewContainerPos,x,
                                                          viewContainerPos.y);
                        return child && child !== mouseArea ? child.__index : -1;
                    }

                    onEntered: {
                        hoveredCellIndex = cellIndexAt(mouseX, mouseY);
                        if (hoveredCellIndex === undefined) {
                            hoveredCellIndex = cellIndexAt(mouseX, mouseY);
                        }

                        var date = view.model.dateAt(hoveredCellIndex);
                        if (__isValidDate(date)) {
                            control.hovered(date);
                        }
                    }

                    onExited: {
                        hoveredCellIndex = -1;
                    }

                    onPositionChanged: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse,y);
                        var previousHoveredCellIndex = hoveredCellIndex;
                        hoveredCellIndex = indexOfCell;
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                if (hoveredCellIndex !==
                                        previousHoveredCellIndex) {
                                    control.hovered(date);
                                }

                                if (pressed && date.getTime() !==
                                        control.selectedDate.getTime()) {
                                    control.selectedDate = date;
                                    pressedCellIndex = indexOfCell;
                                    control.pressed(date);
                                }
                            }
                        }
                    }

                    onPressed: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            pressedCellIndex = indexOfCell;
                            if (__isValidDate(date)) {
                                control.selectedDate = date;
                                control.pressed(date);
                            }
                        }
                    }

                    onReleased: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                control.released(date);
                            }
                        }
                        pressedCellIndex = -1;
                    }

                    onClicked: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                control.clicked(date);
                            }
                        }
                    }

                    onDoubleClicked: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                control.doubleClicked(date);
                            }
                        }
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
    }
}
