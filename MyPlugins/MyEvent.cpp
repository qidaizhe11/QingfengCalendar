#include "MyEvent.h"
#include <QDateTime>

MyEvent::MyEvent(QObject *parent) :
  QObject(parent)
{
//  m_start_date_time = QDateTime::currentDateTime();
//  m_end_date_time = QDateTime::currentDateTime();
  m_all_day = false;
  m_display_label = "";
}

MyEvent::MyEvent(const QOrganizerEvent &event, QObject* parent)
  : QObject(parent)
{
  m_id = event.id();
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

QString MyEvent::itemId() const
{
  return m_id.toString();
}

QOrganizerEvent MyEvent::toQOrganizerEvent() const
{
  QOrganizerEvent event;
  event.setId(m_id);
  event.setStartDateTime(m_start_date_time);
  event.setEndDateTime(m_end_date_time);
  event.setAllDay(m_all_day);

  if(!m_display_label.isEmpty()) {
    event.setDisplayLabel(m_display_label);
  } else {
    event.setDisplayLabel(tr("(No Title)"));
  }

  return event;
}

//-------------------------------------------------------------------------
// private


