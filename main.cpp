
#include <QtGui/QGuiApplication>
#include <QQmlComponent>
#include "qtquick2applicationviewer.h"
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QtWidgets/QMessageBox>
#include <QDateTime>

#include "Database/Database.h"
//#include "Database/CalendarInstancesHelper.h"
#include "Database/Event.h"
#include "CalendarComponents/Calendar.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

//    CalendarInstancesHelper* calendar_instances_helper =
//        new CalendarInstancesHelper();

    if (!Database::init()) {
      QMessageBox::critical(0, QObject::tr("Error"),
                            Database::lastError(),
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
    }

    QList<Event>* my_events = new QList<Event>();
    Event::loadEvents(QDate::currentDate(), 31, my_events);

//    Q_ASSERT(uri == QLatin1String("org.kde.pim.calendar"));
    qmlRegisterType<Calendar>("org.kde.pim.calendar", 1, 0, "Calendar");
    qmlRegisterType<QAbstractItemModel>();
    qmlRegisterType<QAbstractListModel>();

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QingfengCalendar/mainCalendarExample.qml"));
    viewer.showExpanded();

    return app.exec();
}

