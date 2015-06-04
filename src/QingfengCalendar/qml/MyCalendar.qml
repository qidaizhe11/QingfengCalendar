import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls.Private 1.0
//import MyCalendar.Controls.Private 1.0
import MyCalendar.Utils 1.0
//import MyCalendar.Weeks 1.0
//import QtOrganizer 5.0
import MyCalendar.MyQtOrganizer 1.0
//import "Private"
import "Private/CalendarUtils.js" as CalendarUtils

Control {
    id: my_calendar
    width: 960
    height: 600

//    property alias selectedDate: rangedDate.date

    property alias minimumDate: rangedDate.minimumDate

    property alias maximumDate: rangedDate.maximumDate

    // Determines which month or week or day will be shown currently.
    // It's the main connection among different calendar views.
    property date visible_date: new Date()

//    property int visibleMonth: selectedDate.getMonth()
//    property int visibleYear: selectedDate.getFullYear()

    // Refresh event models and event views when visible_date changed
    // or event edit (add, edit, or delete) finished.
    signal refreshEvents()

//    onSelectedDateChanged: {
//        visibleMonth = selectedDate.getMonth();
//        visibleYear = selectedDate.getFullYear();
//    }

    RangedDate {
        id: rangedDate
        date: new Date()
        minimumDate: CalendarUtils.minimumCalendarDate
        maximumDate: CalendarUtils.maximumCalendarDate
    }

    property bool gridVisible: true
    property bool weekNumbersVisible: false

    //
    // enum: Calendar::dayOfWeekFormat
    //
    // The format in which the days of the week (in the header) are displayed.
    //
    // Local.ShortFormat is the default and recommended format, as
    // Local.NarrowFormat may not by fullly supported by each local and
    // Local.LongFormat may not fit within the header cells.
    //
//    property int dayOfWeekFormat: Locale.ShortFormat
    property int dayOfWeekFormat: Locale.LongFormat

    property var __locale: Qt.locale()

    // Provides list model of MyEvents, MyCollections that ranged in the given
    // start date and end date, the core center between c++ and qml.
    // The items could be accessed by direct list.
//    property MyEventModel event_model: MyEventModel {
//        startDate: new Date()
//        endDate: new Date()

//        onEventsChanged: {
//            console.log("MyEventModel, onEventsChanged.");
//            my_calendar.refreshEvents();
//        }
//    }

    property OrganizerModel event_model: OrganizerModel {
        startPeriod: '2005-01-01'
        endPeriod: '2015-01-01'

        autoUpdate: true

        manager: "memory"

        onModelChanged: {
//            console.log("OrganizerModel, onModelChanged.");
//            my_calendar.refreshEvents();
        }
    }

    style: Qt.createComponent("MyCalendarStyle.qml", my_calendar)

    signal hovered(date date)

    signal pressed(date date)

    signal released(date date)

    signal clicked(date date)

    signal doubleClicked(date date)

    function showPreviousMonth() {
        if (visibleMonth === 0) {
            visibleMonth = CalendarUtils.monthsInYear - 1;
            --visibleYear;
        } else {
            --visibleMonth;
        }
    }

    function showNextMonth() {
        if (visibleMonth === CalendarUtils.monthsInYear - 1) {
            visibleMonth = 0;
            ++visibleYear;
        } else {
            ++visibleMonth;
        }
    }

    function showPreviousYear() {
        if (visibleYear - 1 >= minimumDate.getFullYear()) {
            --visibleYear;
        }
    }

    function showNextYear() {
        if (visibleYear + 1 <= maximumDate.getFullYear()) {
            ++visibleYear;
        }
    }

    function __selectPreviousMonth() {
        my_calendar.selectedDate = CalendarUtils.setMonth(my_calendar.selectedDate,
                                    my_calendar.selectedDate.getMonth() - 1);
    }

    function __selectNextMonth() {
        my_calendar.selectedDate = CalendarUtils.setMonth(my_calendar.selectedDate,
                                    my_calendar.selectedDate.getMonth() + 1);
    }

    function __selectPreviousWeek() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(newDate.getDate() - CalendarUtils.daysInAWeek);
        my_calendar.selectedDate = newDate;
    }

    function __selectNextWeek() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(newDate.getDate() + CalendarUtils.daysInAWeek);
        my_calendar.selectedDate = newDate;
    }

    function __selectedFirstDayOfMonth() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(1);
        my_calendar.selectedDate = newDate;
    }

    function __selectedLastDayOfMonth() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(CalendarUtils.daysInMonth(newDate));
        my_calendar.selectedDate = newDate;
    }

    function __selectedPreviousDay() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(newDate.getDate() - 1);
        my_calendar.selectedDate = newDate;
    }

    function __selectedNextDay() {
        var newDate = new Date(my_calendar.selectedDate);
        newDate.setDate(newDate.getDate() + 1);
        my_calendar.selectedDate = newDate;
    }

    Keys.onLeftPressed: {
        my_calendar.__selectedPreviousDay();
    }

    Keys.onUpPressed: {
        my_calendar.__selectPreviousWeek();
    }

    Keys.onDownPressed: {
        my_calendar.__selectNextWeek();
    }

    Keys.onRightPressed: {
        my_calendar.__selectedNextDay();
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Home) {
            my_calendar.__selectedFirstDayOfMonth();
            event.accepted = true;
        } else if (event.key === Qt.Key_End) {
            my_calendar.__selectedLastDayOfMonth();
            event.accepted = true;
        } else if (event.key === Qt.Key_PageUp) {
            my_calendar.__selectPreviousMonth();
            event.accepted = true;
        } else if (event.key === Qt.Key_PageDown) {
            my_calendar.__selectNextMonth();
            event.accepted = true;
        }
    }
}
