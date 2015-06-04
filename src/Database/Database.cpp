
//-------------------------------------------------------------------------

#include "Database.h"
#include <QtWidgets>
#include <QtSql>

const QString DATABASE_NAME = "calendar.db";
const int DATABASE_VERSION = 600;

QString Database::sm_last_error_ = "";

//-------------------------------------------------------------------------

Database::Database()
{
}

//-------------------------------------------------------------------------
// public

bool Database::init()
{
  sm_last_error_ = "";

  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(DATABASE_NAME);

  QDir::setCurrent(QApplication::applicationDirPath());

  if (QFile::exists(DATABASE_NAME)) {
    if (!db.open()) {
      sm_last_error_ = tr("Unable to open exiting database file.");
      return false;
    }

    int dbver = databaseVersion(db);
    if (dbver > DATABASE_VERSION) {
      sm_last_error_ =
          tr("The database file is for a new version of the software.");
      return false;
    }
    else if (dbver < DATABASE_VERSION) {
      if (!upgradeDatabase(db, dbver)) {
        sm_last_error_ = tr("An error has occurred upgrading the database.");
        return false;
      }
    }
  }
  else {
    if (!db.open()) {
      sm_last_error_ = tr("Unable to create new database file.");
      return false;
    }
    if (!createDatabase(db)) {
      sm_last_error_ = tr("An error has occurred creating a new database.");
      return false;
    }
  }

  return true;
}

void Database::close()
{
  QSqlDatabase db = QSqlDatabase::database();
  if (db.isValid()) {
    if (db.isOpen())
      db.close();
    db.removeDatabase(DATABASE_NAME);
  }
}

QString Database::lastError()
{
  QString message;

  if (!sm_last_error_.isEmpty()) {
    message += QString("%1").arg(sm_last_error_);
  }
  else {
    message += tr("Unknown error");
  }

  message = tr("A database error has occurred:\n\n") + message;
  return message;
}

//-------------------------------------------------------------------------
// private

int Database::databaseVersion(QSqlDatabase &db)
{
  QSqlQuery query(db);
  query.exec("PRAGMA user_version");
  if (query.next()) {
    return query.value(0).toInt();
  }
  return 0;
}

bool Database::createDatabase(QSqlDatabase& db)
{
  bool rc = alterDatabase(db, "create", tr("Creating database..."));
  if (!rc) {
    db.close();
    QFile::remove(DATABASE_NAME);
    return false;
  }
  return true;
}

bool Database::upgradeDatabase(QSqlDatabase &db, int oldVersion)
{
  if (!QFile::copy(DATABASE_NAME, DATABASE_NAME + ".bkp")) {
    return false;
  }

  for (int i = oldVersion; i < DATABASE_VERSION; ++i) {
    QString scriptName = QString("upgrade_%1-%2").arg(i).arg(i + 1);
    if (!alterDatabase(db, scriptName, tr("Upgrading database..."))) {
      return false;
    }
  }

  return true;
}

bool Database::alterDatabase(QSqlDatabase& db,
                             const QString& script_name,
                             const QString& message)
{
  qDebug() << "[ALTER DATABASE] Executing:" << script_name;

  QProgressDialog progress;
  progress.setWindowModality(Qt::WindowModal);
  progress.setLabelText(message);

  QStringList queries;

  QFile file(QString(":/sql/%1.sql").arg(script_name));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return false;
  }
  QTextStream text_stream(&file);
  QString buffer = text_stream.readAll();
  file.close();
  queries << buffer.split("\n\n", QString::SkipEmptyParts);

  QFile file1(QString(":/sql/%1_init.sql").arg(script_name));
  if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream text_stream_1(&file1);
    buffer = text_stream_1.readAll();
    file1.close();
    queries << buffer.split("\n\n", QString::SkipEmptyParts);
  }

  progress.setMinimum(0);
  progress.setMaximum(queries.count());

  QSqlQuery query(db);
  int i = 0;
  foreach (QString text, queries) {
    progress.setValue(++i);
    qApp->processEvents();

    if (!query.exec(text)) {
      qDebug() << "[ALTER DATABASE] Error executing:" << text;
      return false;
    }
  }

  return true;
}

//-------------------------------------------------------------------------
