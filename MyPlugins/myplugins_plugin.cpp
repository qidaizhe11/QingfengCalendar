#include "myplugins_plugin.h"
#include "EventUtils.h"
#include "MyEventModel.h"
#include "MyEvent.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Utils.Events
  qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
  qmlRegisterType<MyEventModel>(uri, 1, 0, "MyEventModel");
  qmlRegisterType<MyEvent>(uri, 1, 0, "MyEvent");
//  qRegisterMetaType<QDeclarativeOrganizerItem*>();
  qRegisterMetaType<QObject*>();
  qRegisterMetaType<MyEvent*>();
  qRegisterMetaType< QList<QObject*> >();
  qRegisterMetaType< QList<MyEvent*> >();
//  qRegisterMetaType< QList<QDeclarativeOrganizerItem*> >();
}


