import QtQuick 2.1
import QtQuick.Controls 1.0
//import QtQuick.Window 2.1
import MyCalendar.Controls.Private 1.0
import MyCalendar.Utils.Events 1.0
import QtOrganizer 5.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
//import "EventJsUtils.js" as EventJsUtils

Style {
    id: calendarMonthViewStyle
    objectName: "calendar_style"

    property MyCalendar control: __control

    property color gridColor: "#f0f0f0"
    property color base_color: "lightgray"

    property real __gridLineWidth: 1

    signal refreshEvents()


    ListModel {
        id: label_list_model
    }

    EventUtils {
        id: event_utils
    }

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
        color: base_color
    }

    property Component navigationBar: Rectangle {
        height: 100

        color: base_color

        readonly property real bottom_margin: 15

        Button {
            id: previousMonth
            width: parent.height * 0.4
            height: width
            anchors.left: parent.left
            anchors.leftMargin: (parent.height - height) / 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: bottom_margin
//            anchors.verticalCenter: parent.verticalCenter
//            iconSource: "qrc:///images/arrow-left.png"
            iconSource: "images/arrow-left-mine.png"
            opacity: 0.6

            onClicked: {
                control.showPreviousMonth();
            }
        }
        Label {
            id: dateText
            text: styleData.title
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 36
            anchors.left: previousMonth.right
            anchors.leftMargin: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
        }
        Button {
            id: nextMonth
            width: parent.height * 0.4
            height: width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: (parent.height - height) / 5
            anchors.bottomMargin: bottom_margin
//            iconSource: "qrc:///images/arrow-right.png"
            iconSource: "images/arrow-right-mine.png"
            opacity: 0.6

            onClicked: control.showNextMonth()
        }
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
            anchors.topMargin: dayDelegateText.width * 0.4
            anchors.left: parent.left
            anchors.leftMargin: dayDelegateText.height * 0.5
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
    }

    property Component dayOfWeekDelegate: Rectangle {
//        color: Qt.darker("darkgray", 1.5)
        color: base_color
        Label {
            text: Qt.locale().dayName(styleData.dayOfWeek,
                                           control.dayOfWeekFormat)
//            text: styleData.dayOfWeek
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

//    property Component loat_event_edit: FloatEventEditView {
//        opacity: 0
//    }

    property Component panel: MonthViewPanelItem {
//        id: panelItem
    }
}
