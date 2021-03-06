#include "MyEventModel.h"
#include <QtOrganizer>
#include <QOrganizerManager>
#include <QVersitReader>
#include <QVersitWriter>
//#include <QtVersitOrganizer/QVersitOrganizerImporter>
//#include <QtVersitOrganizer/QVersitOrganizerExporter>
#include "versitorganizer/qversitorganizerimporter.h"
#include "versitorganizer/qversitorganizerexporter.h"
#include <QList>
#include <QVariant>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QtQml/QQmlEngine>
#include "Google/GoogleManager.h"

//-------------------------------------------------------------------------
// static local function

static QString urlToLocalFileName(const QUrl& url)
{
  if (!url.isValid()) {
    return url.toString();
  } else if (url.scheme() == "qrc") {
    return url.toString().remove(0, 5).prepend(':');
  } else {
    return url.toLocalFile();
  }
}

//-------------------------------------------------------------------------
// Construction

MyEventModel::MyEventModel(QQuickItem* parent) :
  QQuickItem(parent),
  m_manager(0),
  m_google_manager(0),
  m_start_date(QDateTime::currentDateTime()),
  m_end_date(QDateTime::currentDateTime()),
  m_error(QOrganizerManager::NoError),
  m_reader(0),
  m_writer(0)
{
  m_manager = new QOrganizerManager("memory");

  m_default_collection_id = m_manager->defaultCollection().id().toString();

//  foreach (const QString& manager, QOrganizerManager::availableManagers()) {
//    qDebug() << "Avialable manager: " << manager;
//  }

  QOrganizerCollection collection;
  collection.setId(m_manager->defaultCollection().id());
  collection.setMetaData(QOrganizerCollection::KeyName, "MyCalendar2");
  collection.setMetaData(QOrganizerCollection::KeyColor,
                         QColor("lightblue").darker(130));
//  Qt.darker("lightblue", 1.3);
  m_manager->saveCollection(&collection);

  MyCollection* my_collection = new MyCollection();
  my_collection->setCollection(collection);
  m_collections.append(my_collection);
  m_default_collection_id = my_collection->id();

  importEvents(QUrl("file:///home/daizhe/qidaizhe11@gmail.com-2.ics"));

  m_google_manager = new GoogleManager(this);

  connect(this, SIGNAL(initGoogleSync()),
          m_google_manager,SLOT(freshStartSync()));
  connect(m_google_manager, SIGNAL(calendarListReady(QVariantList)),
          this, SLOT(saveGoogleCalendars(QVariantList)));
  connect(m_google_manager, SIGNAL(eventsReady(QString,QVariantList)),
          this, SLOT(saveGoogleEvents(QString,QVariantList)));
}

MyEventModel::~MyEventModel()
{
  if (m_manager) {
    delete m_manager;
  }
  delete m_google_manager;
  delete m_writer;
  delete m_reader;
  if (!m_events.isEmpty()) {
    foreach (MyEvent* my_event, m_events) {
      delete my_event;
    }
    m_events.clear();
  }
  if (!m_collections.isEmpty()) {
    foreach (MyCollection* my_collection, m_collections) {
      delete my_collection;
    }
    m_collections.clear();
  }
}

//-------------------------------------------------------------------------
// public

void MyEventModel::setStartDate(const QDateTime &start_date)
{
  if (start_date != m_start_date && start_date.isValid()) {
    m_start_date = start_date;
  }
}

void MyEventModel::setEndDate(const QDateTime &end_date)
{
  if (end_date != m_end_date && end_date.isValid()) {
    m_end_date = end_date;
//    updateEvents();
  }
}

QVariantList MyEventModel::events() const
{
//  return QQmlListProperty<MyEvent>(this, m_events);
//  return m_events;
  QVariantList list;
  foreach (MyEvent* event, m_events) {
//    QQmlEngine::setObjectOwnership(event, QQmlEngine::JavaScriptOwnership);
    list.append(QVariant::fromValue<QObject*>(event));
  }
  return list;
}

QVariantList MyEventModel::collections() const
{
  QVariantList list;
  foreach (MyCollection* collection, m_collections) {
//    QQmlEngine::setObjectOwnership(collection, QQmlEngine::JavaScriptOwnership);
    list.append(QVariant::fromValue<QObject*>(collection));
  }
  return list;
}

QString MyEventModel::error() const
{
  if (m_manager) {
    switch (m_error) {
    case QOrganizerManager::DoesNotExistError:
      return QStringLiteral("DoesNotExist");
    case QOrganizerManager::AlreadyExistsError:
      return QStringLiteral("AlreadyExists");
    case QOrganizerManager::InvalidDetailError:
      return QStringLiteral("InvalidDetail");
    case QOrganizerManager::InvalidCollectionError:
      return QStringLiteral("InvalidCollection");
    case QOrganizerManager::LockedError:
      return QStringLiteral("LockedError");
    case QOrganizerManager::DetailAccessError:
      return QStringLiteral("DetailAccessError");
    case QOrganizerManager::PermissionsError:
      return QStringLiteral("PermissionsError");
    case QOrganizerManager::OutOfMemoryError:
      return QStringLiteral("OutOfMemory");
    case QOrganizerManager::NotSupportedError:
      return QStringLiteral("NotSupported");
    case QOrganizerManager::BadArgumentError:
      return QStringLiteral("BadArgument");
    case QOrganizerManager::UnspecifiedError:
      return QStringLiteral("UnspecifiedError");
    case QOrganizerManager::LimitReachedError:
      return QStringLiteral("LimitReached");
    case QOrganizerManager::InvalidItemTypeError:
      return QStringLiteral("InvalidItemType");
    case QOrganizerManager::InvalidOccurrenceError:
      return QStringLiteral("InvalidOccurrence");
    default:
      break;
    }
  }

  return QStringLiteral("NoError");
}

//-------------------------------------------------------------------------
// Q_INVOKABLE functions

//
// importEvents
//

void MyEventModel::importEvents(const QUrl &url, const QStringList &profiles)
{
  ImportError import_error = ImportNotReadyError;

  if (!m_reader || m_reader->state() != QVersitReader::ActiveState) {
    m_import_profiles = profiles;

    QFile* file = new QFile(urlToLocalFileName(url));
    if (file->open(QIODevice::ReadOnly)) {
      if (!m_reader) {
        m_reader = new QVersitReader;
        connect(m_reader, SIGNAL(stateChanged(QVersitReader::State)),
                this, SLOT(startImport(QVersitReader::State)));
      }
      m_reader->setDevice(file);
      if (m_reader->startReading()) {
        m_last_import_url = url;
        return;
      }
      import_error = MyEventModel::ImportError(m_reader->error());
    } else {
      import_error = ImportIOError;
    }
  }

  qDebug() << "MyEventModel::importEvents." << "Url:" << url <<
              ", Error:" << import_error;
  emit importCompleted(import_error, url);
}

//
// exportEvents
//

void MyEventModel::exportEvents(const QUrl &url, const QStringList &profiles)
{
  ExportError export_error = ExportNotReadyError;

  if (!m_writer ||  m_writer->state() != QVersitWriter::ActiveState) {
    QString profile = profiles.isEmpty() ? QString() : profiles.at(0);

    QVersitOrganizerExporter exporter(profile);
    QList<QOrganizerItem> items;

    foreach (MyEvent* event, m_events) {
      items.append(event->toQOrganizerEvent());
    }

    exporter.exportItems(items, QVersitDocument::ICalendar20Type);
    QVersitDocument document = exporter.document();
    QFile* file = new QFile(urlToLocalFileName(url));
    if (file->open(QIODevice::ReadWrite)) {
      if (!m_writer) {
        m_writer = new QVersitWriter;
        connect(m_writer, SIGNAL(stateChanged(QVersitWriter::State)),
                this, SLOT(itemsExported(QVersitWriter::State)));
      }
      m_writer->setDevice(file);
      if (m_writer->startWriting(document)) {
        m_last_export_url = url;
        return;
      }
      export_error = MyEventModel::ExportError(m_writer->error());
    } else {
      export_error = ExportIOError;
    }
  }

  qDebug() << "MyEventModel::exportEvents. Url:" << url <<
              "Error: " << export_error;
  emit exportCompleted(export_error, url);
}

//
// saveEvent
//

void MyEventModel::saveEvent(MyEvent* my_event)
{
//  qDebug() << "MyEventModel::SaveEvent.";
//  qDebug() << "Organizer_event id: " + organizer_event.id().toString();

//  MyEvent* new_event = my_event;
//  qDebug() << my_event->displayLabel() << my_event->allDay() <<
//              my_event->startDateTime().toString() <<
//              my_event->endDateTime().toString();

  if (my_event->startDateTime().isValid()) {
    QOrganizerEvent organizer_event = my_event->toQOrganizerEvent();

//    qDebug() << "Organizer_event id: " + organizer_event.id().toString();

    QOrganizerItemSaveRequest* req = new QOrganizerItemSaveRequest(this);
    req->setManager(m_manager);
    req->setItem(organizer_event);

    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
            this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

    req->start();
  }
}

//
// deleteEvent
//

void MyEventModel::deleteEvent(const QString &id)
{
  QOrganizerItemRemoveByIdRequest* req =
      new QOrganizerItemRemoveByIdRequest(this);
  req->setManager(m_manager);

  if (id.startsWith(QString("qtorganizer:occurrence"))) {
    QMessageBox::information(NULL, tr("Remove failed"),
        tr("Can't remove an occurrence item, please modify the parent "
           "item's recurrence rule instead!"));
    return;
  }

  QOrganizerItemId item_id = QOrganizerItemId::fromString(id);
  if (!item_id.isNull()) {
    req->setItemId(item_id);
  }
  connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
          this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));
  req->start();
}

void MyEventModel::deleteEvent(MyEvent *my_event)
{
  QOrganizerItemRemoveRequest* req = new QOrganizerItemRemoveRequest(this);
  req->setManager(m_manager);
  req->setItem(my_event->toQOrganizerEvent());
  connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
          this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));
  req->start();
}

//
// saveCollection
//

void MyEventModel::saveCollection(MyCollection *my_collection)
{
  if (my_collection) {
    QOrganizerCollection collection = my_collection->toOrganizerCollection();
    QOrganizerCollectionSaveRequest* req =
        new QOrganizerCollectionSaveRequest(this);
    req->setManager(m_manager);
    req->setCollection(collection);

    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this,
            SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

    req->start();
  }
}

//
// removeCollection
//

void MyEventModel::removeCollection(const QString &collectionId)
{
  QOrganizerCollectionRemoveRequest* req =
      new QOrganizerCollectionRemoveRequest(this);
  req->setManager(m_manager);
  req->setCollectionId(QOrganizerCollectionId::fromString(collectionId));

  connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
          this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

  req->start();
}

//
// defaultCollection
//

MyCollection* MyEventModel::defaultCollection()
{
  MyCollection* my_collection =
      collection(m_manager->defaultCollection().id().toString());
//  QQmlEngine::setObjectOwnership(my_collection, QQmlEngine::JavaScriptOwnership);
  return my_collection;
}

MyCollection* MyEventModel::collection(const QString &collection_id)
{
//  qDebug() << "MyEventModel::collection.";
//  qDebug() << "m_collection count: " << m_collections.count();
//  qDebug() << "input collection_id:" << collection_id;

//  for( int i = 0; i < m_collections.count(); ++i) {
////    MyCollection* collection = m_collections[i];
//    qDebug() << "m_collections, id:" << m_collections[i]->id();
//    qDebug() << "m_collections, name:" << m_collections[i]->name();
//  }

  foreach (MyCollection* collection, m_collections) {
    if (collection->id() == collection_id) {
      qDebug() << "MyEventModel::collection, return.";
      qDebug() << collection;
      return collection;
    }
  }
  return 0;
}

//-------------------------------------------------------------------------
// public slots

//
// updateEvents
//

void MyEventModel::updateEvents()
{
  qDebug() << "MyEventModel::updateEvents.";

  if (m_end_date >= m_start_date) {

    QList<QOrganizerItem> event_items = m_manager->items(
          m_start_date, m_end_date);
    qDebug() << "event_items length: " << event_items.length();

    if (!m_events.isEmpty()) {
      foreach (MyEvent* my_event, m_events) {
        qDebug() << "Delete Event: " << my_event->displayLabel();
        my_event->deleteLater();
      }
      m_events.clear();
    }

    for (int i = 0; i < event_items.length(); ++i) {
      if (event_items[i].type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(event_items[i]);

        QString event_collection_name;
        QList<QOrganizerCollection> collections = m_manager->collections();
        foreach (QOrganizerCollection collection, collections) {
          if (collection.id() == event.collectionId()) {
            event_collection_name = collection.metaData(
                          QOrganizerCollection::KeyName).toString();
          }
        }

        MyEvent* my_event = new MyEvent(event);

        qDebug() << "Find Event:" <<
                    my_event->displayLabel() <<
                    my_event->startDateTime().toString("yyyy-MM-d hh:mm") <<
                    " End:" <<
                    my_event->endDateTime().toString("yyyy-MM-d hh:mm") <<
                    event_collection_name <<
                    my_event->allDay() <<
                    my_event->location();

        m_events.append(my_event);
      } else if (event_items[i].type() ==
                 QOrganizerItemType::TypeEventOccurrence) {
//        QOrganizerEventOccurrence event_occurrence =
//            static_cast<QOrganizerEventOccurrence>(event_items[i]);
        qDebug() << "Find Event Occurrence:";
      }
    }

    qDebug() << "Find events count:" << m_events.length();
  } else {
    qDebug() << "Error: endDate < startDate";
    return;
  }
}

void MyEventModel::updateCollections()
{
  foreach (MyCollection* my_collection, m_collections) {
    delete my_collection;
  }
  m_collections.clear();

  qDebug() << "MyEventModel::updateCollections.";
  foreach (QOrganizerCollection collection, m_manager->collections()) {
    MyCollection* my_collection = new MyCollection();
    my_collection->setCollection(collection);
    m_collections.append(my_collection);
  }
}

//-------------------------------------------------------------------------
// private slots

void MyEventModel::saveGoogleCalendars(QVariantList calendars)
{
  foreach (QVariant calender_var, calendars) {
    MyCollection* collection = new MyCollection();
    m_google_manager->parseCalendar(calender_var, collection);
    saveCollection(collection);
  }
  updateCollections();
}

void MyEventModel::saveGoogleEvents(const QString &cal_id, QVariantList events)
{
  qDebug() << "MyEventModel::saveGoogleEvents";
  QString collection_id;
  foreach (MyCollection* my_collection, m_collections) {
    qDebug() << "extendedId:" << my_collection->extendedId();
    qDebug() << "cal_id" << cal_id;
    if (my_collection->extendedId() == cal_id) {
      collection_id = my_collection->id();
      qDebug() << "collection_id" << collection_id;
    }
  }

  foreach (QVariant event_var, events) {
    MyEvent* event = new MyEvent();
    m_google_manager->parseEvent(event_var, event);
    event->setCollectionId(collection_id);
    saveEvent(event);
  }
//  updateEvents();
  if (events.count() != 0) {
    emit eventsChanged();
  }
}

void MyEventModel::onRequestStateChanged(
    QOrganizerAbstractRequest::State new_state)
{
  if (new_state != QOrganizerAbstractRequest::FinishedState)
    return;

  QOrganizerAbstractRequest* request =
      qobject_cast<QOrganizerAbstractRequest*>(sender());
  Q_ASSERT(request);

  checkError(request);
  request->deleteLater();
}

void MyEventModel::startImport(QVersitReader::State state)
{
  if (state == QVersitReader::FinishedState ||
      state == QVersitReader::CanceledState) {
    if (!m_reader->results().isEmpty()) {
      QVersitOrganizerImporter importer;
      importer.importDocument(m_reader->results().at(0));
      QList<QOrganizerItem> items = importer.items();
      delete m_reader->device();
      m_reader->setDevice(0);

      if (m_manager && !m_manager->saveItems(&items) &&
          m_error != m_manager->error()) {
        m_error = m_manager->error();
        emit errorChanged();
      }
    }
    qDebug() << "MyEventModel::startImport. Import Completed" <<
                m_last_import_url << MyEventModel::ImportError(m_reader->error());
    qDebug() << "Imported events count: " << m_manager->items().count();
    emit importCompleted(MyEventModel::ImportError(m_reader->error()),
                         m_last_import_url);
  }
}

void MyEventModel::itemsExported(QVersitWriter::State state)
{
  if (state == QVersitWriter::FinishedState ||
      state == QVersitWriter::CanceledState) {
    qDebug() << "MyEventModel::itemsExported. Exported Completed" <<
                m_last_export_url << MyEventModel::ExportError(m_writer->error());
    qDebug() << "Exported events count: " << m_manager->items().count();
    emit exportCompleted(MyEventModel::ExportError(m_writer->error()),
                         m_last_export_url);
    delete m_writer->device();
    m_writer->setDevice(0);
  }
}

//-------------------------------------------------------------------------
// private

void MyEventModel::checkError(const QOrganizerAbstractRequest *request)
{
  if (request && m_error != request->error()) {
    m_error = request->error();
    emit errorChanged();
  }
}
