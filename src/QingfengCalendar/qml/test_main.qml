import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import MyCalendar.MyQtOrganizer 1.0
import MyCalendar.Database 1.0

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
        Component.onCompleted: visible = true
    }

    DatabaseModule {
        id: database_module
        objectName: "database_module"
//        organizerModel: calendar.event_model
        organizerModel: organizer_model
    }

    Connections {
        target: organizer_model
        onImportCompleted: {
//            database_module.sqlSaveOrganizerMemoryDataIntoDb();
            calendar.refreshEvents();
        }
    }

    Component.onCompleted: {
        calendar.refreshEvents();

//        database_module.init();

//        database_module.sqlInitOrganizerMemoryDataFromDb();
    }
}
