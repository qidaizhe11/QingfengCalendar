import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import org.kde.pim.calendar 1.0

Item {
    Layout.fillHeight: true
    Layout.fillWidth: true

    Rectangle {
        id: todayRect
        width: parent.width
        height: parent.height
//        border.width: 1

        color: "darkgray"

        opacity: (isToday(dayNumber + "/" + monthNumber + "/" + yearNumber)) ?
                     0.8 : 0.3
        Behavior on opacity { NumberAnimation {} }
    }

    Label {
        id: label
        anchors { left: parent.left; top: parent.top;
            leftMargin: 6; topMargin: 4 }
        font.pixelSize: 16
        text: dayNumber
        opacity: (isPreviousMonth || isNextMonth) ? 0.5 : 1.0
    }

    Rectangle {
        id: frameLeft
        width: 1
        color: "darkgray"
        opacity: 0.6
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
    }
    Rectangle {
        id: frameTop
        height: 1
        color: "darkgray"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    Component.onCompleted: {
        var t = isToday(dayNumber + "/" + monthNumber + "/" + yearNumber)
        if (t) {
            root.date = model;
        }
        console.log(dayNumber + " " + monthNumber + " " + yearNumber)
    }
}
