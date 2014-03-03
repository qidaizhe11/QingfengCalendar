
#include <QtGui/QGuiApplication>
#include <QQmlEngine>
#include "qtquick2controlsapplicationviewer.h"
#include <QQuickWindow>
#include <QQuickView>
#include <QVariant>

#include "MyPlugins/MyEventModel.h"
#include "MyPlugins/eventutils.h"
#include "MyPlugins/MyEvent.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const char* uri = "MyCalendar2.Utils.Events";
    // @uri MyCalendar.Utils.Events
    qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
    qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
    qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
    qRegisterMetaType<QObject*>();
    qRegisterMetaType<MyEvent*>();
    qRegisterMetaType< QList<QObject*> >();
    qRegisterMetaType< QList<MyEvent*> >();

    QVariantList events;
    events.append(QVariant::fromValue((QObject*)new MyEvent("Date1", "Display1")));
    events.append(QVariant::fromValue((QObject*)new MyEvent("Again and again", "O.O")));

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

