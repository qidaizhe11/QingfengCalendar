import QtQuick 2.1

Rectangle {
    width: 800
    height: 600

    property date selected_dt_start: new Date()
    property date selected_dt_end: new Date()

    Component.onCompleted: {
        console.log("Dt_start: ", selected_dt_start);
        console.log("Dt_end: ", selected_dt_end);
    }
}
