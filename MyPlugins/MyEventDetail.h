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
//    Classification = QOrganizerItemDetail::TypeClassification,
//    Comment = QOrganizerItemDetail::TypeComment,
    Description = QOrganizerItemDetail::TypeDescription,
    DisplayLabel = QOrganizerItemDetail::TypeDisplayLabel,
    ItemType = QOrganizerItemDetail::TypeItemType,
    Guid = QOrganizerItemDetail::TypeGuid,
    Location = QOrganizerItemDetail::TypeLocation,
    Parent = QOrganizerItemDetail::TypeParent,
//    Priority = QOrganizerItemDetail::TypePriority,
    Recurrence = QOrganizerItemDetail::TypeRecurrence,
//    Tag = QOrganizerItemDetail::TypeTag,
    Timestamp = QOrganizerItemDetail::TypeTimestamp,
//    Version = QOrganizerItemDetail::TypeVersion,
    Reminder = QOrganizerItemDetail::TypeReminder,
    AudibleReminder = QOrganizerItemDetail::TypeAudibleReminder,
    EmailReminder = QOrganizerItemDetail::TypeEmailReminder,
    VisualReminder = QOrganizerItemDetail::TypeVisualReminder,
//    ExtendedDetail = QOrganizerItemDetail::TypeExtendedDetail,
    EventAttendee = QOrganizerItemDetail::TypeEventAttendee,
//    EventRsvp = QOrganizerItemDetail::TypeEventRsvp,
    EventTime = QOrganizerItemDetail::TypeEventTime
//    JournalTime = QOrganizerItemDetail::TypeJournalTime,
//    TodoTime = QOrganizerItemDetail::TypeTodoTime,
//    TodoProgress = QOrganizerItemDetail::TypeTodoProgress
  };

  explicit MyEventDetail(QObject *parent = 0);
  ~MyEventDetail() {}

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

class MyEventType : public MyEventDetail
{
    Q_OBJECT

    Q_ENUMS(ItemTypeField)
    Q_ENUMS(ItemType)

    Q_PROPERTY(ItemType itemType READ itemType WRITE setItemType NOTIFY valueChanged)

public:
    enum ItemTypeField {
        FieldType = QOrganizerItemType::FieldType
    };

    enum ItemType {
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

    void setItemType(ItemType newType);
    ItemType itemType() const;

Q_SIGNALS:
    void valueChanged();
}

QT_END_NAMESPACE

//class MyEventParent

#endif // MYEVENTDETAIL_H
