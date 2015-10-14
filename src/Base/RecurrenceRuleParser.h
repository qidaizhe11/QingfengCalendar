#ifndef RECURRENCERULEPARSER_H
#define RECURRENCERULEPARSER_H

//#include "base_global.h"

#include <QObject>
#include <QOrganizerRecurrenceRule>

QTORGANIZER_USE_NAMESPACE

class RecurrenceRuleParser : public QObject
{
    Q_OBJECT
public:
    explicit RecurrenceRuleParser(QObject *parent = 0);
    ~RecurrenceRuleParser();

    static bool parseRecurrenceRuleStr(const QString& str,
                                QOrganizerRecurrenceRule* rule);
    static QString encodeRecurrenceRule(const QOrganizerRecurrenceRule& rule);

private:
    static void parseRecurrenceFragment(const QString& key, const QString& value,
                                 QOrganizerRecurrenceRule* rule);
    static QSet<int> parseInts(const QString& str, int min, int max);
    static int parseDayOfWeek(const QString& str);
    static QDateTime parseDateTime(QString str);

    static QString encodeInts(const QSet<int>& ints);
    static QString encodeWeekString(Qt::DayOfWeek day);
};

#endif // RECURRENCERULEPARSER_H
