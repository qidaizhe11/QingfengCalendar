import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import MyCalendar.Controls.Private 1.0
import MyCalendar2.Events 1.0
//import QtOrganizer 5.0
import "Private"
import "Private/CalendarUtils.js" as CalendarUtils
import "Content"
import "Test"

Style {
    id: my_calendar_style
    objectName: "calendar_style"

    property MyCalendar control: __control

    property color gridColor: "#f0f0f0"

    // The base color of the main view.
    property color base_color: "lightgray"

    property real __gridLineWidth: 1

    // Provide some useful functions for event dealing.
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

        // The signal should be dealed in each tab view.
        signal showPrevious()
        signal showNext()

        readonly property real bottom_margin: 15

        Button {
            id: previous_button
            width: 40
            height: width
            anchors.left: parent.left
            anchors.leftMargin: parent.height * 0.1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: bottom_margin

            iconSource: "images/arrow-left-mine.png"
            opacity: 0.6

            onClicked: {
                parent.showPrevious();
            }
        }
        Label {
            id: title_text
            text: styleData.title
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 32
            anchors.left: previous_button.right
            anchors.leftMargin: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
        }
        Button {
            id: next_button
            width: 40
            height: width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: parent.height * 0.1
            anchors.bottomMargin: bottom_margin
            iconSource: "images/arrow-right-mine.png"
            opacity: 0.6

            onClicked: parent.showNext()
        }

        MyTabBar {
            id: tab_bar
            tabView: styleData.calendar_tab_view

            anchors.right: next_button.left
            anchors.rightMargin: parent.height * 0.5
            anchors.bottom: parent.bottom
            height: parent.height * 0.3
            width: parent.width * 0.1
        }
    } // navigationBar

//    Item {
    property Component panel: Item {
        id: panelItem

        implicitHeight: 800
        implicitWidth: 600

        property alias navigationBarItem: navigationBarLoader.item

        readonly property real dayOfWeekHeaderRowHeight: 30

        property int hoveredCellIndex: -1
        property int pressedCellIndex: -1

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
                // The title property would be dealed differently in each
                // tabview by all binding this property.
                property string title

                // Used for tab bar since it has been seperated from tab_view.
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

            tabsVisible: false
            frameVisible: false

            Tab {
                title: "Week"
                WeekView {
                    id: week_tab

                    Component.onCompleted: control.refreshEvents()
                }
            }

            Tab {
                title: "Month"
                MonthView {
                    id: month_tab

                    Component.onCompleted: control.refreshEvents()
                }
            }

            currentIndex: 1
        }

        // A light event edit view, only provides most frequent used properties.
        // It's always here, but should only be visible when the date or event
        // be clicked.
        FloatEditView {
            id: float_edit
            visible: false
            z: 10
        }

        // Stores the event label objects shown currently.
        // Since they are created dynamically, it should be tracked here
        // as for destroy when the view changed.
        ListModel {
            id: label_list_model
        }

        // Just change the opacity to 0.
        function hideEventLabels() {
            for (var i = 0; i < label_list_model.count; ++i) {
                label_list_model.get(i).object.opacity = 0;
            }
        }

//        function showEventLabels() {
//            for (var i = 0; i < label_list_model.count; ++i) {
//                label_list_model.get(i).object.opacity = 1;
//            }
//        }

        function clearLabelListModel() {
            while (label_list_model.count > 0) {
                console.log("clearLabelListModel, destroy: ",
                            label_list_model.get(0).object.eventItem.displayLabel);
                label_list_model.get(0).object.destroy();
                label_list_model.remove(0);
            }
            label_list_model.clear();
        }

        function labelListModelAddItem(object) {
            label_list_model.append({"object": object});
        }
    } // panelItem
}
