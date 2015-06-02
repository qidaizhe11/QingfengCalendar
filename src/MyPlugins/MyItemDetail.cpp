#include "MyItemDetail.h"
#include <QtQml/QQmlInfo>
#include <QtOrganizer/QOrganizerItemId>

//-------------------------------------------------------------------------
// MyItemDetail

MyItemDetail::MyItemDetail(QObject *parent) :
  QObject(parent)
{
}

MyItemDetail::~MyItemDetail()
{
}

MyItemDetail::DetailType MyItemDetail::type() const
{
    return Undefined;
}

QVariant MyItemDetail::value(int field) const
{
    return m_detail.value(field);
}

bool MyItemDetail::setValue(int field, const QVariant &value)
{
    bool ok = m_detail.setValue(field, value);
    if (ok)
        emit detailChanged();
    return ok;
}

bool MyItemDetail::removeValue(int field)
{
    bool ok = m_detail.removeValue(field);
    if (ok)
        emit detailChanged();
    return ok;
}

QOrganizerItemDetail MyItemDetail::toOrganizerDetail() const
{
    return m_detail;
}

void MyItemDetail::setDetail(const QOrganizerItemDetail &detail)
{
    m_detail = detail;
    emit detailChanged();
}

//-------------------------------------------------------------------------
// MyItemType

MyItemType::MyItemType(QObject *parent)
    : MyItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemType());
}

MyItemDetail::DetailType MyItemType::type() const
{
    return MyItemDetail::ItemType;
}

void MyItemType::setItemType(ItemType newType)
{
    if (newType != ItemType()) {
        m_detail.setValue(QOrganizerItemType::FieldType,
                          static_cast<QOrganizerItemType::ItemType>(newType));
        emit valueChanged();
    }
}

MyItemType::ItemType MyItemType::itemType() const
{
    return static_cast<ItemType>(
          m_detail.value(QOrganizerItemType::FieldType).toInt());
}

//-------------------------------------------------------------------------
// MyEventParent

MyEventParent::MyEventParent(QObject *parent)
    : MyItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemParent());
}

MyItemDetail::DetailType MyEventParent::type() const
{
    return MyItemDetail::Parent;
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

MyEventAttendee::MyEventAttendee(QObject *parent)
    : MyItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerEventAttendee());
}

MyItemDetail::DetailType MyEventAttendee::type() const
{
    return MyItemDetail::EventAttendee;
}

/*!
    \qmlproperty variant EventAttendee::name

    This property holds the name of the attendee.
 */
void MyEventAttendee::setName(const QString &newName)
{
    if (name() != newName) {
        m_detail.setValue(QOrganizerEventAttendee::FieldName, newName);
        emit valueChanged();
    }
}

QString MyEventAttendee::name() const
{
    return m_detail.value(QOrganizerEventAttendee::FieldName).toString();
}

/*!
    \qmlproperty variant EventAttendee::emailAddress

    This property holds the email address of the attendee.
 */
void MyEventAttendee::setEmailAddress(const QString &newEmailAddress)
{
    if (emailAddress() != newEmailAddress) {
        m_detail.setValue(QOrganizerEventAttendee::FieldEmailAddress,
                          newEmailAddress);
        emit valueChanged();
    }
}

QString MyEventAttendee::emailAddress() const
{
    return m_detail.value(
          QOrganizerEventAttendee::FieldEmailAddress).toString();
}

/*!
    \qmlproperty variant EventAttendee::participationStatus

    This property holds the participation status of the attendee of the event. The value can be one of:
    \list
    \li EventAttendee.StatusUnknown
    \li EventAttendee.StatusAccepted
    \li EventAttendee.StatusDeclined
    \li EventAttendee.StatusTentative
    \li EventAttendee.StatusDelegated
    \li EventAttendee.StatusInProcess
    \li EventAttendee.StatusCompleted
    \endlist
 */
void MyEventAttendee::setParticipationStatus(ParticipationStatus status)
{
    if (participationStatus() != status) {
        m_detail.setValue(
              QOrganizerEventAttendee::FieldParticipationStatus, status);
        emit valueChanged();
    }
}

MyEventAttendee::ParticipationStatus MyEventAttendee::participationStatus() const
{
    return static_cast<ParticipationStatus>(m_detail.value(
                  QOrganizerEventAttendee::FieldParticipationStatus).toInt());
}

/*!
    \qmlproperty variant EventAttendee::participationRole

    This property holds the participation role of the attendee of the event.The value can be one of:
    \list
    \li EventAttendee.RoleUnknown
    \li EventAttendee.RoleOrganizer
    \li EventAttendee.RoleChairperson
    \li EventAttendee.RoleHost
    \li EventAttendee.RoleRequiredParticipant
    \li EventAttendee.RoleOptionalParticipant
    \li EventAttendee.RoleNonParticipant
    \endlist
 */
void MyEventAttendee::setParticipationRole(ParticipationRole role)
{
    if (participationRole() != role) {
        m_detail.setValue(
              QOrganizerEventAttendee::FieldParticipationRole, role);
        emit valueChanged();
    }
}

MyEventAttendee::ParticipationRole MyEventAttendee::participationRole() const
{
    return static_cast<ParticipationRole>(m_detail.value(
                  QOrganizerEventAttendee::FieldParticipationRole).toInt());
}

/*!
    \qmlproperty variant EventAttendee::attendeeId

    This property holds the unique identifier of the attendee.
 */
void MyEventAttendee::setAttendeeId(const QString &newAttendeeId)
{
    if (attendeeId() != newAttendeeId) {
        m_detail.setValue(QOrganizerEventAttendee::FieldAttendeeId,
                          newAttendeeId);
        emit valueChanged();
    }
}

QString MyEventAttendee::attendeeId() const
{
    return m_detail.value(QOrganizerEventAttendee::FieldAttendeeId).toString();
}

//-------------------------------------------------------------------------
// MyEventRsvp



//-------------------------------------------------------------------------
// MyItemDetailFactory

MyItemDetail *MyItemDetailFactory::createItemDetail(
    MyItemDetail::DetailType type)
{
    MyItemDetail *itemDetail;
//    if (type == MyItemDetail::EventTime)
//        itemDetail = new QDeclarativeOrganizerEventTime;
//    else if (type == MyItemDetail::AudibleReminder)
//        itemDetail = new MyEventAudibleReminder;
//    else if (type == MyItemDetail::Comment)
//        itemDetail = new MyEventComment;
//    else if (type == MyItemDetail::Description)
//        itemDetail = new MyEventDescription;
//    else if (type == MyItemDetail::DisplayLabel)
//        itemDetail = new MyEventDisplayLabel;
//    else if (type == MyItemDetail::EmailReminder)
//        itemDetail = new MyEventEmailReminder;
//    else if (type == MyItemDetail::Guid)
//        itemDetail = new MyEventGuid;
//    else if (type == MyItemDetail::Location)
//        itemDetail = new MyEventLocation;
    if (type == MyItemDetail::Parent)
        itemDetail = new MyEventParent;
//    else if (type == MyItemDetail::Priority)
//        itemDetail = new MyEventPriority;
//    else if (type == MyItemDetail::Recurrence)
//        itemDetail = new MyEventRecurrence;
//    else if (type == MyItemDetail::Reminder)
//        itemDetail = new MyEventReminder;
//    else if (type == MyItemDetail::Tag)
//        itemDetail = new MyEventTag;
//    else if (type == MyItemDetail::Timestamp)
//        itemDetail = new MyEventTimestamp;
    else if (type == MyItemDetail::ItemType)
        itemDetail = new MyItemType;
//    else if (type == MyItemDetail::VisualReminder)
//        itemDetail = new MyEventVisualReminder;
//    else if (type == MyItemDetail::JournalTime)
//        itemDetail = new QDeclarativeOrganizerJournalTime;
//    else if (type == MyItemDetail::TodoProgress)
//        itemDetail = new QDeclarativeOrganizerTodoProgress;
//    else if (type == MyItemDetail::TodoTime)
//        itemDetail = new QDeclarativeOrganizerTodoTime;
//    else if (type == MyItemDetail::ExtendedDetail)
//        itemDetail = new MyEventExtendedDetail;
    else if (type == MyItemDetail::EventAttendee)
        itemDetail = new MyEventAttendee;
//    else if (type == MyItemDetail::EventRsvp)
//        itemDetail = new QDeclarativeOrganizerEventRsvp;
//    else if (type == MyItemDetail::Classification)
//        itemDetail = new MyEventClassification;
//    else if (type == MyItemDetail::Version)
//        itemDetail = new MyEventVersion;
    else
        itemDetail = new MyItemDetail;
    return itemDetail;
}
