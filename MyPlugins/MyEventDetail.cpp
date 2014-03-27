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

QOrganizerItemDetail MyEventDetail::toOrganizerDetail() const
{
    return m_detail;
}

void MyEventDetail::setDetail(const QOrganizerItemDetail &detail)
{
    m_detail = detail;
    emit detailChanged();
}

//-------------------------------------------------------------------------
// MyItemType

MyItemType::MyItemType(QObject *parent)
    : MyEventDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemType());
}

MyEventDetail::DetailType MyItemType::type() const
{
    return MyEventDetail::ItemType;
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

MyEventAttendee::MyEventAttendee(QObject *parent)
    : MyEventDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerEventAttendee());
}

MyEventDetail::DetailType MyEventAttendee::type() const
{
    return MyEventDetail::EventAttendee;
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
// MyEventDetailFactory

MyEventDetail *MyEventDetailFactory::createItemDetail(
    MyEventDetail::DetailType type)
{
    MyEventDetail *itemDetail;
//    if (type == MyEventDetail::EventTime)
//        itemDetail = new QDeclarativeOrganizerEventTime;
//    else if (type == MyEventDetail::AudibleReminder)
//        itemDetail = new MyEventAudibleReminder;
//    else if (type == MyEventDetail::Comment)
//        itemDetail = new MyEventComment;
//    else if (type == MyEventDetail::Description)
//        itemDetail = new MyEventDescription;
//    else if (type == MyEventDetail::DisplayLabel)
//        itemDetail = new MyEventDisplayLabel;
//    else if (type == MyEventDetail::EmailReminder)
//        itemDetail = new MyEventEmailReminder;
//    else if (type == MyEventDetail::Guid)
//        itemDetail = new MyEventGuid;
//    else if (type == MyEventDetail::Location)
//        itemDetail = new MyEventLocation;
    if (type == MyEventDetail::Parent)
        itemDetail = new MyEventParent;
//    else if (type == MyEventDetail::Priority)
//        itemDetail = new MyEventPriority;
//    else if (type == MyEventDetail::Recurrence)
//        itemDetail = new MyEventRecurrence;
//    else if (type == MyEventDetail::Reminder)
//        itemDetail = new MyEventReminder;
//    else if (type == MyEventDetail::Tag)
//        itemDetail = new MyEventTag;
//    else if (type == MyEventDetail::Timestamp)
//        itemDetail = new MyEventTimestamp;
    else if (type == MyEventDetail::ItemType)
        itemDetail = new MyItemType;
//    else if (type == MyEventDetail::VisualReminder)
//        itemDetail = new MyEventVisualReminder;
//    else if (type == MyEventDetail::JournalTime)
//        itemDetail = new QDeclarativeOrganizerJournalTime;
//    else if (type == MyEventDetail::TodoProgress)
//        itemDetail = new QDeclarativeOrganizerTodoProgress;
//    else if (type == MyEventDetail::TodoTime)
//        itemDetail = new QDeclarativeOrganizerTodoTime;
//    else if (type == MyEventDetail::ExtendedDetail)
//        itemDetail = new MyEventExtendedDetail;
    else if (type == MyEventDetail::EventAttendee)
        itemDetail = new MyEventAttendee;
//    else if (type == MyEventDetail::EventRsvp)
//        itemDetail = new QDeclarativeOrganizerEventRsvp;
//    else if (type == MyEventDetail::Classification)
//        itemDetail = new MyEventClassification;
//    else if (type == MyEventDetail::Version)
//        itemDetail = new MyEventVersion;
    else
        itemDetail = new MyEventDetail;
    return itemDetail;
}
