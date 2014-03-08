import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import org.kde.pim.calendar 1.0

Rectangle {
    id: root
    width: 800
    height: 480

    property QtObject date
    property date showDate: new Date()

    property alias calendar: monthCalendar

//    property alias selectedMonth: monthCalendar.mof

    property alias startDate: monthCalendar.startDate

    property int rows: monthCalendar.weeks

    property int firstDay: new Date(showDate.getFullYear(),
                                    showDate.getMonth(), 1).getDay()

    function isToday(date) {
        if (date === Qt.formatDateTime(new Date(), "d/M/yyyy")) {
            return true;
        }
        else {
            return false;
        }
    }

    function isTodayMonth() {
        return Qt.formatDateTime(new Date(), "yyyy-MM-dd")
    }

    Calendar {
        id: monthCalendar
        weeks: 6
        startDate: isTodayMonth()
    }

//    DaysCalendar {
//        id: calendarGrid
//        anchors.fill: parent
//    }

    GridLayout {
        id: mainLayout
        anchors.fill: parent
//        anchors.margins: -1
        anchors.leftMargin: -1
        anchors.topMargin: -1
        columns: 7
        flow: GridLayout.LeftToRight
        columnSpacing: 0
        rowSpacing: 0

        Repeater {
            id: repeater
            model: monthCalendar.daysModel
            delegate: DayDelegate {}
        }
    }
}
