#include "myplugins_plugin.h"
#include "eventutils.h"
#include "eventlistutils.h"
#include "MyEvent.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Utils.Events
  qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
  qmlRegisterType<EventListUtils>(uri, 1, 0, "EventListUtils");
  qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
  qRegisterMetaType<QDeclarativeOrganizerItem*>();
  qRegisterMetaType<QObject*>();
  qRegisterMetaType<MyEvent*>();
  qRegisterMetaType< QList<QObject*> >();
  qRegisterMetaType< QList<MyEvent*> >();
//  qRegisterMetaType< QList<QDeclarativeOrganizerItem*> >();
}


