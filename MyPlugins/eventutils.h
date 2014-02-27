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

//  Q_INVOKABLE int gridIndex(const QDate& date, const QDate& visible_date);
  Q_INVOKABLE int lastDays(const QDate& start_date, const QDate& end_date);
};

#endif // EVENTUTILS_H

