
#include "DatabaseModule.h"
//#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "Database.h"
//#include "CalendarContract.h"

namespace
{
//    const QString insertCollectionSql(
//            "insert into " + Tables::CALENDARS +
//            " values (" + ":" + Calendars::NAME + ", :" + Calendars::ACCOUNT_NAME + ")");
    const QString insertCollectionSql(
            "insert into Calendars (name, account_name) values (:name, :account_name)");

    const QString insertEventSql(
            "insert into Events (title, eventLocation, description) values "
            "(:title, :eventLocation, :description)");
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

    qDebug() << "collection count:" << m_organizer_model->calendars().count();
    qDebug() << "calendars count:" << m_organizer_model->calendars().count();

//    QList<QOrganizerCollection> collection_list = m_organizer_model->calendars();

    foreach (QVariant var, m_organizer_model->calendars()) {
        QDeclarativeOrganizerCollection* collection =
                (QDeclarativeOrganizerCollection *)var.value<QObject *>();

        if (collection != NULL) {
            sqlInsertOrganizerCollection(collection);
        }
    }

    foreach (QVariant var, m_organizer_model->events()) {
        QDeclarativeOrganizerEvent* event = (QDeclarativeOrganizerEvent*)var.value<QObject*>();

        if (event != NULL) {
            sqlInsertOrganizerEvent(event);
        }
    }

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

    QSqlQuery query(db);
    query.prepare(insertCollectionSql);
//    query.bindValue(QString(":" + Calendars::NAME), p_collection->name());
//    query.bindValue(QString(":" + Calendars::ACCOUNT_NAME), p_collection->description());
    query.bindValue(":name", p_collection->name());
    query.bindValue(":account_name", p_collection->description());

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseModule::sqlInsertOrganizerItem(QDeclarativeOrganizerItem *p_item)
{
    return true;
}

bool DatabaseModule::sqlInsertOrganizerEvent(QDeclarativeOrganizerEvent* p_event)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid()) {
        return false;
    }

    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare(insertEventSql);
    query.bindValue(":title", p_event->displayLabel());
    query.bindValue(":eventLocation", p_event->location());
    query.bindValue(":description", p_event->description());

    if (!query.exec()) {
        qDebug() << "SQL exec error, lastError:" << query.lastError().text();
        return false;
    }

    return true;
}

