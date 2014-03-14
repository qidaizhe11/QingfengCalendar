#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include <QtQml/qqml.h>
#include <QDateTime>
#include <QString>
#include <QDebug>
//#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/QOrganizerEvent>
#include <QtOrganizer/qorganizerglobal.h>

//QT_USE_NAMESPACE

QTORGANIZER_USE_NAMESPACE

class MyEvent : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString itemId READ itemId NOTIFY valueChanged)
  Q_PROPERTY(QString collectionId READ collectionId WRITE setCollectionId
             NOTIFY valueChanged)
  Q_PROPERTY(QString displayLabel READ displayLabel WRITE setDisplayLabel
             NOTIFY valueChanged)
  Q_PROPERTY(bool allDay READ allDay WRITE setAllDay NOTIFY valueChanged)
  Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime
             NOTIFY startDateTimeChanged)
  Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime
             NOTIFY endDateTimeChanged)
  Q_PROPERTY(QString description READ description WRITE setDescription
             NOTIFY valueChanged)
  Q_PROPERTY(QString location READ location WRITE setLocation
             NOTIFY valueChanged)

public:
  MyEvent(QObject *parent = 0);
  MyEvent(const QOrganizerEvent& event, QObject *parent = 0);
//  explicit MyEvent(const QString& description, const QString& display_label);
  ~MyEvent() {}

  QString itemId() const;

  QString collectionId() const;
  void setCollectionId(const QString& collectionId);

  QString displayLabel() const { return m_display_label; }
  void setDisplayLabel(QString display_label) {
    m_display_label = display_label;
  }

  bool allDay() const { return m_all_day; }
  void setAllDay(bool all_day) { m_all_day = all_day; }

  QDateTime startDateTime() const { return m_start_date_time; }
  void setStartDateTime(const QDateTime& start_date_time) {
    m_start_date_time = start_date_time;
  }

  QDateTime endDateTime() const { return m_end_date_time; }
  void setEndDateTime(const QDateTime& end) { m_end_date_time = end; }

  QString description() const { return m_description; }
  void setDescription(const QString& description) {
    m_description = description;
  }

  QString location() const { return m_location; }
  void setLocation(const QString& location) { m_location = location;}

//  void setItem(const QOrganizerItem& item);

  QOrganizerEvent toQOrganizerEvent() const;

  Q_SIGNALS:
  void valueChanged();
  void startDateTimeChanged();
  void endDateTimeChanged();

private:

  QOrganizerItemId m_id;
  QOrganizerCollectionId m_collection_id;
  QString m_display_label;
  bool m_all_day;
  QDateTime m_start_date_time;
  QDateTime m_end_date_time;
  QString m_description;
  QString m_location;
};

Q_DECLARE_METATYPE(MyEvent*)

#endif // MYEVENT_H
