
#include <QtGui/QGuiApplication>
#include <QQmlEngine>
#include "qtquick2controlsapplicationviewer.h"
#include <QQuickWindow>
#include <QQuickView>
#include <QVariant>

#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/eventutils.h"
#include "MyPlugins/MyEvent.h"
#include "MyPlugins/MyTestEvent.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const char* uri = "MyCalendar2.Utils.Events";
    // @uri MyCalendar.Utils.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
//    qRegisterMetaType<QObject*>();
    qRegisterMetaType< QList<QObject*> >();
    qRegisterMetaType< QList<MyEvent*> >();

//    MyTestEvent event;
//    event.setStartDateTime(QDateTime::currentDateTime());
//    QDateTime time = event.startDateTime();

    MyEvent my_event;
    my_event.setDisplayLabel("hello");
    my_event.setStartDateTime(QDateTime::currentDateTime());

    QDateTime temp = my_event.startDateTime();
//    qDebug() << temp;

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/mainCalendarExample.qml"));

//    QQuickView* viewer = new QQuickView;
//    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
//    viewer->setMinimumSize(QSize(800, 560));
//    viewer->setSource(QUrl::fromLocalFile("qml/QingfengCalendar/mainCalendarExample.qml"));

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

