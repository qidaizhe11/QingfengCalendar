#include "DatabaseModule.h"
#include "Database.h"

NAMESPACE_BEGIN(module)
DatabaseModule* getDatabaseModule()
{
    static DatabaseModule module;
    return &module;
}
NAMESPACE_END(module)

DatabaseModule::DatabaseModule(QObject *parent) : QObject(parent)
{

}

DatabaseModule::~DatabaseModule()
{
    Database::close();
}

bool DatabaseModule::init()
{
    bool result = Database::init();

    if (result == false) {

    }

    return result;
}

