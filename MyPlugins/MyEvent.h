#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include <QDateTime>
//#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/QOrganizerEvent>
#include <QtOrganizer/qorganizerglobal.h>

QTORGANIZER_USE_NAMESPACE

class MyEvent : public QObject
{
  Q_OBJECT

//  Q_PROPERTY(QString itemId READ itemId WRITE setItemId NOTIFY valueChanged)
  Q_PROPERTY(QString description READ description WRITE setDescription
             NOTIFY valueChanged)
  Q_PROPERTY(QString displayLabel READ displayLabel WRITE setDisplayLabel
             NOTIFY valueChanged)
  Q_PROPERTY(bool allDay READ allDay WRITE setAllDay NOTIFY valueChanged)
  Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime
             NOTIFY valueChanged)
  Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime
             NOTIFY valueChanged)

public:
  explicit MyEvent(QObject *parent = 0);
  explicit MyEvent(const QOrganizerEvent& event);
  explicit MyEvent(const QString& description, const QString& display_label);

//  QString itemId() const { return m_item_id; }
//  void setItemId(const QString& id) { m_item_id = id; }

  QString description() const { return m_description; }
  void setDescription(const QString& description) {
    m_description = description;
  }

  QString displayLabel() const { return m_display_label; }
  void setDisplayLabel(const QString& display_label) {
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

//  void setItem(const QOrganizerItem& item);

  Q_SIGNALS:
  void valueChanged();

private:
  QString m_item_id;
  QString m_description;
  QString m_display_label;
  bool m_all_day;
  QDateTime m_start_date_time;
  QDateTime m_end_date_time;

};

Q_DECLARE_METATYPE(MyEvent*)

#endif // MYEVENT_H
