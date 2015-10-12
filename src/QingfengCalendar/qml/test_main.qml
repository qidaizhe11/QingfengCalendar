import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import MyCalendar.MyQtOrganizer 1.0
import MyCalendar.Database 1.0
import MyCalendar.Google 1.0

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

    OrganizerModel {
        id: organizer_model
        objectName: "organizer_model"

        startPeriod: '2006-01-01'
        endPeriod: '2016-01-01'

        autoUpdate: true

        manager: "memory"

        onModelChanged: {
//            console.log("OrganizerModel, onModelChanged.");
//            my_calendar.refreshEvents();
        }
    }

    FileDialog {
        id: file_dialog
        title: qsTr("Select iCalendar file")
        nameFilters: [ "icalendar files (*.ics)"]
        modality: Qt.WindowModal
        selectMultiple: false

        onAccepted: {
            console.log("FileDialog: ", file_dialog.fileUrl)
            calendar.event_model.importItems(file_dialog.fileUrl)
//            organizer_model.importItems(file_dialog.fileUrl)
//            database_module.sqlSaveOrganizerMemoryDataIntoDb()
        }

        visible: false
//        Component.onCompleted: visible = true
    }

    Connections {
        target: organizer_model
        onImportCompleted: {
//            database_module.sqlSaveOrganizerMemoryDataIntoDb();
            calendar.refreshEvents();
        }
    }

    DatabaseModule {
        id: database_module
        objectName: "database_module"
//        organizerModel: calendar.event_model
        organizerModel: organizer_model
    }

    GoogleOAuth {
        id: google_oauth
        anchors.fill: parent
        visible: false

        onLoginDone: {
            console.log("Login Done")
            visible = false;
            google_settings.accessToken = google_oauth.access_token;

//            calendar.event_model.initGoogleSync();
            google_manager.freshStartSync();
        }
    }

    GoogleSettings {
        id: google_settings
    }

    GoogleManager {
        id: google_manager
    }

    //
    // TODO: onCalendarListReady, onEventsReady logic fully wrong! Require fixed.
    //

    Connections {
        target: google_manager
        onCalendarListReady: {
            var calendars_var = [];
            calendars_var = calendars;
            for (var i = 0; i < calendars.length; ++i) {
                var calendar_var = calendars[i];
                var new_calendar = Qt.createQmlObject(
                            "import MyCalendar.MyQtOrganizer 1.0; Collection {}",
                            main_window);
                google_manager.parseCalendar(calendar_var, new_calendar);
                calendar.event_model.saveCollection(new_calendar);
                //console.log("new_calendar.collectionId: ", new_calendar.collectionId);
//                database_module.sqlInsertOrganizerCollection(new_calendar);
            }

            var default_collection_id = calendar.event_model.defaultCollectionId();
            var organizer_calendars = [];
            organizer_calendars = calendar.event_model.calendars;
            for( i = 0; i < organizer_calendars.length; ++i) {
                var organizer_collection = organizer_calendars[i];

                if (organizer_collection.collectionId === default_collection_id) {
                    organizer_collection.calendarId = "qingfengcalendar";
                }

                database_module.sqlInsertOrganizerCollection(organizer_collection);
            }

            calendar.refreshEvents();
        }
    }

    Connections {
        target: google_manager
        onEventsReady: {
            var events_var = [];
            events_var = events;
            for (var i = 0; i < events.length; ++i) {
                var event_var = events[i];
                var new_event = Qt.createQmlObject(
                            "import MyCalendar.MyQtOrganizer 1.0; Event {}",
                            main_window);
                google_manager.parseEvent(event_var, new_event);
//                new_event.collectionId = cal_id;
                new_event.collectionId = findCollectionIdByCalendarId(cal_id);
                new_event.calendarName = cal_id;
                calendar.event_model.saveItem(new_event);
//                database_module.sqlInsertOrganizerEvent(new_event);
            }

            calendar.refreshEvents();

            var organizer_events = [];
            organizer_events = calendar.event_model.events;
            for (i = 0; i < organizer_events.length; ++i) {
                var organizer_event = organizer_events[i];
//                console.log("organizer_event.collectionId:", organizer_event.collectionId,
//                            organizer_event.displayLabel);
                database_module.sqlInsertOrganizerEvent(organizer_event);
//                console.log("Create Event:", event.displayLabel,
//                            event.startDateTime.toLocaleString(),
//                            event.endDateTime.toLocaleString());
            }
        }
    }

    function findCollectionIdByCalendarId(cal_id) {
        var collectionId;
        var organizer_calendars = [];
        organizer_calendars = calendar.event_model.calendars;
        for(var i = 0; i < organizer_calendars.length; ++i) {
            var organizer_collection = organizer_calendars[i];
            if (organizer_collection.calendarId === cal_id) {
                collectionId = organizer_collection.collectionId;
                break;
            }
        }

        return collectionId;
    }

    Connections {
        target: calendar.event_model
        onErrorChanged: {
            console.log("calendar.event_model onErrorChanged, error: ", calendar.event_model.error);
        }
    }

    Component.onCompleted: {
        database_module.init();

        calendar.refreshEvents();

        if (google_settings.refreshToken === "") {
            google_oauth.visible = true;
            google_oauth.login();
        } else {
            google_oauth.refreshAccessToken(google_settings.refreshToken);
        }

//        database_module.init();

//        database_module.sqlInitOrganizerMemoryDataFromDb();
    }
}
