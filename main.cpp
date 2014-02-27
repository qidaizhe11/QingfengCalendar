
#include <QtGui/QGuiApplication>
#include <QQmlEngine>
#include "qtquick2controlsapplicationviewer.h"
#include <QQuickWindow>
#include <QVariant>

#include "MyPlugins/MyEventList.h"
#include "MyPlugins/eventutils.h"
#include "MyPlugins/MyEvent.h"

//#include "Database/Database.h"
//#include "Database/CalendarInstancesHelper.h"
//#include "Database/Event.h"
//#include "CalendarComponents/Calendar.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    CalendarInstancesHelper* calendar_instances_helper =
//        new CalendarInstancesHelper();

//    if (!Database::init()) {
//      QMessageBox::critical(0, QObject::tr("Error"),
//                            Database::lastError(),
//                            QMessageBox::Ok,
//                            QMessageBox::NoButton);
//    }

//    QList<Event>* my_events = new QList<Event>();
//    Event::loadEvents(QDate::currentDate(), 31, my_events);

    const char* uri = "MyCalendar2.Utils.Events";
    // @uri MyCalendar.Utils.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
    qmlRegisterType<MyEventList>(uri, 1, 0, "MyEventList");
    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
    qRegisterMetaType<QObject*>();
    qRegisterMetaType<MyEvent*>();
    qRegisterMetaType< QList<QObject*> >();
    qRegisterMetaType< QList<MyEvent*> >();

//    EventListUtils* event_list = new EventListUtils();
//    event_list->setStartDate(QDate(2010, 01, 03));
//    event_list->setEndDate(QDate(2014, 11, 25));
//    qDebug() << "This. " + QString::number(event_list->events().length());

    QVariantList list;
    list << 10 << "blue" << "bottles";

//    QVariantList my_variant_list;
//    for (int i = 0; i < event_list->events().length(); ++i) {
//      my_variant_list.append(event_list->events().at(i));
//    }

//    if (event_list->events().at(0).canConvert())

//    MyEvent* event = event_list->events().at(0).value<MyEvent*>();
//    qDebug() << "MyEvent: " + QString::number(event->description());

//    qDebug() << event_list->events().at(0).value<QObject*>()
//    qDebug() << "Yes.";

    QVariantList events;
    events.append(QVariant::fromValue((QObject*)new MyEvent("Date1", "Display1")));
    events.append(QVariant::fromValue((QObject*)new MyEvent("Again and again", "O.O")));

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/mainCalendarExample.qml"));
//    QObject* style_object = viewer.rootObject()->findChild<QObject*>("calendar_style");
//    QMetaObject::invokeMethod(viewer.rootObject(), "refreshData",
//                              Q_ARG(QVariant, event_list->events()));
//    QMetaObject::invokeMethod(style_object, "refreshData",
//                              Q_ARG(QVariant, event_list->events()));
//    QMetaObject::invokeMethod(viewer.rootObject(), "refreshData",
//                              Q_ARG(QVariant, events));


    viewer.show();

    return app.exec();
}

