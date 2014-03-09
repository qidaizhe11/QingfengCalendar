import QtQuick 2.0
import QtQml.Models 2.1

Rectangle {
    width: 600
    height: 960

    property date visible_month_date

    Component.onCompleted: {
        var select_date = new Date();
        visible_month_date = new Date(select_date.getFullYear(),
                                      select_date.getMonth(),
                                      1);
    }

    ListModel {
        id: month_list_model

        signal showUp()
        signal showDown()

        Component.onCompleted: {
            for (var i = 0; i < 3; ++i) {
                var date = visible_month_date;
                date.setMonth(date.getMonth() + i - 1);
                month_list_model.append( { "month_date": date} );
                console.log(date);
            }
        }

        onShowUp: {
            var date = visible_month_date;
            date.setMonth(date.getMonth() - 2);
            month_list_model.move(0, 1, 1);
            month_list_model.move(1, 2, 1);
            month_list_model.insert(0, { "month_date": date});
            month_list_model.remove(3);

            console.log(visible_month_date);
            date.setMonth(date.getMonth() + 1);
            visible_month_date = date;
            console.log(visible_month_date);
        }

        onShowDown: {
            month_list_model.remove(0);
            month_list_model.move(1, 0, 2);
            var date = visible_month_date;
            date.setMonth(date.getMonth() + 2);
            month_list_model.append({ "month_date": date});
//            month_list_model

            date.setMonth(date.getMonth() - 1);
            visible_month_date = date;
        }
    }

    ListView {
        id: view_container_listview
        anchors.fill: parent

        model: month_list_model
        delegate: monthDelegate
        clip: true
        interactive: true
        focus: true
        cacheBuffer: 3000
        snapMode: ListView.SnapOneItem
        orientation: ListView.Vertical
        flickDeceleration: 1000
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.DragOverBounds
//            preferredHighlightBegin: 0
//            preferredHighlightEnd: 0
//            keyNavigationWraps: true

        displaced: Transition {
            NumberAnimation { properties: "y"; duration: 2000 }
        }

//            add: Transition {
//                NumberAnimation { properties: "y"; duration: 1000 }
//            }

//            addDisplaced: Transition {
//                NumberAnimation { properties: "y"; duration: 1000 }
//            }

            move: Transition {
                NumberAnimation { properties: "y"; duration: 2000 }
            }

            moveDisplaced: Transition {
                NumberAnimation { properties: "y"; duration: 2000}
            }

//        remove: Transition {
//            NumberAnimation { properties: "y"; to: 0; duration: 1000 }
//        }

//            removeDisplaced: Transition {
//                NumberAnimation { properties: "y"; duration: 1000}
//            }
    }

    property Component monthDelegate: Rectangle {
        width: 600
        height: 200
        border.color: "lightblue"
        border.width: 2

        Text {
            anchors.centerIn: parent
            text: month_date.toLocaleDateString()
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true

            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    month_list_model.showUp();
                }

                if (wheel.angleDelta.y < 0 && index !== 5) {
                    month_list_model.showDown();
                }
            }
        }
    }
}
