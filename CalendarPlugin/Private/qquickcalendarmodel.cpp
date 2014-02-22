
#include "qquickcalendarmodel_p.h"

namespace {
static const int daysInAWeek = 7;

static const int weeksOnACalendarMonth = 6;

static const int daysOnACalendarMonth = daysInAWeek * weeksOnACalendarMonth;
}

QQuickCalendarModel::QQuickCalendarModel(QObject *parent) :
  QAbstractListModel(parent)
{
}

QDate QQuickCalendarModel::visibleDate() const
{
  return mVisibleDate;
}

void QQuickCalendarModel::setVisibleDate(const QDate &visibleDate)
{
  if (visibleDate != mVisibleDate && visibleDate.isValid()) {
    const QDate previousDate = mVisibleDate;
    mVisibleDate = visibleDate;
    populateFromVisibleDate(previousDate);
    emit visibleDateChanged(visibleDate);
  }
}

QLocale QQuickCalendarModel::locale() const
{
  return mLocale;
}

void QQuickCalendarModel::setLocale(const QLocale &locale)
{
  if (locale != mLocale) {
    Qt::DayOfWeek previousFirstDayOfWeek = mLocale.firstDayOfWeek();
    mLocale = locale;
    emit localeChanged(mLocale);
    if (mLocale.firstDayOfWeek() != previousFirstDayOfWeek) {
      populateFromVisibleDate(mVisibleDate, true);
    }
  }
}

QVariant QQuickCalendarModel::data(const QModelIndex &index, int role) const
{
  if (role == DateRole) {
    return mVisibleDates.at(index.row());
  }
  return QVariant();
}

int QQuickCalendarModel::rowCount(const QModelIndex &parent) const
{
  return mVisibleDates.isEmpty() ? 0 : weeksOnACalendarMonth * daysInAWeek;
}

QHash<int, QByteArray> QQuickCalendarModel::roleNames() const
{
  QHash<int, QByteArray> names;
  names[DateRole] = QByteArrayLiteral("date");
  return names;
}

QDate QQuickCalendarModel::dateAt(int index) const
{
  return index >= 0 && index < mVisibleDates.size() ? mVisibleDates.at(index) :
                                                      QDate();
}

//
// Returns the index for @date, or -1 if @date is outside of our range.
//
int QQuickCalendarModel::indexAt(const QDate &date)
{
  if (mVisibleDates.size() == 0 || date < mFirstVisibleDate ||
      date > mLastVisibleDate) {
    return -1;
  }

  return qMax(qint64(0), mFirstVisibleDate.daysTo(date));
}

int QQuickCalendarModel::weekNumberAt(int row) const
{
  const int index = row * daysInAWeek;
  const QDate date = dateAt(index);
  if (date.isValid()) {
    return date.weekNumber();
  }
  return -1;
}

