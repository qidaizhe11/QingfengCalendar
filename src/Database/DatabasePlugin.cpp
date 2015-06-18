#include "DatabasePlugin.h"

#include <qqml.h>

#include "DatabaseModule.h"
//#include "TestModule.h"

void MyPluginsPlugin::registerTypes(const char *uri)
{
    // @uri MyCalendar.Database
    qmlRegisterType<DatabaseModule>(uri, 1, 0, "DatabaseModule");
    //    qmlRegisterType<TestModule>(uri, 1, 0, "TestModule");

    qRegisterMetaType<QObject*>("QObject*");
    qRegisterMetaType<QDeclarativeOrganizerModel*>("QDeclarativeOrganizerModel*");
}

