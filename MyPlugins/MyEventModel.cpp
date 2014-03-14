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
  m_reader(0),
  m_writer(0),
  m_start_date(QDateTime::currentDateTime()),
  m_end_date(QDateTime::currentDateTime()),
  m_error(QOrganizerManager::NoError)
{
  m_manager = new QOrganizerManager("memory");

//  foreach (const QString& manager, QOrganizerManager::availableManagers()) {
//    qDebug() << "Avialable manager: " << manager;
//  }

//  importEvents(QUrl("file:///home/daizhe/qidaizhe11@gmail.com-2.ics"));

//  QOrganizerCollection collection;
//  collection.setMetaData(QOrganizerCollection::KeyName, "MyCalendar");
//  m_manager->saveCollection(&collection);

//  QList<QOrganizerCollection> collections = m_manager->collections();
//  foreach (QOrganizerCollection collection, collections) {
//    qDebug() << collection.metaData(QOrganizerCollection::KeyName).toString() <<
//                collection.metaData(QOrganizerCollection::KeyDescription).toString() <<
//                collection.metaData(QOrganizerCollection::KeyColor).toString() <<
//                collection.id().managerUri() << collection.id().toString();
//  }

//  QOrganizerCollection default_collection = m_manager->defaultCollection();
//  qDebug() << "Default_collection: " << default_collection.metaData(
//                QOrganizerCollection::KeyName).toString();
}

MyEventModel::~MyEventModel()
{
  if (m_manager) {
    delete m_manager;
  }
  delete m_writer;
  delete m_reader;
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
  return m_events;
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
    foreach (QVariant var, m_events) {
      items.append(var.value<MyEvent*>()->toQOrganizerEvent());
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
  qDebug() << "MyEventModel::SaveEvent.";
//  qDebug() << "Organizer_event id: " + organizer_event.id().toString();

//  MyEvent* new_event = my_event;
  qDebug() << my_event->displayLabel() << my_event->allDay() <<
              my_event->startDateTime().toString() <<
              my_event->endDateTime().toString();

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

//  my_event->deleteLater();
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

//-------------------------------------------------------------------------
// public slots

void MyEventModel::updateEvents()
{
  qDebug() << "MyEventModel::updateEvents.";

  if (m_end_date >= m_start_date) {

//    QList<QOrganizerItem> event_items = m_manager->items(
//          QDateTime(m_start_date, QTime(0, 0, 0)),
//          QDateTime(m_end_date, QTime(23, 59, 59)),
//          QOrganizerItemFilter(), -1, sort_order_list);

    QList<QOrganizerItem> event_items = m_manager->items(
          m_start_date, m_end_date);

//    foreach (QVariant var, m_events) {
////      MyEvent* object = var.value<MyEvent*>();
////      object->deleteLater();
////      QObject::destroyed(object);
////      delete object;
//      qDebug() << "Delete Events:" << object->displayLabel() <<
//                  object->startDateTime().toString();
//    }

    for (int i = m_events.count() - 1; i >= 0; --i) {
      qDebug() << "Remove Event: " <<
                  m_events.at(i).value<MyEvent*>()->displayLabel();
      m_events.removeAt(i);
    }

    m_events.clear();


    for (int i = 0; i < event_items.length(); ++i) {
      if (event_items[i].type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(event_items[i]);
//        qDebug() << "Event Id: " + event.id().toString();

//        QStringList list = event.comments();
//        if (list.count() != 0) {
//          qDebug() << "Comments count: " + QString::number(list.count());
//        }
//        foreach (QString str, list) {
//          qDebug() << "Commnet: " + str;
//        }

//        qDebug() << "Description: " + event.description();
//        QOrganizerItemLocation location =
//            event.detail(QOrganizerItemDetail::TypeLocation);
//        qDebug() << "Location: " + location.label();

        MyEvent* my_event = new MyEvent(event);
  //      MyEvent* my_event = new MyEvent(description, display_label);

        qDebug() << "Find Event:" <<
                    my_event->displayLabel() <<
                    my_event->startDateTime().toString() <<
                    my_event->endDateTime().toString() <<
                    my_event->allDay() <<
                    my_event->location();

  //      QObject* item_object = static_cast<QObject*>(&my_event);
        m_events.append(QVariant::fromValue<QObject*>(my_event));
  //      m_events.append(QVariant::fromValue(str));
      }
    }

    qDebug() << "Find events count:" << m_events.length();
  } else {
    qDebug() << "Error: endDate < startDate";
    return;
  }
}

//-------------------------------------------------------------------------
// private slots

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
