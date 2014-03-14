#ifndef EVENTLISTUTILS_H
#define EVENTLISTUTILS_H

#include <QQuickItem>
#include <QDateTime>
#include <QQmlProperty>
#include <QtOrganizer/QOrganizerManager>
#include <QtOrganizer/qorganizerglobal.h>
#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtVersit/QVersitReader>
#include <QtVersit/QVersitWriter>
//#include <QtVersitOrganizer/qversitorganizerglobal.h>
#include <versitorganizer/qversitorganizerglobal.h>
#include "MyEvent.h"
#include "MyCollection.h"

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

class MyEventModel : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate
             NOTIFY startDateChanged)
  Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(QVariantList events READ events NOTIFY eventsChanged)
  Q_PROPERTY(QVariant collections READ collections NOTIFY collectionsChanged)
  Q_PROPERTY(QString error READ error NOTIFY errorChanged)

  Q_ENUMS(ExportError)
  Q_ENUMS(ImportError)

public:
  enum ExportError {
    ExportNoError          = QVersitWriter::NoError,
    ExportUnspecifiedError = QVersitWriter::UnspecifiedError,
    ExportIOError          = QVersitWriter::IOError,
    ExportOutOfMemoryError = QVersitWriter::OutOfMemoryError,
    ExportNotReadyError    = QVersitWriter::NotReadyError
  };

  enum ImportError {
    ImportNoError          = QVersitReader::NoError,
    ImportUnspecifiedError = QVersitReader::UnspecifiedError,
    ImportIOError          = QVersitReader::IOError,
    ImportOutOfMemoryError = QVersitReader::OutOfMemoryError,
    ImportNotReadyError    = QVersitReader::NotReadyError,
    ImportParseError       = QVersitReader::ParseError
  };

  MyEventModel(QQuickItem* parent = 0);
  ~MyEventModel();

  QDateTime startDate() const { return m_start_date; }
  void setStartDate(const QDateTime& start_date);

  QDateTime endDate() const { return m_end_date; }
  void setEndDate(const QDateTime& end_date);

//  QList<QObject*> events() const { return m_events; }
  QVariantList events() const;
  QVariantList collections() const;

  QString error() const;

//  Q_INVOKABLE void importEvents();
  Q_INVOKABLE void importEvents(const QUrl& url,
                                const QStringList& profiles = QStringList());
  Q_INVOKABLE void exportEvents(const QUrl& url,
                                const QStringList& profiles = QStringList());

  Q_INVOKABLE void saveEvent(MyEvent* my_event);
  Q_INVOKABLE void deleteEvent(const QString& id);
  Q_INVOKABLE void deleteEvent(MyEvent* my_event);

  Q_INVOKABLE void saveCollection(MyCollection* my_collection);
  Q_INVOKABLE void removeCollection(const QString& collectionId);

  Q_INVOKABLE QVariant defaultCollection();
  Q_INVOKABLE QVariant collection(const QString& collection_id);

Q_SIGNALS:
  void startDateChanged();
  void endDateChanged();
  void eventsChanged();
  void collectionsChanged();
  void errorChanged();

  void exportCompleted(ExportError error, QUrl url);
  void importCompleted(ImportError error, QUrl url);

public slots:
  void updateEvents();

private slots:
  void onRequestStateChanged(QOrganizerAbstractRequest::State new_state);
  void startImport(QVersitReader::State state);
  void itemsExported(QVersitWriter::State state);

private:
  void checkError(const QOrganizerAbstractRequest* request);

  QOrganizerManager* m_manager;

  QDateTime m_start_date;
  QDateTime m_end_date;
  QVariantList m_events;
  QVariantList m_collections;
  QString m_default_collection_id;
  QOrganizerManager::Error m_error;
  QStringList m_import_profiles;
  QVersitReader* m_reader;
  QVersitWriter* m_writer;
  QUrl m_last_import_url;
  QUrl m_last_export_url;
};

#endif // EVENTLISTUTILS_H
