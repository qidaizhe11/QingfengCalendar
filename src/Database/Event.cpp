
#include "Event.h"
#include <QtSql>
#include "CalendarContract.h"
#include "CalendarInstancesHelper.h"

Event::Event()
{
}

void Event::loadEvents(const QDate &start_day, int days, QList<Event> */*events*/)
{
//    CalendarInstancesHelper* instances_helper = new CalendarInstancesHelper();

//    QDate end_day = start_day.addDays(days);
//    qint64 begin = QDateTime(start_day).toMSecsSinceEpoch();
//    qint64 end = QDateTime(QDate(end_day), QTime(0, 0)).toMSecsSinceEpoch();

//    QString SQL_SELECT_REPEAT_EVENTS = QString(
//                "select * from " + Tables::EVENTS +
//                " where " +
//                "(" + Events::DT_END + " is null)" +
//                " and " + "( " +
//                Events::DT_START + " <= " + QString::number(end) + " and " +
//                + "(" + Events::LAST_DATE + " is null or " +
//                Events::LAST_DATE + " >= " + QString::number(begin) + ")"
//                + " )");

//    //  select * from events
//    //  where
//    //  (dtend is null)
//    //  and
//    //  ( dtstart <= ? and (lastDate is null or lastDate > ?) )

//    QSqlDatabase db = QSqlDatabase::database();
//    if (db.isValid()) {
//        if (db.isOpen()) {
//            QSqlQuery query(db);
//            bool rc = query.exec(SQL_SELECT_REPEAT_EVENTS);
//            if (rc) {
//                while (query.next()) {
//                    //          QSqlRecord record = query.record();
//                    instances_helper->performInstanceExpansion(
//                                begin, end, query.record());
//                }
//            }
//        }
//    }
}
