#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <QQuickItem>
//#include "GlobalDefine.h"

#include "qdeclarativeorganizermodel_p.h"

//NAMESPACE_BEGIN(module)

class QSqlRecord;

class DatabaseModule : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(DatabaseModule)

    Q_PROPERTY(QDeclarativeOrganizerModel* organizerModel READ organizerModel WRITE setOrganizerModel)
public:
    DatabaseModule(QQuickItem *parent = 0);
    ~DatabaseModule();

    QDeclarativeOrganizerModel* organizerModel();
    void setOrganizerModel(QDeclarativeOrganizerModel* organizer_model);

    Q_INVOKABLE bool init();

    Q_INVOKABLE bool sqlSaveOrganizerMemoryDataIntoDb();
    Q_INVOKABLE bool sqlInitOrganizerMemoryDataFromDb();
    Q_INVOKABLE bool sqlInsertOrganizerCollection(QDeclarativeOrganizerCollection* p_collection);
//    Q_INVOKABLE bool sqlInsertOrganizerItem(QDeclarativeOrganizerItem* p_item);
    Q_INVOKABLE bool sqlInsertOrganizerEvent(QDeclarativeOrganizerEvent* p_event);

private:
    bool _sqlInsertOrganizerEvent(const QOrganizerEvent& event) const;
    bool _sqlInsertOrganizerItem(const QOrganizerItem& item) const;

    bool _sqlSelectCalendarIdOfEvent(const QString& calendar_id_text, int* calendar_id) const;

    bool _sqlCreateOrganizerEventFromDb(const QSqlRecord& record, QOrganizerEvent* event);

private:
    QDeclarativeOrganizerModel* m_organizer_model;
};

//NAMESPACE_END(module)

#endif // DATABASEMODULE_H
