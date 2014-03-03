#include "MyEvent.h"

MyEvent::MyEvent(QObject *parent) :
  QObject(parent)
{
  m_start_date_time = QDateTime();
  m_end_date_time = QDateTime();
  m_all_day = false;
  m_display_label = QString();
}

MyEvent::MyEvent(const QOrganizerEvent &event)
{
//  m_item_id = item.id().toString();
//  m_description = event.description();
  m_display_label = event.displayLabel();
  m_all_day = event.isAllDay();
  m_start_date_time = event.startDateTime();
  m_end_date_time = event.endDateTime();
}

//MyEvent::MyEvent(const QString &description, const QString& display_label)
//{
////  m_description = description;
//  m_display_label = display_label;
//}
