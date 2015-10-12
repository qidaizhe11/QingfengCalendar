
#include "DatabaseModule.h"
//#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QtOrganizer/qorganizer.h>
#include <QtVersitOrganizer/qversitorganizerexporter.h>
#include "Database.h"
//#include "CalendarContract.h"

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

namespace
{
//    const QString insertCollectionSql(
//            "insert into " + Tables::CALENDARS +
//            " values (" + ":" + Calendars::NAME + ", :" + Calendars::ACCOUNT_NAME + ")");

    const QString insertCollectionSql(
            "insert or ignore into Calendars "
            "(collection_id, calendar_id, account_name, account_type, "
            " name, calendar_description, calendar_color, calendar_access_level, "
            "calendar_timezone) "
            "values "
            "(:collection_id, :calendar_id, :account_name, :account_type, "
            " :name, :calendar_description, :calendar_color, :calendar_access_level, "
            " :calendar_timezone)");

    const QString insertEventSql(
            "insert or replace into Events "
            "(calendar_id, calendar_name, title, guid, eventLocation, description, allDay,"
            "dtstart, dtend, rrule)"
            " values "
            "(:calendar_id, :calendar_name, :title, :guid, :eventLocation, :description, "
            ":allDay, :dtstart, :dtend, :rrule)");

    const QString selectEventSql(
                "select * from Events order by _id");

    const QString selectCalendarIdOfEventByCalendarNameSql(
                "select _id from Calendars c where c.calendar_id = :calendar_name");
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
    QOrganizerEvent event = static_cast<QOrganizerEvent>(p_event->item());

    return _sqlInsertOrganizerEvent(event);
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
//            "(calendar_id, calendar_name, title, guid, eventLocation, description, allDay,"
//            "dtstart, dtend, rrule)"
//            " values "
//            "(:calendar_id, :calendar_name, :title, :guid, :eventLocation, :description, "
//            ":allDay, :dtstart, :dtend, :rrule)");

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
    //query.bindValue(":item_id", event.id().toString());
    //query.bindValue(":collection_id", event.collectionId().toString());
//    query.bindValue(":calendar_id", 0);
    int calendar_id = 0;
//    QString calendar_name;
    QOrganizerItemExtendedDetail extended_detail =
            event.detail(QOrganizerItemDetail::TypeExtendedDetail);
    if (extended_detail.name() == "CalendarName") {
//        query.bindValue(":calendar_name", extended_detail.data().toString());
        _sqlSelectCalendarIdOfEventbyCalendarName(extended_detail.data().toString(), &calendar_id);
    }
    query.bindValue(":calendar_id", calendar_id);
    query.bindValue(":title", event.displayLabel());
    query.bindValue(":guid", event.guid());
    query.bindValue(":eventLocation", event.location());
    query.bindValue(":description", event.description());
    query.bindValue(":allDay", event.isAllDay() ? 1 : 0);
    query.bindValue(":dtstart", event.startDateTime().toMSecsSinceEpoch());
    query.bindValue(":dtend", event.endDateTime().toMSecsSinceEpoch());

    QString rrule_str = QString();
    if (!event.recurrenceRules().isEmpty()) {
        foreach (const QOrganizerRecurrenceRule& rrule, event.recurrenceRules()) {
            rrule_str = encodeRecurrenceRule(rrule);
            qDebug() << "Event rrule: " << rrule_str;
        }
    }
    query.bindValue(":rrule", rrule_str);

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseModule::_sqlSelectCalendarIdOfEventbyCalendarName(const QString &calendar_name,
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
    query.prepare(selectCalendarIdOfEventByCalendarNameSql);

    query.bindValue(":calendar_name", calendar_name);

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
        parseRecurrenceRuleStr(rrule_str, &rrule);
        event->setRecurrenceRule(rrule);
    }

    QString rdate = record.value("rdate").toString();

    return true;
}

bool DatabaseModule::parseRecurrenceRuleStr(const QString &str,
                                            QOrganizerRecurrenceRule *rule) const
{
    QStringList parts = str.split(QLatin1Char(';'));
    if (parts.size() == 0) {
        return false;
    }

    QString freq_part = parts.takeFirst();
    QStringList freq_parts = freq_part.split(QLatin1Char('='));
    if (freq_parts.size() != 2) {
        return false;
    }
    if (freq_parts.at(0) != QStringLiteral("FREQ")) {
        return false;
    }
    QString freq_value = freq_parts.at(1);
    if (freq_value == QStringLiteral("DAILY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Daily);
    } else if (freq_value == QStringLiteral("WEEKLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Weekly);
    } else if (freq_value == QStringLiteral("MONTHLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Monthly);
    } else if (freq_value == QStringLiteral("YEARLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Yearly);
    } else {
        return false;
    }

    foreach (const QString& part, parts) {
        QStringList key_value = part.split(QLatin1Char('='));
        if (key_value.size() != 2) {
            return false;
        }
        parseRecurrenceFragment(key_value.at(0), key_value.at(1), rule);
    }
    return true;
}

void DatabaseModule::parseRecurrenceFragment(const QString &key, const QString &value,
                                             QOrganizerRecurrenceRule *rule) const
{
    if (key == QStringLiteral("INTERVAL")) {
        bool ok;
        int n = value.toInt(&ok);
        if (ok && n >= 1) {
            rule->setInterval(n);
        }
    } else if (key == QStringLiteral("COUNT")) {
        bool ok;
        int count = value.toInt(&ok);
        if (ok && count >= 0) {
            rule->setLimit(count);
        }
    } else if (key == QStringLiteral("UNTIL")) {
        QDate date;
        if (value.contains(QLatin1Char('T'))) {
            QDateTime dt = parseDateTime(value);
            date = dt.date();
        } else {
            date = QDate::fromString(value, QStringLiteral("yyyyMMdd"));
        }
        if (date.isValid()) {
            rule->setLimit(date);
        }
    } else if (key == QStringLiteral("BYDAY")) {
        QSet<Qt::DayOfWeek> days;
        QStringList day_parts = value.split(QLatin1Char(','));
        foreach (QString day_str, day_parts) {
            if (day_str.length() < 2) {
                // bad day specifier
                continue;
            } else if (day_str.length() > 2) {
                // parse something like -2SU, meaning the second-last Sunday
                QString pos_str = day_str;
                day_str = day_str.right(2);
                pos_str.chop(2);
                bool ok;
                int pos = pos_str.toInt(&ok);
                if (!ok) {
                    continue;
                }
                rule->setPositions(QSet<int>() << pos);
            }
            int day = parseDayOfWeek(day_str);
            if (day != -1) {
                days << (Qt::DayOfWeek)day;
            }
        }
        if (!days.isEmpty()) {
            rule->setDaysOfWeek(days);
        }
    } else if (key == QStringLiteral("BYMONTHDAY")) {
        QSet<int> days = parseInts(value, -31, 31);
        if (!days.isEmpty()) {
            rule->setDaysOfMonth(days);
        }
    } else if (key == QStringLiteral("BYWEEKNO")) {
        QSet<int> weeks = parseInts(value, -53, 53);
        if (!weeks.isEmpty()) {
            rule->setWeeksOfYear(weeks);
        }
    } else if (key == QStringLiteral("BYMONTH")) {
        QSet<QOrganizerRecurrenceRule::Month> months;
        QStringList month_parts = value.split(QLatin1Char(','));
        foreach (const QString& month_part, month_parts) {
            bool ok;
            int month = month_part.toInt(&ok);
            if (ok && month >= 1 && month <= 12) {
                months << (QOrganizerRecurrenceRule::Month)month;
            }
        }
        if (!months.isEmpty()) {
            rule->setMonthsOfYear(months);
        }
    } else if (key == QStringLiteral("BYYEARDAY")) {
        QSet<int> days = parseInts(value, -366, 366);
        if (!days.isEmpty()) {
            rule->setDaysOfYear(days);
        }
    } else if (key == QStringLiteral("BYSETPOS")) {
        QSet<int> poss = parseInts(value, -366, 366);
        if (!poss.isEmpty()) {
            rule->setPositions(poss);
        }
    } else if (key == QStringLiteral("WKST")) {
        int day = parseDayOfWeek(value);
        if (day != -1) {
            rule->setFirstDayOfWeek((Qt::DayOfWeek)day);
        }
    }
}

QSet<int> DatabaseModule::parseInts(const QString &str, int min, int max) const
{
    QSet<int> values;
    QStringList parts = str.split(QLatin1Char(','));
    foreach (const QString& part, parts) {
        bool ok;
        int value = part.toInt(&ok);
        if (ok && value >= min && value <= max && value != 0) {
            values << value;
        }
    }

    return values;
}

int DatabaseModule::parseDayOfWeek(const QString &str) const
{
    if (str == QStringLiteral("MO")) {
        return Qt::Monday;
    } else if (str == QStringLiteral("TU")) {
        return Qt::Tuesday;
    } else if (str == QStringLiteral("WE")) {
        return Qt::Wednesday;
    } else if (str == QStringLiteral("TH")) {
        return Qt::Thursday;
    } else if (str == QStringLiteral("FR")) {
        return Qt::Friday;
    } else if ( str == QStringLiteral("SA")) {
        return Qt::Saturday;
    } else if (str == QStringLiteral("SU")) {
        return Qt::Sunday;
    } else {
        return -1;
    }
}

QDateTime DatabaseModule::parseDateTime(QString str) const
{
    bool utc = str.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive);
    if (utc) {
        str.chop(1);
    }
    QDateTime dt(QDateTime::fromString(str, QStringLiteral("yyyyMMddTHHmmss")));
    if (utc) {
        dt.setTimeSpec(Qt::UTC);
    }
    return dt;
}

QString DatabaseModule::encodeRecurrenceRule(const QOrganizerRecurrenceRule &rule) const
{
    QString value = QStringLiteral("FREQ=");
    switch (rule.frequency()) {
    case QOrganizerRecurrenceRule::Daily:
        value.append(QStringLiteral("DAILY"));
        break;
    case QOrganizerRecurrenceRule::Weekly:
        value.append(QStringLiteral("WEEKLY"));
        break;
    case QOrganizerRecurrenceRule::Monthly:
        value.append(QStringLiteral("MONTHLY"));
        break;
    case QOrganizerRecurrenceRule::Yearly:
        value.append(QStringLiteral("YEARLY"));
        break;
    case QOrganizerRecurrenceRule::Invalid:
    default:
        return QString();
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::CountLimit) {
        value.append(QStringLiteral(";COUNT="));
        value.append(QString::number(rule.limitCount()));
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::DateLimit) {
        value.append(QStringLiteral(";UNTIL="));
        value.append(rule.limitDate().toString(QStringLiteral("yyyyMMdd")));
    }
    if (rule.interval() > 1) {
        value.append(QStringLiteral(";INTERVAL="));
        value.append(QString::number(rule.interval()));
    }
    if (!rule.daysOfWeek().isEmpty()) {
        value.append(QStringLiteral(";BYDAY="));
        bool first = true;
        QList<Qt::DayOfWeek> days_of_week(QList<Qt::DayOfWeek>::fromSet(rule.daysOfWeek()));
        std::sort(days_of_week.begin(), days_of_week.end());
        foreach (Qt::DayOfWeek day, days_of_week) {
            if (!first) {
                value.append(QStringLiteral(","));
            }
            first = false;
            value.append(encodeWeekString(day));
        }
    }
    if (!rule.daysOfMonth().isEmpty()) {
        value.append(QStringLiteral(";BYMONTHDAY="));
        value.append(encodeInts(rule.daysOfMonth()));
    }
    if (!rule.daysOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYYEARDAY="));
        value.append(encodeInts(rule.daysOfYear()));
    }
    if (!rule.weeksOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYWEEKNO="));
        value.append(encodeInts(rule.weeksOfYear()));
    }
    if (!rule.monthsOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYMONTH="));
        bool first = true;
        QList<QOrganizerRecurrenceRule::Month> months(
                    QList<QOrganizerRecurrenceRule::Month>::fromSet(rule.monthsOfYear()));
        std::sort(months.begin(), months.end());
        foreach (QOrganizerRecurrenceRule::Month month, months) {
            if (!first) {
                value.append(QStringLiteral(","));
            }
            first = false;
            value.append(QString::number(month));
        }
    }
    if (!rule.positions().isEmpty()) {
        value.append(QStringLiteral(";BYSETPOS="));
        value.append(encodeInts(rule.positions()));
    }
    if (rule.firstDayOfWeek() != Qt::Monday && rule.firstDayOfWeek() > 0) {
        value.append(QStringLiteral(";WKST="));
        value.append(encodeWeekString(rule.firstDayOfWeek()));
    }

    return value;
}

QString DatabaseModule::encodeInts(const QSet<int> &ints) const
{
    QString value = QString();
    bool first = true;
    QList<int> int_list(QList<int>::fromSet(ints));
    std::sort(int_list.begin(), int_list.end());
    foreach (int n, int_list) {
        if (!first) {
            value.append(QStringLiteral(","));
        }
        first = false;
        value.append(QString::number(n));
    }
    return value;
}

QString DatabaseModule::encodeWeekString(Qt::DayOfWeek day) const
{
    switch (day) {
    case Qt::Monday:
        return QStringLiteral("MO");
    case Qt::Tuesday:
        return QStringLiteral("TU");
    case Qt::Wednesday:
        return QStringLiteral("WE");
    case Qt::Thursday:
        return QStringLiteral("TH");
    case Qt::Friday:
        return QStringLiteral("FR");
    case Qt::Saturday:
        return QStringLiteral("SA");
    case Qt::Sunday:
        return QStringLiteral("SU");
    default:
        return QString();
    }
}

