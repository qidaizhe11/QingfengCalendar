# Add more folders to ship with the application, here
folder_01.source = qml/QingfengCalendar
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += core gui widgets organizer versit webkitwidgets

#LIBS        += /usr/lib/libical.so.1

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
#    Database/CalendarContract.cpp \
#    Database/CalendarInstancesHelper.cpp \
#    Database/Database.cpp \
#    Database/Event.cpp \
#    Database/EventRecurrence.cpp \
#    Database/RecurrenceProcessor.cpp \
    CalendarPlugin/Private/qquickrangeddate.cpp \
    CalendarPlugin/Private/private_plugin.cpp \
    CalendarPlugin/Private/qquickcalendarmodel.cpp \
    CalendarPlugin/Private/qquickabstractstyle.cpp \
    MyPlugins/MyEventModel.cpp \
    MyPlugins/MyTestEvent.cpp \
    MyPlugins/EventUtils.cpp \
    versitorganizer/qversitorganizerimporter_p.cpp \
    versitorganizer/qversitorganizerhandler.cpp \
    versitorganizer/qversitorganizerexporter.cpp \
    versitorganizer/qversitorganizerexporter_p.cpp \
    versitorganizer/qtimezones_p.cpp \
    versitorganizer/qversitorganizerpluginloader_p.cpp \
    versitorganizer/qversitorganizerimporter.cpp \
    Google/OAuth2.cpp \
    Google/LoginDialog.cpp \
    Google/MainWindow.cpp \
    Google/GoogleSettings.cpp \
    MyPlugins/MyCollection.cpp \
    Google/GoogleManager.cpp \
    MyPlugins/MyWeekModel.cpp \
    MyPlugins/MyRecurrenceRule.cpp \
    MyPlugins/MyItem.cpp \
    MyPlugins/MyItemDetail.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizercollection.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizeritem.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizeritemdetail.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizeritemfetchhint.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizeritemfilter.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizeritemsortorder.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizermodel.cpp \
    QDeclarativeOrganizer/qdeclarativeorganizerrecurrencerule.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
#    Database/CalendarContract.h \
#    Database/CalendarInstancesHelper.h \
#    Database/Database.h \
#    Database/Event.h \
#    Database/EventRecurrence.h \
#    Database/RecurrenceProcessor.h \
    CalendarPlugin/Private/private_plugin.h \
    CalendarPlugin/Private/qquickrangeddate_p.h \
    CalendarPlugin/Private/qquickcalendarmodel_p.h \
    CalendarPlugin/Private/qquickabstractstyle_p.h \
    CalendarPlugin/Private/qquickpadding_p.h \
    CalendarPlugin/Private/qquickcontrolsprivate_p.h \
    MyPlugins/MyEventModel.h \
    MyPlugins/MyTestEvent.h \
    MyPlugins/EventUtils.h \
    versitorganizer/qversitorganizerhandler.h \
    versitorganizer/qversitorganizerglobal.h \
    versitorganizer/qversitorganizerexporter.h \
    versitorganizer/qversitorganizerexporter_p.h \
    versitorganizer/qversitorganizerdefs_p.h \
    versitorganizer/qtimezones_p.h \
    versitorganizer/qversittimezonehandler.h \
    versitorganizer/qversitorganizerpluginsearch_p.h \
    versitorganizer/qversitorganizerpluginloader_p.h \
    versitorganizer/qversitorganizerimporter.h \
    versitorganizer/qversitorganizerimporter_p.h \
    Google/OAuth2.h \
    Google/LoginDialog.h \
    Google/MainWindow.h \
    Google/GoogleSettings.h \
    MyPlugins/MyCollection.h \
    Google/GoogleManager.h \
    MyPlugins/MyWeekModel.h \
    MyPlugins/MyRecurrenceRule.h \
    MyPlugins/MyItem.h \
    MyPlugins/MyItemDetail.h \
    QDeclarativeOrganizer/qdeclarativeorganizercollection_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizeritem_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizeritemdetail_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizeritemfetchhint_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizeritemfilter_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizeritemsortorder_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizermodel_p.h \
    QDeclarativeOrganizer/qdeclarativeorganizerrecurrencerule_p.h

RESOURCES += \
    resources/Calendar.qrc

OTHER_FILES += \
    README.md \
    qml/QingfengCalendar/MonthEventLabel.qml
