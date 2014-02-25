import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtOrganizer 5.0
import MyCalendar2.Utils.Events 1.0

Rectangle {
    visible: true
    width: 960
    height: 600
//    minimumHeight: 480
//    minimumWidth: 640

//    title: "Calendar Example"

//    OrganizerModel {
//        id: organizer
//        startPeriod: new Date("1970-01-01")
//        endPeriod: new Date("2014-12-31")
//        manager: "memory"

////        Component.onCompleted: {
////            organizer.importItems(Qt.resolvedUrl(
////                                      "content/qidaizhe11@gmail.com-2.ics"))
////        }
//    }

//    property var array: my_event_list.events

//    EventListUtils {
//        id: my_event_list
////        startDate: __model.firstVisibleDate
//        startDate: new Date("2011-01-01")
////        endDate: __model.lastVisibleDate
//        endDate: new Date("2014-02-15")

//        Component.onCompleted: {
//            console.log("mainEvents.")
////            console.log(endDate)
////            console.log(array.lengh)
//        }
//    }

    Row {
        id: row
        anchors.fill: parent
//        anchors.margins: 20
//        spacing: 10

//        Rectangle {
//            width: row.width * 0.4 - row.spacing / 2
////            height:
//        }

        function refreshData(arrData) {
            console.log(arrData);
            for (var i = 0; i < arrData.length; ++i) {
                console.log(arrData[i].description + ", " + arrData[i].displayLabel);
    //            console.log(arrData[i].displayLabel);
            }
        }

        Calendar {
            id: calendar
            objectName: "calendar"
//            width: parent.width * 0.6 - row.spacing / 2
            width: parent.width
            height: parent.height
            selectedDate: new Date(2014, 0, 1)
            focus: true

            style: CalendarMonthViewStyle {
                dayDelegate: Rectangle {
                    color: styleData.date !== undefined &&
                           styleData.selected ? selectedDateColor : "white"
                    readonly property color sameMonthDateTextColor: "black"
                    readonly property color selectedDateColor: "#aaa"
                    readonly property color selectedDateTextColor: "white"
                    readonly property color differentMonthDateTextColor:
                        Qt.darker("darkgrey", 1.4)
                    readonly property color invalidDateColor: "#dddddd"

                    Label {
                        id: dayDelegateText
                        text: styleData.date.getDate()
                        font.pixelSize: 14
//                        anchors.centerIn: parent
                        anchors.top: parent.top
                        anchors.topMargin: parent.height * 0.1
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width * 0.1
                        color: {
                            var color = invalidDateColor;
                            if (styleData.valid) {
                                color = styleData.visibleMonth ?
                                            sameMonthDateTextColor :
                                            differentMonthDateTextColor;
                                if (styleData.selected) {
                                    color = selectedDateTextColor;
                                }
                            }
                            color;
                        }
                    }

//                    Rectangle {
//                        color: styleData.selected ? "white" : "red"
//                        width: 4
//                        height: width
//                        radius: width / 2
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        anchors.top: dayDelegateText.bottom
//                        anchors.topMargin: 2
//                        visible: true
//                    }
                }
            }
        }
    }

//    function refreshData(arrData) {
//        console.log(arrData);
//        for (var i = 0; i < arrData.length; ++i) {
//            console.log(arrData[i].description + ", " + arrData[i].displayLabel);
////            console.log(arrData[i].displayLabel);
//        }
//    }

    Component.onCompleted: {
        console.log("The last");
//        console.log(calendar.event_list.events)
//        for (var i = 0; i < calendar.event_list.events.length; ++i) {
//            console.log(calendar.event_list.events[i])
//        }
//        refreshData();
//        console.log(refreshData())
    }
}
