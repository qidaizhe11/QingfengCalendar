#include "MyEventDetail.h"
#include <QtQml/QQmlInfo>
#include <QtOrganizer/QOrganizerItemId>

//-------------------------------------------------------------------------
// MyEventDetail

MyEventDetail::MyEventDetail(QObject *parent) :
  QObject(parent)
{
}

MyEventDetail::~MyEventDetail()
{
}

MyEventDetail::DetailType MyEventDetail::type() const
{
    return Undefined;
}

QVariant MyEventDetail::value(int field) const
{
    return m_detail.value(field);
}

bool MyEventDetail::setValue(int field, const QVariant &value)
{
    bool ok = m_detail.setValue(field, value);
    if (ok)
        emit detailChanged();
    return ok;
}

bool MyEventDetail::removeValue(int field)
{
    bool ok = m_detail.removeValue(field);
    if (ok)
        emit detailChanged();
    return ok;
}

QOrganizerItemDetail MyEventDetail::detail() const
{
    return m_detail;
}

void MyEventDetail::setDetail(const QOrganizerItemDetail &detail)
{
    m_detail = detail;
    emit detailChanged();
}

//-------------------------------------------------------------------------
// MyEventType

MyEventType::MyEventType(QObject *parent)
    : MyEventDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemType());
}

MyEventDetail::DetailType MyEventType::type() const
{
    return MyEventDetail::ItemType;
}

void MyEventType::setItemType(ItemType newType)
{
    if (newType != itemType()) {
        m_detail.setValue(QOrganizerItemType::FieldType,
                          static_cast<QOrganizerItemType::ItemType>(newType));
        emit valueChanged();
    }
}

MyEventType::ItemType MyEventType::itemType() const
{
    return static_cast<ItemType>(
          m_detail.value(QOrganizerItemType::FieldType).toInt());
}

//-------------------------------------------------------------------------
// MyEventParent

MyEventParent::MyEventParent(QObject *parent)
    : MyEventDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemParent());
}

MyEventDetail::DetailType MyEventParent::type() const
{
    return MyEventDetail::Parent;
}

QVariant MyEventParent::value(int field) const
{
    switch (field) {
    case FieldParentId:
    {
        QString id = parentId();
        return id.isNull() ? QVariant() : id;
    }
    case FieldOriginalDate:
    {
        QDateTime date = originalDate();
        return date.isValid() ? date : QVariant();
    }
    default:
    {
        return QVariant();
    }
    }
}

bool MyEventParent::setValue(int field, const QVariant &value)
{
    switch (field) {
    case FieldParentId:
    {
        if (value.canConvert<QString>()) {
            setParentId(value.toString());
            return true;
        }
        break;
    }
    case FieldOriginalDate:
    {
        if (value.canConvert<QDateTime>()) {
            setOriginalDate(value.toDateTime());
            return true;
        }
        break;
    }
    default:
    {
        return false;
    }
    }
    return false;
}

void MyEventParent::setOriginalDate(const QDateTime &date)
{
    if (date != originalDate()) {
        m_detail.setValue(QOrganizerItemParent::FieldOriginalDate, date.date());
        emit valueChanged();
    }
}

QDateTime MyEventParent::originalDate() const
{
    QDateTime retDateTime(
          m_detail.value(QOrganizerItemParent::FieldOriginalDate).toDate(),
          QTime(0, 0, 0, 0), Qt::UTC);
    return retDateTime;
}

void MyEventParent::setParentId(const QString &newParentId)
{
    if (newParentId != parentId()) {
        m_detail.setValue(QOrganizerItemParent::FieldParentId,
                          QVariant::fromValue(QOrganizerItemId::fromString(newParentId)));
        emit valueChanged();
    }
}

QString MyEventParent::parentId() const
{
    return m_detail.value(
          QOrganizerItemParent::FieldParentId).value<QOrganizerItemId>().toString();
}

//-------------------------------------------------------------------------
// MyEventPriority



//-------------------------------------------------------------------------
// MyEventRecurrence



//-------------------------------------------------------------------------
// MyEventTimestamp



//-------------------------------------------------------------------------
// MyEventReminder

//-------------------------------------------------------------------------
// MyEventAudibleReminder

//-------------------------------------------------------------------------
// MyEventEmailReminder

//-------------------------------------------------------------------------
// MyEventVisualReminder


//-------------------------------------------------------------------------
// MyEventAttendee



//-------------------------------------------------------------------------
// MyEventRsvp
