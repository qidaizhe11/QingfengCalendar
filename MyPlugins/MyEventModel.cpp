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

MyEventModel::MyEventModel(QQuickItem* parent) :
  QQuickItem(parent),
  m_start_date(QDateTime::currentDateTime()),
  m_end_date(QDateTime::currentDateTime()),
  m_error(QOrganizerManager::NoError)
{
  m_manager = new QOrganizerManager("memory");

  importEvents();
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
// Q_INVOKABLE

void MyEventModel::importEvents()
{
  QString message_title(tr("Import of Items failed"));

  QString doc_path = QStandardPaths::standardLocations(
        QStandardPaths::HomeLocation).first();
  if (doc_path.isEmpty()) {
    doc_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  }
  if (doc_path.isEmpty()) {
    doc_path = ".";
  }

//  QString file_name = QFileDialog::getOpenFileName(
//        NULL, tr("Select iCalendar file"), doc_path,
//        tr("iCalendar files (*.ics)"));
  QString file_name = "/home/daizhe/qidaizhe11@gmail.com-2.ics";

  if (file_name.isEmpty()) {
    return;
  }

  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly) || !file.isReadable()) {
    QMessageBox::warning(NULL, message_title,
                         tr("Unable to read from file: %1").arg(file_name));
    return;
  }

  QVersitReader reader;
  reader.setDevice(&file);
  if (!reader.startReading() || !reader.waitForFinished()) {
    QMessageBox::warning(NULL, message_title,
                         tr("Versit reader failed: %1").arg(reader.error()));
    return;
  }

  QVersitOrganizerImporter importer;
  QList<QOrganizerItem> all_items;
  QString error_message;
  foreach (const QVersitDocument& document, reader.results()) {
    if (!importer.importDocument(document)) {
      error_message += tr("Import failed.");
      QMap<int, QVersitOrganizerImporter::Error>::const_iterator iterator =
          importer.errorMap().constBegin();
      while (iterator != importer.errorMap().constEnd()) {
        switch (iterator.value()) {
        case QVersitOrganizerImporter::InvalidDocumentError:
          error_message += QString(" index %1:").arg(iterator.key());
          error_message += tr("One of the documents is not an iCalendar file");
          break;
        case QVersitOrganizerImporter::EmptyDocumentError:
          error_message += QString(" index %1:").arg(iterator.key());
          error_message += tr("One of the documents is empty");
          break;
        default:
          error_message += QString(" index %1:").arg(iterator.key());
          error_message += tr("Unknown error");
        }
        ++iterator;
      }
      error_message += QLatin1String("\n");
      continue;
    }
    QList<QOrganizerItem> items = importer.items();
    foreach (const QOrganizerItem& item, items) {
      all_items.append(item);
    }
  }

  if (!error_message.isEmpty()) {
    QMessageBox::warning(NULL, message_title, error_message);
//    qDebug() << error_message;
  }

  if (!m_manager->saveItems(&all_items)) {
    qDebug() << "Save failed.";
  }
}

void MyEventModel::saveEvent(MyEvent* my_event)
{
  qDebug() << "In saveEvent function.";
//  qDebug() << "Organizer_event id: " + organizer_event.id().toString();

//  MyEvent* new_event = my_event;
  qDebug() << my_event->allDay();
  qDebug() << my_event->displayLabel();

  qDebug() << my_event->startDateTime();
  qDebug() << my_event->endDateTime();

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
// private slots

void MyEventModel::updateEvents()
{
  qDebug() << "Begin to search events from manager.";

  if (m_end_date >= m_start_date) {
//    QOrganizerItemSortOrder sort_order;
//    sort_order.setDetail(QOrganizerItemDetail::TypeEventTime,
//                           QOrganizerEventTime::FieldStartDateTime);
//    sort_order.setDirection(Qt::AscendingOrder);
//    sort_order.setCaseSensitivity(Qt::CaseInsensitive);
//    sort_order.setBlankPolicy(QOrganizerItemSortOrder::BlanksLast);
//    QList<QOrganizerItemSortOrder> sort_order_list;
//    sort_order_list.append(sort_order);

//    QList<QOrganizerItem> event_items = m_manager->items(
//          QDateTime(m_start_date, QTime(0, 0, 0)),
//          QDateTime(m_end_date, QTime(23, 59, 59)),
//          QOrganizerItemFilter(), -1, sort_order_list);

    QList<QOrganizerItem> event_items = m_manager->items(
          m_start_date, m_end_date);

    foreach (QVariant var, m_events) {
      MyEvent* object = var.value<MyEvent*>();
      object->deleteLater();
//      QObject::destroyed(object);
//      delete object;
      qDebug() << "try to delete Events: " + object->displayLabel() + " " + object->startDateTime().toString();
    }

    m_events.clear();


    for (int i = 0; i < event_items.length(); ++i) {
      if (event_items[i].type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(event_items[i]);
//        qDebug() << "Event Id: " + event.id().toString();

        QStringList list = event.comments();
        if (list.count() != 0) {
          qDebug() << "Comments count: " + QString::number(list.count());
        }
        foreach (QString str, list) {
          qDebug() << "Commnet: " + str;
        }

//        qDebug() << "Description: " + event.description();
        QOrganizerItemLocation location =
            event.detail(QOrganizerItemDetail::TypeLocation);
        qDebug() << "Location: " + location.label();

        MyEvent* my_event = new MyEvent(event);
        QString description = QString("Description: ") + QString::number(i);
        QString display_label = QString("Display: ") + QString::number(i);
  //      MyEvent* my_event = new MyEvent(description, display_label);

        qDebug() << "Event: " + my_event->displayLabel() + ", " +
                    my_event->startDateTime().toString();
//        qDebug() << "Event Id: " + my_event->itemId();

  //      QObject* item_object = static_cast<QObject*>(&my_event);
        m_events.append(QVariant::fromValue<QObject*>(my_event));
  //      m_events.append(QVariant::fromValue(str));
      }
    }

    qDebug() << "Find event numbers: " + QString::number(m_events.length());
  } else {
    qDebug() << "Error: endDate < startDate";
    return;
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

//-------------------------------------------------------------------------
// private

void MyEventModel::checkError(const QOrganizerAbstractRequest *request)
{
  if (request && m_error != request->error()) {
    m_error = request->error();
    emit errorChanged();
  }
}
