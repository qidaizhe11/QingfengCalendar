#include "MyEventModel.h"
#include <QtOrganizer>
#include <QOrganizerManager>
#include <QVersitReader>
#include <QVersitWriter>
#include <QtVersitOrganizer/QVersitOrganizerImporter>
#include <QtVersitOrganizer/QVersitOrganizerExporter>
#include <QList>
#include <QVariant>
#include <QString>

MyEventModel::MyEventModel(QQuickItem* parent) :
  QQuickItem(parent),
  m_start_date(QDateTime::currentDateTime()),
  m_end_date(QDateTime::currentDateTime()),
  m_error(QOrganizerManager::NoError)
{
  m_manager = new QOrganizerManager("memory");
  //  QString doc_path = QDesktopServices::
  //  QString file_name = QFileDialog::getOpenFileName(
  //        this, tr("Select iCalendar file"), ".", tr("iCalendar files (*.ics)"));

//  QString file_name = "/home/daizhe/organizer_ical_test.ics";
  QString file_name = "/home/daizhe/qidaizhe11@gmail.com-2.ics";

  if (file_name.isEmpty()) {
    return;
  }

  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly) || !file.isReadable()) {
    qDebug() << "Failed to open";
    return;
  }

  QVersitReader reader;
  reader.setDevice(&file);
  if (!reader.startReading() || !reader.waitForFinished()) {
    qDebug() << "Read failed";
    return;
  }

  QVersitOrganizerImporter importer;
  QList<QOrganizerItem> all_items;
  QString error_message;
  foreach (const QVersitDocument& document, reader.results()) {
    if (!importer.importDocument(document)) {
      error_message += tr("Import failed.");
      continue;
    }
    QList<QOrganizerItem> items = importer.items();
    foreach (const QOrganizerItem& item, items) {
      all_items.append(item);
    }
  }

  if (!error_message.isEmpty()) {
//    QMessageBox::warning(this, "Message", error_message);
    qDebug() << error_message;
  }

  if (!m_manager->saveItems(&all_items)) {
    qDebug() << "Save failed.";
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


// TODO: 此save函数当前极易崩溃，或者造成Event事件信息的丢失！

void MyEventModel::saveEvent(MyEvent* my_event)
{
  QOrganizerEvent organizer_event;
  qDebug() << "In saveEvent function.";
  qDebug() << my_event;

//  MyEvent* new_event = my_event;
  qDebug() << my_event->allDay();
  qDebug() << my_event->displayLabel();

  qDebug() << my_event->startDateTime();
  qDebug() << my_event->endDateTime();

//  if (my_event->startDateTime()) {
    organizer_event.setStartDateTime(my_event->startDateTime());
    organizer_event.setDisplayLabel(my_event->displayLabel());
    organizer_event.setEndDateTime(my_event->endDateTime());
    organizer_event.setAllDay(my_event->allDay());
    if (my_event->displayLabel().isEmpty()) {
      my_event->displayLabel() = tr("(No title)");
    }

    QOrganizerItemSaveRequest* req = new QOrganizerItemSaveRequest(this);
    req->setManager(m_manager);
    req->setItem(organizer_event);

    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
            this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

    req->start();

    my_event->deleteLater();

//    updateEvents();
    //    m_manager->saveItem(&organizer_event);
    //    if (m_manager->error()) {
    ////      QMessageBox::warning(this, tr("Failed!"), QString("Failed to save event!\n(error code %1)").arg(m_manager->error()));
    //    } else {
    //      updateEvents();
    //    }
//  }
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
//    QList<QOrganizerItem> event_items = m_manager->items(
//          QDateTime(m_start_date, QTime(0, 0, 0)),
//          QDateTime(m_end_date, QTime(23, 59, 59)));

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

        MyEvent* my_event = new MyEvent(event);
        QString description = QString("Description: ") + QString::number(i);
        QString display_label = QString("Display: ") + QString::number(i);
  //      MyEvent* my_event = new MyEvent(description, display_label);

        qDebug() << "Event: " + my_event->displayLabel() + ", " +
                    my_event->startDateTime().toString();

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
