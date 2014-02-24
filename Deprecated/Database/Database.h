#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class Database : public QObject
{
  Q_OBJECT

public:
  Database();
  ~Database() {}

  static bool init();
  static void close();
  static QString lastError();

private:
  static int databaseVersion(QSqlDatabase& db);
  static bool createDatabase(QSqlDatabase& db);
  static bool upgradeDatabase(QSqlDatabase& db, int oldVersion);
  static bool alterDatabase(
    QSqlDatabase& db, const QString& script_name, const QString& message);

  static QString sm_last_error_;
};

#endif // DATABASE_H
