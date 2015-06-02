import QtQuick 2.1
import QtQml.Models 2.1

Rectangle {
    width: 600
    height: 960

    property date visible_month_date

    readonly property int max_month_list_count: 11
    readonly property int middle_index_of_month_list: (max_month_list_count - 1) / 2

    Component.onCompleted: {
        var select_date = new Date();
        visible_month_date = new Date(select_date.getFullYear(),
                                      select_date.getMonth(),
                                      1);
    }

    ListModel {
        id: month_list_model

        signal refreshMonthList()
        signal insertAtBeginning()
        signal insertAtEnd()

        Component.onCompleted: {
            console.log("List Model onCompleted.")
            var date = visible_month_date;
            date.setMonth(date.getMonth() - middle_index_of_month_list);

            for (var i = 0; i < max_month_list_count; ++i) {
                month_list_model.append({ "month_date": date} );
                console.log("In month_list_model: ", date.toLocaleDateString());
                date.setMonth(date.getMonth() + 1);
            }
            month_list_view.currentIndex = middle_index_of_month_list;
//            control.refreshEvents();
        }

        onInsertAtBeginning: {
            var date = visible_month_date;
            for (var i = 0; i < middle_index_of_month_list; ++i) {
                date.setMonth(date.getMonth() - 1);
                month_list_model.insert(0, { "month_date": date});
                console.log("Insert date: ", date);

                console.log("Try to delete item form month_list_model.",
                            month_list_model.count);
                month_list_model.remove(max_month_list_count);
            }

            month_list_view.positionViewAtIndex(middle_index_of_month_list,
                                                ListView.SnapPosition);
        }

        onInsertAtEnd: {
            var date = visible_month_date;
            for (var i = 0; i < middle_index_of_month_list; ++i) {
                date.setMonth(date.getMonth() + 1);
                month_list_model.append({"month_date": date});
                console.log("Insert date: ", date);
                month_list_model.remove(0);
                console.log("Try to delete date, ", month_list_model.count);
            }

            month_list_view.positionViewAtIndex(middle_index_of_month_list,
                                                ListView.SnapPosition);
        }
    }

    ListView {
        id: month_list_view
        anchors.fill: parent
        anchors.topMargin: parent.height * 0.4

        model: month_list_model
        delegate: month_delegate
//        clip: true
        interactive: true
        focus: true
        cacheBuffer: 3000
        snapMode: ListView.SnapOneItem
        orientation: ListView.Vertical
//        currentIndex: middle_index_of_month_list
        currentIndex: middle_index_of_month_list
//            flickDeceleration: 0
        highlight: Rectangle { width: 600; height: 50;
            y: month_list_view.currentItem.y; color: "indigo" }
        highlightFollowsCurrentItem: true
        highlightMoveVelocity: 1600
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.DragOverBounds

//        add: Transition {
//            NumberAnimation { properties: "y"; duration: 1000 }
//        }

//        addDisplaced: Transition {
//            NumberAnimation { properties: "y"; duration: 1000 }
//        }
    }

    Timer {
        id: insert_at_begin_timer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            console.log("Timer triggered.")
            month_list_model.insertAtBeginning();
        }
    }

    Timer {
        id: insert_at_end_timer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            console.log("Timer triggered.")
            month_list_model.insertAtEnd();
        }
    }

//    Timer {
//        id: refresh_events_timer
//        interval: 800
//        onTriggered: {
//            control.refreshEvents()
//        }
//    }

    Connections {
        target: month_list_view
        onCurrentItemChanged: {
            console.log("Month list view, Current Item Changed.");
            console.log("Index: ", month_list_view.currentIndex);

            var index = month_list_view.currentIndex;
            visible_month_date = month_list_model.get(index).month_date;

            console.log(visible_month_date.toLocaleDateString());

//            refresh_events_timer.start();

            if (index === 0) {
                insert_at_begin_timer.start();
            }
            if (index === max_month_list_count - 1) {
                insert_at_end_timer.start();
            }
        }
    }

    property Component month_delegate: Rectangle {
        width: 600
        height: 50
        border.color: "lightgrey"
        border.width: 1
        color: "transparent"

        Text {
            anchors.centerIn: parent
            text: month_date.toLocaleDateString()
//            color: isCurrentItem ? "red" : "black"
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true

            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    month_list_view.decrementCurrentIndex();
                }

                if (wheel.angleDelta.y < 0) {
                    month_list_view.incrementCurrentIndex();
                }
            }
        }
    }
}
