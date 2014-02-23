import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    visible: true
    width: 960
    height: 600
    minimumHeight: 480
    minimumWidth: 640

    title: "Calendar Example"

    Row {
        id: row
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

//        Rectangle {
//            width: row.width * 0.4 - row.spacing / 2
////            height:
//        }

        Calendar {
            id: calendar
            width: parent.width * 0.6 - row.spacing / 2
            height: parent.height
            selectedDate: new Date(2014, 0, 1)
            focus: true

            style: CalendarStyle {
                dayDelegate: Rectangle {
                    color: styleData.date !== undefined &&
                           styleData.selected ? selectedDateColor : "white"
                    readonly property color sameMonthDateTextColor: "black"
                    readonly property color selectedDateColor: "#aaa"
                    readonly property color seelctedDateTextColor: "white"
                    readonly property color differentMonthDateTextColor:
                        Qt.darker("darkgrey", 1.4)
                    readonly property color invalidDateColor: "#dddddd"

                    Label {
                        id: dayDelegateText
                        text: styleData.date.getDate()
                        font.pixelSize: 14
                        anchors.centerIn: parent
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

                    Rectangle {
                        color: styleData.selected ? "white" : "red"
                        width: 4
                        height: width
                        radius: width / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: dayDelegateText.bottom
                        anchors.topMargin: 2
                        visible: true
                    }
                }
            }
        }
    }
}
