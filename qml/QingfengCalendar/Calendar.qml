import QtQuick 2.0
import MyCalendar.Controls.Private 1.0
import "CalendarUtils.js" as CalendarUtils

Rectangle {
    id: calendar
    width: 960
    height: 600

    property alias selectedDate: rangedDate.date

    property alias minimumDate: rangedDate.minimumDate

    property alias maximumDate: rangedDate.maximumDate

    property int visibleMonth: selectedDate.getMonth()

    property int visibleYear: selectedDate.getFullYear()

    onSelectedDateChanged: {
        visibleMonth = selectedDate.getMonth();
        visibleYear = selectedDate.getFullYear();
    }

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
    property int dayOfWeekFormat: Locale.ShortFormat

    property var __locale: Qt.locale()

    property CalendarModel __model: CalendarModel {
        locale: calendar.__locale
        visibleDate: new Date(visibleYear, visibleMonth, 1)
    }


}
