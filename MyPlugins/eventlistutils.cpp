#include "eventlistutils.h"
#include <QtOrganizer>
#include <QOrganizerManager>
#include <QVersitReader>
#include <QVersitWriter>
#include <QtVersitOrganizer/QVersitOrganizerImporter>
#include <QtVersitOrganizer/QVersitOrganizerExporter>
#include <QList>
#include <QVariant>

//#define QT_NO_DEBUG_OUTPUT

EventListUtils::EventListUtils(QQuickItem* parent) :
  QQuickItem(parent)
{
  m_start_date = QDate();
  m_end_date = QDate();

  m_manager = new QOrganizerManager("memory");
  //  QString doc_path = QDesktopServices::
  //  QString file_name = QFileDialog::getOpenFileName(
  //        this, tr("Select iCalendar file"), ".", tr("iCalendar files (*.ics)"));

//  QString file_name = "/home/daizhe/organizer_ical_test.ics";
  QString file_name = "/home/daizhe/qidaizhe11@gmail.com-2.ics";

  if (file_name.isEmpty()) {
    return;
  }

//  qDebug() << file_name;

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

//  qDebug() << "All_items: " + QString::number(all_items.length());

  if (!error_message.isEmpty()) {
//    QMessageBox::warning(this, "Message", error_message);
    qDebug() << error_message;
  }

  if (!m_manager->saveItems(&all_items)) {
    qDebug() << "Save failed.";
  }
}

void EventListUtils::setStartDate(const QDate &start_date)
{
  if (start_date != m_start_date && start_date.isValid()) {
    m_start_date = start_date;
  }
//  qDebug() << "startDate: " + m_start_date.toString();
}

void EventListUtils::setEndDate(const QDate &end_date)
{
  if (end_date != m_end_date && end_date.isValid()) {
    m_end_date = end_date;
    getEvents();
  }

//  qDebug() << "endDate: " + m_end_date.toString();
}

QVariantList EventListUtils::events()
{
//  return QQmlListProperty<MyEvent>(this, m_events);
  return m_events;
}

//-------------------------------------------------------------------------
// private

void EventListUtils::getEvents()
{
  qDebug() << "Begin to search events from manager.";

  if (m_end_date >= m_start_date) {
    QOrganizerItemSortOrder sort_order;
    sort_order.setDetail(QOrganizerItemDetail::TypeEventTime,
                           QOrganizerEventTime::FieldStartDateTime);
    sort_order.setDirection(Qt::AscendingOrder);
    sort_order.setCaseSensitivity(Qt::CaseInsensitive);
    sort_order.setBlankPolicy(QOrganizerItemSortOrder::BlanksLast);
    QList<QOrganizerItemSortOrder> sort_order_list;
    sort_order_list.append(sort_order);

//    QList<QOrganizerItem> event_items = m_manager->items(
//          QDateTime(m_start_date, QTime(0, 0, 0)),
//          QDateTime(m_end_date, QTime(23, 59, 59)),
//          QOrganizerItemFilter(), -1, sort_order_list);
    QList<QOrganizerItem> event_items = m_manager->items(
          QDateTime(m_start_date, QTime(0, 0, 0)),
          QDateTime(m_end_date, QTime(23, 59, 59)));
    m_events.clear();

//    qDebug() << "Event_items length: " + QString::number(event_items.length());
//    qDebug() << "QOrganizerItem: ";
//    qDebug() << event_items.at(0).id();
//    qDebug() << event_items.at(0).description();
//    qDebug() << event_items.at(0).displayLabel();

//    QList<QObject*> events;
    for (int i = 0; i < event_items.length(); ++i) {
//      qDebug() << "Events: " + event_items[i]

      if (event_items[i].type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(event_items[i]);

        qDebug() << "Events Start: " + event.startDateTime().toString() + " " + event.displayLabel() + " , End: " +
                    event.endDateTime().toString();

        MyEvent* my_event = new MyEvent(event);
        QString description = QString("Description: ") + QString::number(i);
        QString display_label = QString("Display: ") + QString::number(i);
  //      MyEvent* my_event = new MyEvent(description, display_label);

  //      QObject* item_object = static_cast<QObject*>(&my_event);
        m_events.append(QVariant::fromValue<QObject*>(my_event));
  //      m_events.append(QVariant::fromValue(str));
  //      m_events.append(&my_event);
      }
    }

    qDebug() << "Date range: " + m_start_date.toString() + " " + m_end_date.toString();
    qDebug() << "Find event numbers: " + QString::number(m_events.length());
  } else {
    qDebug() << "Error: endDate < startDate";
    return;
  }
}
