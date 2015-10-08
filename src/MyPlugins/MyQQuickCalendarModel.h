#ifndef MYQQUICKCALENDARMODEL_H
#define MYQQUICKCALENDARMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QLocale>
#include <QVariant>
#include <QDate>

QT_BEGIN_NAMESPACE

class MyQQuickCalendarModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QDate visibleDate READ visibleDate WRITE setVisibleDate NOTIFY visibleDateChanged)
    Q_PROPERTY(QLocale locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QDate firstVisibleDate READ firstVisibleDate NOTIFY visibleDateChanged)
    Q_PROPERTY(QDate lastVisibleDate READ lastVisibleDate NOTIFY visibleDateChanged)

public:
    explicit MyQQuickCalendarModel(QObject *parent = 0);

    enum {
        // If this class is made public, this will have to be changed.
        DateRole = Qt::UserRole + 1
    };

    QDate visibleDate() const;
    void setVisibleDate(const QDate &visibleDate);

    QLocale locale() const;
    void setLocale(const QLocale &locale);

    QDate firstVisibleDate() const { return mFirstVisibleDate; }
    QDate lastVisibleDate() const { return mLastVisibleDate; }

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE QDate dateAt(int index) const;
    Q_INVOKABLE int indexAt(const QDate &visibleDate);
    Q_INVOKABLE int weekNumberAt(int row) const;

Q_SIGNALS:
    void visibleDateChanged(const QDate &visibleDate);
    void localeChanged(const QLocale &locale);
    void countChanged(int count);

protected:
    void populateFromVisibleDate(const QDate &previousDate, bool force = false);

    QDate mVisibleDate;
    QDate mFirstVisibleDate;
    QDate mLastVisibleDate;
    QVector<QDate> mVisibleDates;
    QLocale mLocale;
};

QT_END_NAMESPACE

#endif // MYQQUICKCALENDARMODEL_H
