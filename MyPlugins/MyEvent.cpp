#include "MyEvent.h"

MyEvent::MyEvent(QObject *parent) :
  QObject(parent)
{
}

MyEvent::MyEvent(const QOrganizerItem &item)
{
//  m_item_id = item.id().toString();
  m_description = item.description();
  m_display_label = item.displayLabel();
}

MyEvent::MyEvent(const QString &description, const QString& display_label)
{
  m_description = description;
  m_display_label = display_label;
}
