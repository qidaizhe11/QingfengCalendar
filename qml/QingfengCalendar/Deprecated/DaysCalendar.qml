import QtQuick 2.0
import QtQuick.Controls 1.0
import org.kde.pim.calendar 1.0

Item {
    Grid {
        id: calendarDays
        anchors.fill: parent
        columns: 7
        rows: 1 + monthCalendar.weeks
        spacing: 0

//        Repeater {
//            id: days
//            model: monthCalendar.days
//            Item {
//                width: 80
//                height: 60
//                Label {
//                    text: Qt.formatDate(new Date(showDate.getFullYear(),
//                            showDate.getMonth(), index - firstDay + 1), "ddd");
//                    font.pixelSize: root.cellHeight / 6
//                }
//            }
//        }

        Repeater {
            id: repeater
            model: monthCalendar.daysModel

            DayDelegate {}
        }
    }
}
