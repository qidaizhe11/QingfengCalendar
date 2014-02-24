#include "myplugins_plugin.h"
#include "eventutils.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Utils.Events
  qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
}


