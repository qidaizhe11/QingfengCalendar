#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include <QDateTime>
#include <QtOrganizer/qorganizeritem.h>
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

public:
  explicit MyEvent(QObject *parent = 0);
  explicit MyEvent(const QOrganizerItem& item);
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

//  void setItem(const QOrganizerItem& item);

  Q_SIGNALS:
  void valueChanged();

private:
  QString m_item_id;
  QString m_description;
  QString m_display_label;

};

Q_DECLARE_METATYPE(MyEvent*)

#endif // MYEVENT_H
