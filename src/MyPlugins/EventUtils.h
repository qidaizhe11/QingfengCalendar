#ifndef EVENTUTILS_H
#define EVENTUTILS_H

#include <QQuickItem>

class EventUtils : public QQuickItem
{
  Q_OBJECT
  Q_DISABLE_COPY(EventUtils)

public:
  EventUtils(QQuickItem *parent = 0);
  ~EventUtils();

  // QDateTime function, daysTo.
  // (Javascript Date has lack of this useful function,so put it here.)
  Q_INVOKABLE int daysTo(const QDateTime& start, const QDateTime& end);

  // return the lasted days from @start_date to @end_date.
  // both the start_date and the end_date, they are all counted.
  Q_INVOKABLE int lastDays(const QDateTime& start_date, const QDateTime& end_date);
};

#endif // EVENTUTILS_H

