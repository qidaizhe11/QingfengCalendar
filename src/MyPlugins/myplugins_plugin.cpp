#include "myplugins_plugin.h"
#include "EventUtils.h"

#include <qqml.h>

void MyPluginsPlugin::registerTypes(const char *uri)
{
  // @uri MyCalendar.Utils
  qmlRegisterType<EventUtils>(uri, 1, 0, "EventUtils");
}


