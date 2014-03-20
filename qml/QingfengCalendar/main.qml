import QtQuick 2.1
import QtQuick.Controls 1.0
//import QtQuick.Controls.Styles 1.0
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0
//import QtOrganizer 5.0
//import MyCalendar2.Events 1.0
import MyCalendar.Sync.Google 1.0
import "Content"

Window {
    visible: true
    width: 960
    height: 600

    minimumHeight: 560
    minimumWidth: 800

    title: "Qingfeng Calendar"

    id: main_window

    StackView {
        id: stack_view
        anchors.fill: parent
        focus: true

        delegate: MyStackViewDelegate {}

        initialItem: MyCalendar {
            id: calendar
            objectName: "calendar"
            //            width: parent.width * 0.6 - row.spacing / 2
            width: parent.width
            height: parent.height
            selectedDate: new Date()
            focus: true
        }
    }

    property Component edit_view: EventEditView {
//        id: event_edit_view
//        visible: false
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Select iCalendar file")
        nameFilters: [ "icalendar files (*.ics)"]
        modality: Qt.WindowModal
        selectMultiple: false

        onAccepted: {
            console.log("FileDialog: ", file_dialog.fileUrl)
            calendar.event_model.importEvents(file_dialog.fileUrl)
        }

        visible: false
//        Component.onCompleted: visible = true
    }

    GoogleSettings {
        id: google_settings
    }

//    GoogleOAuth {
//        id: google_oauth
//        anchors.fill: parent
//        visible: false

//        onLoginDone: {
//            console.log("Login Done")
//            visible = false;
//            google_settings.accessToken = google_oauth.access_token;

//            calendar.event_model.initGoogleSync();
//        }
//    }

    Component.onCompleted: {
        calendar.refreshEvents();

//        if (google_settings.refreshToken === "") {
//            console.log("onCompleted!!")
//            google_oauth.visible = true;
//            google_oauth.login();
//        } else {
//            google_oauth.refreshAccessToken(google_settings.refreshToken);
//        }
    }
}