#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <QQuickItem>
//#include "GlobalDefine.h"

#include "qdeclarativeorganizermodel_p.h"

//NAMESPACE_BEGIN(module)

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
    Q_INVOKABLE bool sqlInsertOrganizerCollection(QDeclarativeOrganizerCollection* p_collection);
    Q_INVOKABLE bool sqlInsertOrganizerItem(QDeclarativeOrganizerItem* p_item);
    Q_INVOKABLE bool sqlInsertOrganizerEvent(QDeclarativeOrganizerEvent* p_event);

    // Event相关
//    bool sql

private:
    QDeclarativeOrganizerModel* m_organizer_model;
};

//NAMESPACE_END(module)

#endif // DATABASEMODULE_H
