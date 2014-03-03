#include "MyEvent.h"
#include <QDateTime>

MyEvent::MyEvent(QObject *parent) :
  QObject(parent)
{
//  qDebug() << QDateTime::currentDateTime();
  m_start_date_time = QDateTime::currentDateTime();
  m_end_date_time = QDateTime::currentDateTime();
  m_all_day = false;
  m_display_label = "";
//  qDebug() <<
}

MyEvent::MyEvent(const QOrganizerEvent &event, QObject* parent)
  : QObject(parent)
{
//  m_item_id = item.id().toString();
//  m_description = event.description();
  m_display_label = event.displayLabel();
  m_all_day = event.isAllDay();
  m_start_date_time = event.startDateTime();
  m_end_date_time = event.endDateTime();
}

QDateTime MyEvent::startDateTime() const
{
  return m_start_date_time;
}

//MyEvent::MyEvent(const QString &description, const QString& display_label)
//{
////  m_description = description;
//  m_display_label = display_label;
//}
