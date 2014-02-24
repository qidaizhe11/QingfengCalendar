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

  Q_INVOKABLE int calculateIndex(const QDate& date);
};

#endif // EVENTUTILS_H

