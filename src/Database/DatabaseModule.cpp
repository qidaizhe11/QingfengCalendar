
#include "DatabaseModule.h"
//#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QtOrganizer/qorganizer.h>
#include <QtVersitOrganizer/qversitorganizerexporter.h>
#include "Database.h"
//#include "CalendarContract.h"
#include "RecurrenceRuleParser.h"

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

namespace
{
//    const QString insertCollectionSql(
//            "insert into " + Tables::CALENDARS +
//            " values (" + ":" + Calendars::NAME + ", :" + Calendars::ACCOUNT_NAME + ")");

    const QString insertCollectionSql(
            "insert or replace into Calendars "
            "(collection_id, calendar_id, account_name, account_type, "
            " name, calendar_description, calendar_color, calendar_access_level, "
            "calendar_timezone) "
            "values "
            "(:collection_id, :calendar_id, :account_name, :account_type, "
            " :name, :calendar_description, :calendar_color, :calendar_access_level, "
            " :calendar_timezone)");

    const QString insertEventSql(
            "insert or replace into Events "
            "(calendar_id, title, guid, eventLocation, description, "
            "dtstart, dtend, allDay, rrule, created, lastModified)"
            " values "
            "(:calendar_id, :title, :guid, :eventLocation, :description, "
            ":dtstart, :dtend, :allDay, :rrule, :created, :lastModified)");

    const QString selectEventSql(
                "select * from Events order by _id");

    const QString selectCalendarIdOfEventSql(
                "select _id from Calendars c where c.calendar_id = :calendar_id_text");
}

DatabaseModule::DatabaseModule(QQuickItem *parent)
    : QQuickItem(parent)
    , m_organizer_model(0)
{

}

DatabaseModule::~DatabaseModule()
{
    Database::close();
}

QDeclarativeOrganizerModel* DatabaseModule::organizerModel()
{
    return m_organizer_model;
}

void DatabaseModule::setOrganizerModel(QDeclarativeOrganizerModel *organizer_model)
{
//    QDeclarativeOrganizerModel* p = qobject_cast<QDeclarativeOrganizerModel*>(organizer_model);
    QDeclarativeOrganizerModel* p = organizer_model;

    if (p == NULL) {
        qDebug() << "!!!DatabaseModule::setOrganizerModel, null in ptr.";
    }

    if (m_organizer_model == p) {
        return;
    }
    m_organizer_model = p;
}

bool DatabaseModule::init()
{
    bool result = Database::init();

    if (result == false) {
        qDebug() << "!!!Database init failed, error: " << Database::lastError();
    }

    return result;
}

bool DatabaseModule::sqlSaveOrganizerMemoryDataIntoDb()
{
    if (m_organizer_model == NULL) {
        qDebug() << "invalid m_organizer_model ptr.";
        return false;
    }

    //qDebug() << "manager name: " << m_organizer_model->managerName();

    //qDebug() << "calendars count:" << m_organizer_model->calendars().count();

//    QList<QOrganizerCollection> collection_list = m_organizer_model->calendars();

    foreach (QVariant var, m_organizer_model->calendars()) {
        QDeclarativeOrganizerCollection* collection =
                (QDeclarativeOrganizerCollection *)var.value<QObject *>();

        if (collection != NULL) {
            sqlInsertOrganizerCollection(collection);
        }
    }

    QList<QOrganizerItem> items = m_organizer_model->managerPtr()->itemsForExport();
//    foreach (QDeclarativeOrganizerItem *di, m_organizer_model->items())
//        items.append(di->item());

//    QVariantList event_list = m_organizer_model->eventsForExport();
//    items = m_organizer_model->managerPtr()->itemsForExport();

    qDebug() << "eventsForExport count:" << items.count();

    foreach (QOrganizerItem item, items) {
//    foreach (QVariant var, event_list) {
//        QDeclarativeOrganizerItem* item = (QDeclarativeOrganizerItem*)var.value<QObject*>();

        if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
//            QDeclarativeOrganizerEventOccurrence* event_occurrence =
//                    static_cast<QDeclarativeOrganizerEventOccurrence*>(item);
        } else {
//            QDeclarativeOrganizerEvent* event =
//                    static_cast<QDeclarativeOrganizerEvent*>(item);

            QOrganizerEvent event = static_cast<QOrganizerEvent>(item);
            if (!event.isEmpty()) {
                _sqlInsertOrganizerEvent(event);
            }
        }

        items.append(item);
    }

    return true;
}

bool DatabaseModule::sqlInitOrganizerMemoryDataFromDb()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid() || !db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    bool result = query.exec(selectEventSql);
    if (result) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QOrganizerEvent* event = new QOrganizerEvent();
            bool ret = _sqlCreateOrganizerEventFromDb(record, event);
            if (ret) {
                bool rc = m_organizer_model->managerPtr()->saveItem((QOrganizerItem*)event);
                qDebug() << "item save result: " << rc;
            }
            qDebug() << "saved Event:" <<
                        event->displayLabel() <<
                        event->startDateTime().toString("yyyy-MM-d hh:mm") <<
                        " End:" <<
                        event->endDateTime().toString("yyyy-MM-d hh:mm") <<
                        event->isAllDay() <<
                        event->location() <<
                        event->guid();
            if (!event->recurrenceRules().isEmpty()) {
                qDebug() << "Event rrule:" << event->recurrenceRule();
            }
        }
    }

    qDebug() << "after InitfromDb, item count: " << m_organizer_model->itemCount();
    QList<QOrganizerItem> items = m_organizer_model->managerPtr()->itemsForExport();
    qDebug() << "itemForExport count: " << items.count();

    return true;
}

bool DatabaseModule::sqlInsertOrganizerCollection(QDeclarativeOrganizerCollection* p_collection)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid()) {
        return false;
    }

    if (!db.isOpen()) {
        return false;
    }

//    const QString insertCollectionSql(
//            "insert or ignore into Calendars "
//            "(collection_id, calendar_id, account_name, account_type, "
//            " name, calendar_description, calendar_color, calendar_access_level, "
//            "calendar_timezone) "
//            "values "
//            "(:collection_id, :calendar_id, :account_name, :account_type, "
//            " :name, :calendar_description, :calendar_color, :calendar_access_level, "
//            " :calendar_timezone)");

    QSqlQuery query(db);
    query.prepare(insertCollectionSql);
//    query.bindValue(QString(":" + Calendars::NAME), p_collection->name());
    query.bindValue(":collection_id", p_collection->id());
    query.bindValue(":calendar_id", p_collection->calendarId());
    query.bindValue(":account_name", p_collection->accountName());
    query.bindValue(":account_type", p_collection->accountType());
//    query.bindValue(":account_name", QString("11"));
    query.bindValue(":name", p_collection->name());
    query.bindValue(":calendar_description", p_collection->description());
    query.bindValue(":calendar_color", p_collection->color().name());
    query.bindValue(":calendar_access_level", p_collection->accessRole());
    query.bindValue(":calendar_timezone", p_collection->timeZone());

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    }

    return true;
}

//bool DatabaseModule::sqlInsertOrganizerItem(QDeclarativeOrganizerItem *p_item)
//{
//    Q_UNUSED(p_item);
//    return true;
//}

bool DatabaseModule::sqlInsertOrganizerEvent(QDeclarativeOrganizerEvent* p_event)
{
    QOrganizerItem item = p_event->item();
    if (item.type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(item);

        return _sqlInsertOrganizerEvent(event);
    } else {
        qDebug() << "QOrganizerItemType:" << item.type();
    }

    return true;
}

bool DatabaseModule::_sqlInsertOrganizerEvent(const QOrganizerEvent& event) const
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid()) {
        return false;
    }

    if (!db.isOpen()) {
        return false;
    }

//    const QString insertEventSql(
//            "insert or replace into Events "
//            "(calendar_id, title, guid, eventLocation, description, "
//            "dtstart, dtend, allDay, rrule, created, lastModified)"
//            " values "
//            "(:calendar_id, :title, :guid, :eventLocation, :description, "
//            ":dtstart, :dtend, :allDay, :rrule, :created, :lastModified)");

//    QOrganizerEvent event = static_cast<QOrganizerEvent>(p_event->item());

    qDebug() << "Insert Event:" <<
                event.displayLabel() <<
                event.startDateTime().toString("yyyy-MM-d hh:mm") <<
                " End:" <<
                event.endDateTime().toString("yyyy-MM-d hh:mm") <<
                event.isAllDay() <<
                event.location();

    QSqlQuery query(db);
    query.prepare(insertEventSql);

    int calendar_id = 0;
    QString created_datetime_str = QString();
    QString updated_datetime_str = QString();
    foreach (QOrganizerItemExtendedDetail detail,
             event.details(QOrganizerItemDetail::TypeExtendedDetail)) {
        if (detail.name() == "CalendarId") {
            _sqlSelectCalendarIdOfEvent(detail.data().toString(), &calendar_id);
        } else if (detail.name() == "CreatedDateTime") {
            created_datetime_str = detail.data().toDateTime().toString(Qt::ISODate);
        } else if (detail.name() == "UpdatedDateTime") {
            updated_datetime_str = detail.data().toDateTime().toString(Qt::ISODate);
        }
    }

    query.bindValue(":calendar_id", calendar_id);
    query.bindValue(":title", event.displayLabel());
    query.bindValue(":guid", event.guid());
    query.bindValue(":eventLocation", event.location());
    query.bindValue(":description", event.description());
//    query.bindValue(":dtstart", event.startDateTime().toMSecsSinceEpoch());
//    query.bindValue(":dtend", event.endDateTime().toMSecsSinceEpoch());
    query.bindValue(":dtstart", event.startDateTime().toString(Qt::ISODate));
    query.bindValue(":dtend", event.endDateTime().toString(Qt::ISODate));
    query.bindValue(":allDay", event.isAllDay() ? 1 : 0);

    QString rrule_str = QString();
    if (!event.recurrenceRules().isEmpty()) {
        foreach (const QOrganizerRecurrenceRule& rrule, event.recurrenceRules()) {
            rrule_str = RecurrenceRuleParser::encodeRecurrenceRule(rrule);
            qDebug() << "Event rrule: " << rrule_str;
        }
    }
    query.bindValue(":rrule", rrule_str);
    query.bindValue(":created", created_datetime_str);
    query.bindValue(":lastModified", updated_datetime_str);

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseModule::_sqlSelectCalendarIdOfEvent(const QString &calendar_id_text,
                                                              int* calendar_id) const
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid()) {
        return false;
    }

    if (!db.isOpen()) {
        return false;
    }

    if (calendar_id == NULL) {
        return false;
    }

//    const QString selectCalendarIdOfEventByCalendarNameSql(
//                "select _id from Calendars c where c.calendar_id = :calendar_name");

    QSqlQuery query(db);
    query.prepare(selectCalendarIdOfEventSql);

    query.bindValue(":calendar_id_text", calendar_id_text);

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    } else {  // query exec success
        while (query.next()) {
            QSqlRecord record = query.record();
            *calendar_id = record.value(0).toInt();
        }
    }

    return true;
}

bool DatabaseModule::_sqlCreateOrganizerEventFromDb(const QSqlRecord &record, QOrganizerEvent *event)
{
    if (event == NULL) {
        return false;
    }

    //QString item_id = record.value("_id").toString();
    //event->setId(QOrganizerItemId::fromString(item_id));

    //QString collection_id = record.value("collection_id").toString();
    //event->setCollectionId(QOrganizerCollectionId::fromString(collection_id));

    QString title = record.value("title").toString();
    event->setDisplayLabel(title);

    QString guid = record.value("guid").toString();
    event->setGuid(guid);

    QString event_location = record.value("eventLocaiton").toString();
    event->setLocation(event_location);

    QString description = record.value("description").toString();
    event->setDescription(description);

    qint64 dtstart_millis = record.value("dtstart").toLongLong();
    event->setStartDateTime(QDateTime::fromMSecsSinceEpoch(dtstart_millis));

    qint64 dtend_millis = record.value("dtend").toLongLong();
    event->setEndDateTime(QDateTime::fromMSecsSinceEpoch(dtend_millis));

    bool is_all_day = record.value("allDay").toInt() == 1 ? true : false;
    event->setAllDay(is_all_day);

    QString rrule_str = record.value("rrule").toString();
    if (!rrule_str.isEmpty()) {
        QOrganizerRecurrenceRule rrule;
        RecurrenceRuleParser::parseRecurrenceRuleStr(rrule_str, &rrule);
        event->setRecurrenceRule(rrule);
    }

//    QString rdate = record.value("rdate").toString();

    return true;
}

