#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <QObject>
#include "GlobalDefine.h"

//NAMESPACE_BEGIN(module)

class DatabaseModule : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseModule(QObject *parent = 0);
    ~DatabaseModule();

    bool init();

signals:

public slots:
};

DatabaseModule* getDatabaseModule();

//NAMESPACE_END(module)

#endif // DATABASEMODULE_H
