#ifndef QQUICKRANGEDDATE_P_H
#define QQUICKRANGEDDATE_P_H

#include <QDate>
#include <QtQml/qqml.h>

class QQuickRangedDate : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged
             RESET resetDate)
  Q_PROPERTY(QDate minimumDate READ minimumDate WRITE setMinimumDate
             NOTIFY minimumDateChanged RESET resetMinimumDate)
  Q_PROPERTY(QDate maximumDate READ maximumDate WRITE setMaximumDate
             NOTIFY maximumDateChanged RESET resetMaximumDate)
public:
  QQuickRangedDate();
  ~QQuickRangedDate() {}

  QDate date() const { return mDate; }
  void setDate(const QDate& date);
  void resetDate() {}

  QDate minimumDate() const { return mMinimumDate; }
  void setMinimumDate(const QDate& minimumDate);
  void resetMinimumDate() {}

  QDate maximumDate() const { return mMaximumDate; }
  void setMaximumDate(const QDate& maximumDate);
  void resetMaximumDate() {}

Q_SIGNALS:
  void dateChanged();
  void minimumDateChanged();
  void maximumDateChanged();

private:
  QDate mDate;
  QDate mMinimumDate;
  QDate mMaximumDate;
};

QML_DECLARE_TYPE(QQuickRangedDate)

#endif // QQUICKRANGEDDATE_P_H
