import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {
    width: 500
    height:300

    Button {
        id: date_edit
        anchors.centerIn: parent
        width: 100
        height: 30

        text: (new Date()).toLocaleDateString();
    }
}
