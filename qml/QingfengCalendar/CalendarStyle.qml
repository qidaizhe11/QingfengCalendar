import QtQuick 2.1
import QtQuick.Controls 1.1
import MyCalendar.Controls.Private 1.0
import "CalendarUtils.js" as CalendarUtils

Style {
    id: calendarStyle

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
        height: 50

        Button {
            id: previousMonth
            width: parent.height * 0.6
            height: width
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: (parent.height - height) / 2
            iconSource: "images/arrow-left.png"

//            onClicked: control.
        }
    }
}
