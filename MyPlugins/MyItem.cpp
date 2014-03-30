#include "MyItem.h"
#include <QDateTime>

//-------------------------------------------------------------------------

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

  foreach (MyItemDetail* detail, m_details) {
    delete detail;
  }
  m_details.clear();
  QList<QOrganizerItemDetail> details(event.details());
  foreach (const QOrganizerItemDetail& detail, details) {
    MyItemDetail* item_detail = MyItemDetailFactory::createItemDetail(
          static_cast<MyItemDetail::DetailType>(detail.type()));
    item_detail->setDetail(detail);
    m_details.append(item_detail);
  }
}

MyEvent::~MyEvent()
{
  if (!m_details.empty()) {
    foreach (MyItemDetail* detail, m_details) {
      delete detail;
    }
    m_details.clear();
  }
}

//-------------------------------------------------------------------------
// public

MyItemType::ItemType MyEvent::itemType() const
{
  foreach (MyItemDetail* detail, m_details) {
    if (MyItemDetail::ItemType == detail->type()) {
      return static_cast<MyItemType* >(detail)->itemType();
    }
  }
  return MyItemType::Undefined;
}

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

  foreach (MyItemDetail* detail, m_details) {
    QOrganizerItemDetail item_detail = detail->toOrganizerDetail();
    event.saveDetail(&item_detail);
  }

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
// Q_INVOKABLE functions

MyItemDetail* MyEvent::detail(int type)
{
  foreach (MyItemDetail* detail, m_details) {
    if (type == detail->type()) {
      MyItemDetail* event_detail =
          MyItemDetailFactory::createItemDetail(detail->type());
      event_detail->setDetail(detail->toOrganizerDetail());
      return event_detail;
    }
  }
  return 0;
}

QVariantList MyEvent::details(int type)
{
  QVariantList list;
  foreach (MyItemDetail* detail, m_details) {
    if (type == detail->type()) {
      MyItemDetail* item_detail =
          MyItemDetailFactory::createItemDetail(detail->type());
      list.append(QVariant::fromValue<QObject*>(item_detail));
    }
  }
  return list;
}

//-------------------------------------------------------------------------
// private


