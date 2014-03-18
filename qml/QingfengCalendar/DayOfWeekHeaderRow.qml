import QtQuick 2.1
import QtQuick.Controls 1.1
//import MyCalendar.Controls.Private 1.0
import "Private"

Item {
    id: dayOfWeekHeaderRow

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

    Row {
        id: row
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
    } // day_of_week_header_row
}
