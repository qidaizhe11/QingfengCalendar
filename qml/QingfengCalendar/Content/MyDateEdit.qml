import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {
    id: item
    width: 300
    height: date_edit.height
    z: 1
    state: ""

    property date selected_date: new Date()

    Button {
        id: date_edit
        anchors.fill: parent

        text: selected_date.toLocaleDateString();

        style: button_style

        onClicked: {
//            parent.state = (parent.state === "drop_down" ? "" : "drop_down")
            if (parent.state === "") {
                calendar.visibleMonth = selected_date.getMonth();
                calendar.visibleYear = selected_date.getFullYear();
                calendar.selectedDate = selected_date;
                parent.state = "drop_down";
            } else {
                calendar.visibleMonth = selected_date.getMonth();
                calendar.visibleYear = selected_date.getFullYear();
                calendar.selectedDate = selected_date;
                parent.state = "";
            }
        }
    }

    Rectangle {
        id: popup
        anchors.top: date_edit.bottom
        anchors.left: date_edit.left
        anchors.leftMargin: 1
        y: date_edit.height
        width: date_edit.width - 2
        visible: false

        MouseArea {
            id: mouse_area
            anchors.fill: popup
            hoverEnabled: true
            preventStealing: true
        }

        Calendar {
            id: calendar
            anchors.fill: parent
            selectedDate: item.date

            onClicked: {
                selected_date = calendar.selectedDate
                date_edit.text = selected_date.toLocaleDateString()
                item.state = ""
                console.log(selected_date)
            }
        }
    }

    property Item root: findRoot()
    function findRoot() {
        var p = parent;
        while(p.parent != undefined) {
            p = p.parent;
        }
        return p;
    }

    states: [
        State {
            name: "drop_down";
            PropertyChanges { target: popup; height: 300 }
            PropertyChanges { target: popup; visible: true }
            ParentChange { target: popup; parent: root }
        }
//        State {
//            name: ""
//            PropertyChanges { target: calendar; visibleMonth: date.getMonth() }
//            PropertyChanges { target: calendar; visibleYear: date.getFullYear() }
//        }
    ]
    transitions: Transition {
        NumberAnimation { target: popup; properties: "height";
            easing.type: Easing.OutExpo; duration: 250 }
    }

    property Component button_style: ButtonStyle {
        background: Item {
            Rectangle {
                anchors.fill: parent

                border.color: control.activeFocus ? "#47b" : "#999"
                implicitWidth: 100
                implicitHeight: 20
                gradient: Gradient {
                    GradientStop {color: control.pressed ? "#aaa" : "#fefefe" ;
                        position: 0}
                    GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3";
                        position: control.pressed ? 0.1: 1}
                }

                radius: height * 0.08

                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    color: control.hovered || control.activeFocus ? "#47b" : "white"
                    opacity: control.hovered || control.activeFocus ? 0.1 : 0
                    Behavior on opacity {NumberAnimation{ duration: 100 }}
                }
            }

            Image {
                id: imageItem
//                visible: control.menu !== null
                source: "../images/arrow-7-down.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: parent.height * 0.2
                opacity: control.enabled ? 0.6 : 0.5
            }
        }
    }
}
