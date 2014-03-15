#ifndef MYEVENTDETAIL_H
#define MYEVENTDETAIL_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizerglobal.h>

//QTORGANIZER_USE_NAMESPACE
using namespace QtOrganizer;

QT_BEGIN_NAMESPACE

class MyEventDetail : public QObject
{
  Q_OBJECT

  Q_ENUMS(DetailType)

  Q_PROPERTY(DetailType type READ type)
public:
  enum DetailType {
    Undefined = QOrganizerItemDetail::TypeUndefined,
////    Classification = QOrganizerItemDetail::TypeClassification,
////    Comment = QOrganizerItemDetail::TypeComment,
//    Description = QOrganizerItemDetail::TypeDescription,
//    DisplayLabel = QOrganizerItemDetail::TypeDisplayLabel,
    ItemType = QOrganizerItemDetail::TypeItemType,
//    Guid = QOrganizerItemDetail::TypeGuid,
//    Location = QOrganizerItemDetail::TypeLocation,
    Parent = QOrganizerItemDetail::TypeParent
////    Priority = QOrganizerItemDetail::TypePriority,
//    Recurrence = QOrganizerItemDetail::TypeRecurrence,
////    Tag = QOrganizerItemDetail::TypeTag,
//    Timestamp = QOrganizerItemDetail::TypeTimestamp,
////    Version = QOrganizerItemDetail::TypeVersion,
//    Reminder = QOrganizerItemDetail::TypeReminder,
//    AudibleReminder = QOrganizerItemDetail::TypeAudibleReminder,
//    EmailReminder = QOrganizerItemDetail::TypeEmailReminder,
//    VisualReminder = QOrganizerItemDetail::TypeVisualReminder,
////    ExtendedDetail = QOrganizerItemDetail::TypeExtendedDetail,
//    EventAttendee = QOrganizerItemDetail::TypeEventAttendee,
////    EventRsvp = QOrganizerItemDetail::TypeEventRsvp,
//    EventTime = QOrganizerItemDetail::TypeEventTime
////    JournalTime = QOrganizerItemDetail::TypeJournalTime,
////    TodoTime = QOrganizerItemDetail::TypeTodoTime,
////    TodoProgress = QOrganizerItemDetail::TypeTodoProgress
  };

  explicit MyEventDetail(QObject *parent = 0);
  ~MyEventDetail();

  virtual DetailType type() const;

  Q_INVOKABLE virtual QVariant value(int key) const;
  Q_INVOKABLE virtual bool setValue(int key, const QVariant& value);
  Q_INVOKABLE bool removeValue(int key);

  // non-QML APIs
  QOrganizerItemDetail detail() const;
  void setDetail(const QOrganizerItemDetail& detail);

Q_SIGNALS:
  void detailChanged();

protected:
  QOrganizerItemDetail m_detail;

private:
  Q_DISABLE_COPY(MyEventDetail)

};

class MyEventType : public MyEventDetail
{
    Q_OBJECT

    Q_ENUMS(EventTypeField)
    Q_ENUMS(EventType)

    Q_PROPERTY(EventType eventType READ eventType WRITE setEventType
               NOTIFY valueChanged)

public:
    enum EventTypeField {
        FieldType = QOrganizerItemType::FieldType
    };

    enum EventType {
        Undefined = QOrganizerItemType::TypeUndefined,
        Event = QOrganizerItemType::TypeEvent,
        EventOccurrence = QOrganizerItemType::TypeEventOccurrence
//        Todo = QOrganizerItemType::TypeTodo,
//        TodoOccurrence = QOrganizerItemType::TypeTodoOccurrence,
//        Journal = QOrganizerItemType::TypeJournal,
//        Note = QOrganizerItemType::TypeNote
    };

    MyEventType(QObject *parent = 0);

    virtual DetailType type() const;

    void setEventType(EventType newType);
    EventType eventType() const;

Q_SIGNALS:
    void valueChanged();
};

class MyEventParent : public MyEventDetail
{
  Q_OBJECT

  Q_ENUMS(ParentField)

  Q_PROPERTY(QDateTime originalDate READ originalDate WRITE setOriginalDate
             NOTIFY valueChanged)
  Q_PROPERTY(QString parentId READ parentId WRITE setParentId
             NOTIFY valueChanged)

public:
  enum ParentField {
    FieldParentId = QOrganizerItemParent::FieldParentId,
    FieldOriginalDate = QOrganizerItemParent::FieldOriginalDate
  };

  MyEventParent(QObject* parent = 0);

  virtual DetailType type() const;
  virtual QVariant value(int key) const;
  virtual bool setValue(int key, const QVariant &value);

  void setOriginalDate(const QDateTime& date);
  QDateTime originalDate() const;

  void setParentId(const QString& newParentId);
  QString parentId() const;

Q_SIGNALS:
  void valueChanged();
};

//class MyEventPriority : public MyEventDetail
//{
//    Q_OBJECT

//    Q_ENUMS(PriorityField)
//    Q_ENUMS(Priority)

//    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY valueChanged)

//public:
//    enum PriorityField {
//        FieldPriority = QOrganizerItemPriority::FieldPriority
//    };

//    enum Priority {
//        Unknown = QOrganizerItemPriority::UnknownPriority,
//        Highest = QOrganizerItemPriority::HighestPriority,
//        ExtremelyHigh = QOrganizerItemPriority::ExtremelyHighPriority,
//        VeryHigh = QOrganizerItemPriority::VeryHighPriority,
//        High = QOrganizerItemPriority::HighPriority,
//        Medium = QOrganizerItemPriority::MediumPriority,
//        Low = QOrganizerItemPriority::LowPriority,
//        VeryLow = QOrganizerItemPriority::VeryLowPriority,
//        ExtremelyLow = QOrganizerItemPriority::ExtremelyLowPriority,
//        Lowest = QOrganizerItemPriority::LowestPriority
//    };

//    MyEventPriority(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setPriority(Priority newPriority);
//    Priority priority() const;

//Q_SIGNALS:
//    void valueChanged();
//};

//class MyEventRecurrence : public MyEventDetail
//{
//    Q_OBJECT

//    Q_ENUMS(RecurrenceField)

//    Q_PROPERTY(QQmlListProperty<MyRecurrenceRule> recurrenceRules READ recurrenceRules NOTIFY recurrenceRulesChanged)
//    Q_PROPERTY(QQmlListProperty<MyRecurrenceRule> exceptionRules READ exceptionRules NOTIFY exceptionRulesChanged)
//    Q_PROPERTY(QVariantList recurrenceDates READ recurrenceDates WRITE setRecurrenceDates NOTIFY valueChanged)
//    Q_PROPERTY(QVariantList exceptionDates  READ exceptionDates WRITE setExceptionDates NOTIFY valueChanged)

//public:
//    enum RecurrenceField {
//        FieldRecurrenceRules = QOrganizerItemRecurrence::FieldRecurrenceRules,
//        FieldExceptionRules = QOrganizerItemRecurrence::FieldExceptionRules,
//        FieldRecurrenceDates = QOrganizerItemRecurrence::FieldRecurrenceDates,
//        FieldExceptionDates = QOrganizerItemRecurrence::FieldExceptionDates
//    };

//    MyEventRecurrence(QObject *parent = 0);

//    virtual DetailType type() const;
//    virtual QVariant value(int field) const;
//    virtual bool setValue(int key, const QVariant& value);

//    QQmlListProperty<MyRecurrenceRule> recurrenceRules();
//    QQmlListProperty<MyRecurrenceRule> exceptionRules();

//    void setRecurrenceDates(const QVariantList &dates);
//    QVariantList recurrenceDates() const;

//    void setExceptionDates(const QVariantList &dates);
//    QVariantList exceptionDates() const;

//Q_SIGNALS:
//    void recurrenceRulesChanged();
//    void exceptionRulesChanged();
//    void valueChanged();

//private Q_SLOTS:
//    void _saveRecurrenceRules();
//    void _saveExceptionRules();

//private:
//    static void rrule_append(QQmlListProperty<MyRecurrenceRule> *p, MyRecurrenceRule *item);
//    static void xrule_append(QQmlListProperty<MyRecurrenceRule> *p, MyRecurrenceRule *item);
//    static int  rule_count(QQmlListProperty<MyRecurrenceRule> *p);
//    static MyRecurrenceRule *rule_at(QQmlListProperty<MyRecurrenceRule> *p, int idx);
//    static void  rrule_clear(QQmlListProperty<MyRecurrenceRule> *p);
//    static void  xrule_clear(QQmlListProperty<MyRecurrenceRule> *p);

//    QList<MyRecurrenceRule*>   m_recurrenceRules;
//    QList<MyRecurrenceRule*>   m_exceptionRules;
//};

//class MyEventTimestamp : public MyEventDetail
//{
//    Q_OBJECT

//    Q_ENUMS(TimestampField)

//    Q_PROPERTY(QDateTime created READ created WRITE setCreated NOTIFY valueChanged)
//    Q_PROPERTY(QDateTime lastModified READ lastModified WRITE setLastModified NOTIFY valueChanged)

//public:
//    enum TimestampField {
//        FieldCreated = QOrganizerItemTimestamp::FieldCreated,
//        FieldLastModified = QOrganizerItemTimestamp::FieldLastModified
//    };

//    MyEventTimestamp(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setCreated(const QDateTime &timestamp);
//    QDateTime created() const;

//    void setLastModified(const QDateTime &timestamp);
//    QDateTime lastModified() const;

//Q_SIGNALS:
//    void valueChanged();
//};

//class MyEventReminder : public MyEventDetail
//{
//    Q_OBJECT

//    Q_ENUMS(ReminderField)
//    Q_ENUMS(ReminderType)

//    Q_PROPERTY(ReminderType reminderType READ reminderType
//               NOTIFY reminderChanged)
//    Q_PROPERTY(int repetitionCount READ repetitionCount WRITE setRepetitionCount
//               NOTIFY reminderChanged)
//    Q_PROPERTY(int repetitionDelay READ repetitionDelay WRITE setRepetitionDelay
//               NOTIFY reminderChanged)
//    Q_PROPERTY(int secondsBeforeStart READ secondsBeforeStart
//               WRITE setSecondsBeforeStart NOTIFY reminderChanged)

//public:
//    enum ReminderField {
//        FieldRepetitionCount = QOrganizerItemReminder::FieldRepetitionCount,
//        FieldRepetitionDelay = QOrganizerItemReminder::FieldRepetitionDelay,
//        FieldSecondsBeforeStart = QOrganizerItemReminder::FieldSecondsBeforeStart
//    };

//    enum ReminderType {
//        NoReminder = QOrganizerItemReminder::NoReminder,
//        VisualReminder = QOrganizerItemReminder::VisualReminder,
//        AudibleReminder = QOrganizerItemReminder::AudibleReminder,
//        EmailReminder = QOrganizerItemReminder::EmailReminder
//    };

//    MyEventReminder(QObject *parent = 0);

//    virtual DetailType type() const;

//    ReminderType reminderType() const;

//    void setRepetitionCount(int count);
//    int repetitionCount() const;

//    void setRepetitionDelay(int delaySeconds);
//    int repetitionDelay() const;

//    void setSecondsBeforeStart(int seconds);
//    int secondsBeforeStart() const;

//Q_SIGNALS:
//    void reminderChanged();
//};

//class MyEventAudibleReminder : public MyEventReminder
//{
//    Q_OBJECT

//    Q_ENUMS(AudibleReminderField)

//    Q_PROPERTY(QUrl dataUrl READ dataUrl WRITE setDataUrl NOTIFY valueChanged)

//public:
//    enum AudibleReminderField {
//        FieldDataUrl = QOrganizerItemAudibleReminder::FieldDataUrl
//    };

//    MyEventAudibleReminder(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setDataUrl(const QUrl &url);
//    QUrl dataUrl() const;

//Q_SIGNALS:
//    void valueChanged();
//};


//class MyEventEmailReminder : public MyEventReminder
//{
//    Q_OBJECT

//    Q_ENUMS(EmailReminderField)

//    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY valueChanged)
//    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY valueChanged)
//    Q_PROPERTY(QStringList recipients READ recipients WRITE setRecipients
//               NOTIFY valueChanged)
//    Q_PROPERTY(QVariantList attachments READ attachments WRITE setAttachments
//               NOTIFY valueChanged)

//public:
//    enum EmailReminderField {
//        FieldSubject = QOrganizerItemEmailReminder::FieldSubject,
//        FieldBody = QOrganizerItemEmailReminder::FieldBody,
//        FieldRecipients = QOrganizerItemEmailReminder::FieldRecipients,
//        FieldAttachments = QOrganizerItemEmailReminder::FieldAttachments
//    };

//    MyEventEmailReminder(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setBody(const QString &newBody);
//    QString body() const;

//    void setSubject(const QString &newSubject);
//    QString subject() const;

//    void setRecipients(const QStringList &newRecipients);
//    QStringList recipients() const;

//    void setAttachments(const QVariantList &newAttachments);
//    QVariantList attachments();

//Q_SIGNALS:
//    void valueChanged();
//};

//class MyEventVisualReminder : public MyEventReminder
//{
//    Q_OBJECT

//    Q_ENUMS(VisualReminderField)

//    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY valueChanged)
//    Q_PROPERTY(QUrl dataUrl READ dataUrl WRITE setDataUrl NOTIFY valueChanged)

//public:
//    enum VisualReminderField {
//        FieldDataUrl = QOrganizerItemVisualReminder::FieldDataUrl,
//        FieldMessage = QOrganizerItemVisualReminder::FieldMessage
//    };

//    MyEventVisualReminder(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setMessage(const QString &msg);
//    QString message() const;

//    void setDataUrl(const QUrl &url);
//    QUrl dataUrl() const;

//Q_SIGNALS:
//    void valueChanged();
//};

//class MyEventAttendee : public MyEventDetail
//{
//    Q_OBJECT

//    Q_ENUMS(EventAttendeeField)
//    Q_ENUMS(ParticipationStatus)
//    Q_ENUMS(ParticipationRole)

//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
//    Q_PROPERTY(QString emailAddress READ emailAddress WRITE setEmailAddress
//               NOTIFY valueChanged)
//    Q_PROPERTY(QString attendeeId READ attendeeId WRITE setAttendeeId
//               NOTIFY valueChanged)
//    Q_PROPERTY(ParticipationStatus participationStatus READ participationStatus
//               WRITE setParticipationStatus NOTIFY valueChanged)
//    Q_PROPERTY(ParticipationRole participationRole READ participationRole
//               WRITE setParticipationRole NOTIFY valueChanged)

//public:
//    enum EventAttendeeField {
//        FieldName = QOrganizerEventAttendee::FieldName,
//        FieldEmailAddress = QOrganizerEventAttendee::FieldEmailAddress,
//        FieldAddendeeId = QOrganizerEventAttendee::FieldAttendeeId,
//        FieldParticipationStatus = QOrganizerEventAttendee::FieldParticipationStatus,
//        FieldParticipationRole = QOrganizerEventAttendee::FieldParticipationRole
//    };

//    enum ParticipationStatus {
//        StatusUnknown = QOrganizerEventAttendee::StatusUnknown,
//        StatusAccepted = QOrganizerEventAttendee::StatusAccepted,
//        StatusDeclined = QOrganizerEventAttendee::StatusDeclined,
//        StatusTentative = QOrganizerEventAttendee::StatusTentative,
//        StatusDelegated = QOrganizerEventAttendee::StatusDelegated,
//        StatusInProcess = QOrganizerEventAttendee::StatusInProcess,
//        StatusCompleted = QOrganizerEventAttendee::StatusCompleted
//    };

//    enum ParticipationRole {
//        RoleUnknown = QOrganizerEventAttendee::RoleUnknown,
//        RoleOrganizer = QOrganizerEventAttendee::RoleOrganizer,
//        RoleChairperson = QOrganizerEventAttendee::RoleChairperson,
//        RoleHost = QOrganizerEventAttendee::RoleHost,
//        RoleRequiredParticipant = QOrganizerEventAttendee::RoleRequiredParticipant,
//        RoleOptionalParticipant = QOrganizerEventAttendee::RoleOptionalParticipant,
//        RoleNonParticipant = QOrganizerEventAttendee::RoleNonParticipant
//    };

//    MyEventAttendee(QObject *parent = 0);

//    virtual DetailType type() const;

//    void setName(const QString &newName);
//    QString name() const;

//    void setEmailAddress(const QString &newEmailAddress);
//    QString emailAddress() const;

//    void setParticipationStatus(ParticipationStatus status);
//    ParticipationStatus participationStatus() const;

//    void setParticipationRole(ParticipationRole role);
//    ParticipationRole participationRole() const;

//    void setAttendeeId(const QString &newAttendeeId);
//    QString attendeeId() const;

//Q_SIGNALS:
//    void valueChanged();
//};


//class MyEventRsvp : public MyEventDetail
//{
//    Q_OBJECT
//    Q_PROPERTY(MyEventAttendee::ParticipationStatus participationStatus
//               READ participationStatus WRITE setParticipationStatus NOTIFY valueChanged)
//    Q_PROPERTY(MyEventAttendee::ParticipationRole participationRole
//               READ participationRole WRITE setParticipationRole NOTIFY valueChanged)
//    Q_PROPERTY(ResponseRequirement responseRequirement READ responseRequirement
//               WRITE setResponseRequirement NOTIFY valueChanged)
//    Q_PROPERTY(QDateTime responseDeadline READ responseDeadline
//               WRITE setResponseDeadline NOTIFY valueChanged)
//    Q_PROPERTY(QDateTime responseDate READ responseDate WRITE setResponseDate
//               NOTIFY valueChanged)
//    Q_PROPERTY(QString organizerName READ organizerName WRITE setOrganizerName
//               NOTIFY valueChanged)
//    Q_PROPERTY(QString organizerEmail READ organizerEmail
//               WRITE setOrganizerEmail NOTIFY valueChanged)
//    Q_ENUMS(EventRsvpField)
//    Q_ENUMS(ResponseRequirement)

//public:
//    enum EventRsvpField {
//        FieldParticipationStatus = QOrganizerEventRsvp::FieldParticipationStatus,
//        FieldParticipationRole = QOrganizerEventRsvp::FieldParticipationRole,
//        FieldResponseRequirement = QOrganizerEventRsvp::FieldResponseRequirement,
//        FieldResponseDeadline = QOrganizerEventRsvp::FieldResponseDeadline,
//        FieldResponseDate = QOrganizerEventRsvp::FieldResponseDate,
//        FieldOrganizerName = QOrganizerEventRsvp::FieldOrganizerName,
//        FieldOrganizerEmail = QOrganizerEventRsvp::FieldOrganizerEmail
//    };

//    enum ResponseRequirement {
//        ResponseNotRequired = QOrganizerEventRsvp::ResponseNotRequired,
//        ResponseRequired = QOrganizerEventRsvp::ResponseRequired
//    };

//    MyEventRsvp(QObject *parent = 0);

//    virtual DetailType type() const;
//    virtual QVariant value(int field) const;
//    virtual bool setValue(int key, const QVariant& value);

//    void setParticipationStatus(MyEventAttendee::ParticipationStatus status);
//    MyEventAttendee::ParticipationStatus participationStatus() const;

//    void setParticipationRole(MyEventAttendee::ParticipationRole role);
//    MyEventAttendee::ParticipationRole participationRole() const;

//    void setResponseRequirement(ResponseRequirement requirement);
//    ResponseRequirement responseRequirement() const;

//    void setResponseDeadline(const QDateTime &date);
//    QDateTime responseDeadline() const;

//    void setResponseDate(const QDateTime &date);
//    QDateTime responseDate() const;

//    void setOrganizerName(const QString &name);
//    QString organizerName() const;

//    void setOrganizerEmail(const QString &email);
//    QString organizerEmail() const;

//Q_SIGNALS:
//    void valueChanged();
//};

QML_DECLARE_TYPE(MyEventDetail)
//QML_DECLARE_TYPE(MyEventTime)
//QML_DECLARE_TYPE(MyEventComment)
//QML_DECLARE_TYPE(MyEventDescription)
//QML_DECLARE_TYPE(MyEventDisplayLabel)
//QML_DECLARE_TYPE(MyEventGuid)
//QML_DECLARE_TYPE(MyEventLocation)
QML_DECLARE_TYPE(MyEventParent)
//QML_DECLARE_TYPE(MyEventPriority)
//QML_DECLARE_TYPE(MyEventRecurrence)
////QML_DECLARE_TYPE(MyEventTag)
//QML_DECLARE_TYPE(MyEventTimestamp)
QML_DECLARE_TYPE(MyEventType)
////QML_DECLARE_TYPE(MyJournalTime)
////QML_DECLARE_TYPE(MyTodoProgress)
////QML_DECLARE_TYPE(MyTodoTime)
//QML_DECLARE_TYPE(MyEventReminder)
//QML_DECLARE_TYPE(MyEventAudibleReminder)
//QML_DECLARE_TYPE(MyEventEmailReminder)
//QML_DECLARE_TYPE(MyEventVisualReminder)
////QML_DECLARE_TYPE(MyEventExtendedDetail)
//QML_DECLARE_TYPE(MyEventAttendee)
//QML_DECLARE_TYPE(MyEventRsvp)
////QML_DECLARE_TYPE(MyEventClassification)
////QML_DECLARE_TYPE(MyEventVersion)

QT_END_NAMESPACE

//class MyEventParent

#endif // MYEVENTDETAIL_H
