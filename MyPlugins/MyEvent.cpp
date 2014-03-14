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
  m_collection_id = event.collectionId();
  m_display_label = event.displayLabel();
  m_all_day = event.isAllDay();
  m_start_date_time = event.startDateTime();
  m_end_date_time = event.endDateTime();
  m_description = event.description();
  m_location = event.location();
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

QString MyEvent::collectionId() const
{
  return m_collection_id.toString();
}

void MyEvent::setCollectionId(const QString &collectionId)
{
  QOrganizerCollectionId new_collection_id(
        QOrganizerCollectionId::fromString(collectionId));

  if (new_collection_id.toString() == collectionId &&
      m_collection_id.toString() != collectionId) {
    m_collection_id = new_collection_id;
    emit valueChanged();
  }
}

QOrganizerEvent MyEvent::toQOrganizerEvent() const
{
  QOrganizerEvent event;
  event.setId(m_id);
  event.setCollectionId(m_collection_id);
  event.setStartDateTime(m_start_date_time);
  event.setEndDateTime(m_end_date_time);
  event.setAllDay(m_all_day);

  if(!m_display_label.isEmpty()) {
    event.setDisplayLabel(m_display_label);
  } else {
    event.setDisplayLabel(tr("(No Title)"));
  }

  event.setDescription(m_description);
  event.setLocation(m_location);

  return event;
}

//-------------------------------------------------------------------------
// private


