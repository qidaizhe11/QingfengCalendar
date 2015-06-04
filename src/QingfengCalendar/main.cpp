
#include <QApplication>
//#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QVariant>
#include <QFont>

//#include "qtquick2controlsapplicationviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setFont(QFont("Microsoft YaHei"));

//    const char* private_uri = "MyCalendar.Controls.Private";
//    // @uri MyCalendar.Controls.Private
//    qmlRegisterType<QQuickAbstractStyle>(private_uri, 1, 0, "AbstractStyle");
//    qmlRegisterType<QQuickRangedDate>(private_uri, 1, 0, "RangedDate");
//    qmlRegisterType<QQuickCalendarModel>(private_uri, 1, 0, "CalendarModel");

//    const char* uri = "MyCalendar2.Events";
    // @uri MyCalendar2.Events
//    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
//    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
//    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
//    qmlRegisterType<MyCollection>(uri, 1, 0, "MyCollection");
//    qRegisterMetaType< QList<QObject*> >();
//    qRegisterMetaType< QList<MyEvent*> >();
//    qRegisterMetaType< QList<MyCollection*> >();

//    uri = "MyCalendar.Weeks";
//    // @uri MyCalendar.Weeks
//    qmlRegisterType<MyWeekModel>(uri, 1, 0, "MyWeekModel");

//    uri = "MyCalendar.Sync.Google";
//    // @uri MyCalendar.Sync.Google
//    qmlRegisterType<GoogleSettings>(uri, 1, 0, "GoogleSettings");

//    registerQtOrganizerTypes("MyCalendar.QtOrganizer");

//    QtQuick2ControlsApplicationViewer viewer;
//    viewer.addImportPath("imports");
//    viewer.addImportPath("imports/MyCalendar/MyPlugins");
//    viewer.addImportPath("imports/MyCalendar/QtOrganizer");
//    viewer.setMainQmlFile(QStringLiteral("qml/main.qml"));

    QQmlApplicationEngine engine;
    engine.addImportPath("../imports");
//    engine.addImportPath("D:\\Codes\\QingfengCalendar\\imports");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    viewer.show();

    return app.exec();
}

