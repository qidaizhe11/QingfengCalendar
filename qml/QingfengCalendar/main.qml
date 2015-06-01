import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0
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
            width: parent.width
            height: parent.height
            focus: true
        }
    }

    property Component edit_view: EventEditView {
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

    GoogleOAuth {
        id: google_oauth
        anchors.fill: parent
        visible: false

        onLoginDone: {
            console.log("Login Done")
            visible = false;
            google_settings.accessToken = google_oauth.access_token;

            calendar.event_model.initGoogleSync();
        }
    }

    GoogleSettings {
        id: google_settings
    }

    Component.onCompleted: {
//        calendar.event_model.importItems("file:///home/daizhe/qidaizhe11@gmail.com-2.ics");
        calendar.refreshEvents();

//        if (google_settings.refreshToken === "") {
//            google_oauth.visible = true;
//            google_oauth.login();
//        } else {
//            google_oauth.refreshAccessToken(google_settings.refreshToken);
//        }
    }
}
