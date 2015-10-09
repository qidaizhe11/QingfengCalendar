
#include "GooglePlugin.h"

#include "GoogleSettings.h"

#include <qqml.h>

//#include "DatabaseModule.h"
//#include "TestModule.h"

void GooglePlugin::registerTypes(const char *uri)
{
    // @uri MyCalendar.Database
    //qmlRegisterType<DatabaseModule>(uri, 1, 0, "DatabaseModule");
    qmlRegisterType<GoogleSettings>(uri, 1, 0, "GoogleSettings");

    qRegisterMetaType<QObject*>("QObject*");
    //qRegisterMetaType<QDeclarativeOrganizerModel*>("QDeclarativeOrganizerModel*");
}

