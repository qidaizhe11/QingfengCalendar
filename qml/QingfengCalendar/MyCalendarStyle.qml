import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
//import QtQuick.Window 2.1
import MyCalendar.Controls.Private 1.0
import MyCalendar2.Events 1.0
import QtOrganizer 5.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import "Content"

Style {
    id: my_calendar_style
    objectName: "calendar_style"

    property MyCalendar control: __control

    property color gridColor: "#f0f0f0"
    property color base_color: "lightgray"

    property real __gridLineWidth: 1

//    signal refreshEvents()

    EventUtils {
        id: event_utils
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
        height: 90

        color: base_color

//        signal showPreviousMonth()
//        signal showNextMonth()
        signal showPrevious()
        signal showNext()

        readonly property real bottom_margin: 15

        Button {
            id: previousMonth
            width: 40
            height: width
            anchors.left: parent.left
            anchors.leftMargin: parent.height * 0.1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: bottom_margin
//            anchors.verticalCenter: parent.verticalCenter
//            iconSource: "qrc:///images/arrow-left.png"
            iconSource: "images/arrow-left-mine.png"
            opacity: 0.6

            onClicked: {
//                control.showPreviousMonth();
                parent.showPrevious();
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
            width: 40
            height: width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: parent.height * 0.1
            anchors.bottomMargin: bottom_margin
//            iconSource: "qrc:///images/arrow-right.png"
            iconSource: "images/arrow-right-mine.png"
            opacity: 0.6

//            onClicked: control.showNextMonth()
            onClicked: parent.showNext()
        }

        MyTabBar {
            id: tab_bar
            tabView: styleData.calendar_tab_view

            anchors.right: nextMonth.left
            anchors.rightMargin: parent.height * 0.5
            anchors.bottom: parent.bottom
            height: parent.height * 0.3
            width: parent.width * 0.1
        }
    }

    property Component weekNumberDelegate: Rectangle {
        color: "white"
        Label {
            text: styleData.weekNumber
            anchors.centerIn: parent
        }
    }

//    Item {
    property Component panel: Item {
        id: panelItem

        implicitHeight: 200
        implicitWidth: 200

        property alias navigationBarItem: navigationBarLoader.item

        readonly property real dayOfWeekHeaderRowHeight: 30

        property int hoveredCellIndex: -1
        property int pressedCellIndex: -1

        ListModel {
            id: label_list_model
        }

        Loader {
            id: backgroundLoader
            anchors.fill: parent
            sourceComponent: background

            MouseArea {
                id: background_mouseArea
                anchors.fill: parent
            }
        }

        Loader {
            id: navigationBarLoader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            sourceComponent: navigationBar

            property QtObject styleData: QtObject {
                property string title
                property var calendar_tab_view: tab_view
            }

            MouseArea {
                id: navigationbar_mouseArea
                anchors.fill: parent
            }
        }

        TabView {
            id: tab_view

            width: panelItem.width
            height: panelItem.height - navigationBarLoader.height
            anchors.top: navigationBarLoader.bottom
//            anchors.top: tab_bar.bottom

            tabsVisible: false
//            tabsVisible: true
            frameVisible: false

            Tab {
                title: "Week"
//                Rectangle {
//                    anchors.fill: parent
//                    color: "indigo"
//                }
                WeekView {
                    id: week_tab
                }
            }

            Tab {
                title: "Month"
                MonthView {
                    id: month_tab
                }
            }
        }

        FloatEditView {
            id: float_edit
            visible: false
            z: 1
        }

        function hideEventLabels() {
            for (var i = 0; i < label_list_model.count; ++i) {
                label_list_model.get(i).object.opacity = 0;
            }
        }

        function showEventLabels() {
            for (var i = 0; i < label_list_model.count; ++i) {
                label_list_model.get(i).object.opacity = 1;
            }
        }

        function clearLabelListModel() {
            while (label_list_model.count > 0) {
                console.log("clearLabelListModel, destroy: ",
                            label_list_model.get(0).object.eventItem.displayLabel);
                label_list_model.get(0).object.destroy();
                label_list_model.remove(0);
            }
        }

        function updateEventModel() {
            control.event_model.startDate = control.__model.firstVisibleDate;
            control.event_model.endDate = control.__model.lastVisibleDate;
            control.event_model.updateCollections();
            control.event_model.updateEvents();
        }

        function labelListModelAddItem(object) {
            label_list_model.append({"object": object});
        }
    } // panelItem
}
