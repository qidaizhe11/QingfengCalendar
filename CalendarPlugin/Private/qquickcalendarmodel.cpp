
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

void QQuickCalendarModel::populateFromVisibleDate(const QDate &previousDate,
                                                  bool force)
{
  if (!force && mVisibleDate.year() == previousDate.year() &&
      mVisibleDate.month() == previousDate.month()) {
    return;
  }

  bool isEmpty = mVisibleDates.isEmpty();
  if (isEmpty) {
    beginResetModel();
    mVisibleDates.fill(QDate(), daysOnACalendarMonth);
  }

  // The actual first day of month.
  QDate firstDayOfMonthDate(mVisibleDate.year(), mVisibleDate.month(), 1);
  int difference = ((firstDayOfMonthDate.dayOfWeek() -
                     mLocale.firstDayOfWeek()) + 7) % 7;
  // The first day to display should never be the 1st of the month, as we want
  // some days from the previous month to be visible.
  if (difference == 0) {
    difference += daysInAWeek;
  }
  QDate firstDayToDisplay = firstDayOfMonthDate.addDays(-difference);
  for (int i = 0; i < daysOnACalendarMonth; ++i) {
    mVisibleDates[i] = firstDayToDisplay.addDays(i);
  }

  mFirstVisibleDate = mVisibleDates.at(0);
  mLastVisibleDate = mVisibleDates.at(mVisibleDates.size() - 1);

  if (!isEmpty) {
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
  } else {
    endResetModel();
    emit countChanged(rowCount());
  }
}
