#include "eventlistutils.h"
#include <QtOrganizer>
#include <QtOrganizer/qorganizer.h>
#include <QOrganizerManager>
#include <QList>
#include <QVariant>

EventListUtils::EventListUtils(QQuickItem* parent) :
  QQuickItem(parent)
{
}

void EventListUtils::setStartDate(const QDate &start_date)
{
  if (start_date != m_start_date && start_date.isValid()) {
    m_start_date = start_date;
  }
}

void EventListUtils::setEndDate(const QDate &end_date)
{
  if (end_date != m_end_date && end_date.isValid()) {
    m_end_date = end_date;
  }
}

QList<QVariant> EventListUtils::events() const
{
  if (m_end_date >= m_start_date) {
    QOrganizerManager manager;
    QList<QOrganizerItem> event_items = manager.items(
          QDateTime(m_start_date, QTime(0, 0, 0)),
          QDateTime(m_end_date, QTime(23, 59, 59)));

//    QList<QDeclarativeOrganizerItem> qml_event_items;
//    for (int i = 0; i < event_items.length(); ++i) {
//      qml_event_items[i].setItem(event_items.at(i));
//    }

//    return qml_event_items;
    return event_items;
  } else {
    return QList<QVariant>();
  }
}
