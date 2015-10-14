#include "RecurrenceRuleParser.h"
#include <QStringList>

RecurrenceRuleParser::RecurrenceRuleParser(QObject *parent) : QObject(parent)
{

}

RecurrenceRuleParser::~RecurrenceRuleParser()
{

}

bool RecurrenceRuleParser::parseRecurrenceRuleStr(const QString &str,
                                            QOrganizerRecurrenceRule *rule)
{
    QStringList parts = str.split(QLatin1Char(';'));
    if (parts.size() == 0) {
        return false;
    }

    QString freq_part = parts.takeFirst();
    QStringList freq_parts = freq_part.split(QLatin1Char('='));
    if (freq_parts.size() != 2) {
        return false;
    }
    if (freq_parts.at(0) != QStringLiteral("FREQ")) {
        return false;
    }
    QString freq_value = freq_parts.at(1);
    if (freq_value == QStringLiteral("DAILY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Daily);
    } else if (freq_value == QStringLiteral("WEEKLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Weekly);
    } else if (freq_value == QStringLiteral("MONTHLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Monthly);
    } else if (freq_value == QStringLiteral("YEARLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Yearly);
    } else {
        return false;
    }

    foreach (const QString& part, parts) {
        QStringList key_value = part.split(QLatin1Char('='));
        if (key_value.size() != 2) {
            return false;
        }
        parseRecurrenceFragment(key_value.at(0), key_value.at(1), rule);
    }
    return true;
}

void RecurrenceRuleParser::parseRecurrenceFragment(const QString &key, const QString &value,
                                             QOrganizerRecurrenceRule *rule)
{
    if (key == QStringLiteral("INTERVAL")) {
        bool ok;
        int n = value.toInt(&ok);
        if (ok && n >= 1) {
            rule->setInterval(n);
        }
    } else if (key == QStringLiteral("COUNT")) {
        bool ok;
        int count = value.toInt(&ok);
        if (ok && count >= 0) {
            rule->setLimit(count);
        }
    } else if (key == QStringLiteral("UNTIL")) {
        QDate date;
        if (value.contains(QLatin1Char('T'))) {
            QDateTime dt = parseDateTime(value);
            date = dt.date();
        } else {
            date = QDate::fromString(value, QStringLiteral("yyyyMMdd"));
        }
        if (date.isValid()) {
            rule->setLimit(date);
        }
    } else if (key == QStringLiteral("BYDAY")) {
        QSet<Qt::DayOfWeek> days;
        QStringList day_parts = value.split(QLatin1Char(','));
        foreach (QString day_str, day_parts) {
            if (day_str.length() < 2) {
                // bad day specifier
                continue;
            } else if (day_str.length() > 2) {
                // parse something like -2SU, meaning the second-last Sunday
                QString pos_str = day_str;
                day_str = day_str.right(2);
                pos_str.chop(2);
                bool ok;
                int pos = pos_str.toInt(&ok);
                if (!ok) {
                    continue;
                }
                rule->setPositions(QSet<int>() << pos);
            }
            int day = parseDayOfWeek(day_str);
            if (day != -1) {
                days << (Qt::DayOfWeek)day;
            }
        }
        if (!days.isEmpty()) {
            rule->setDaysOfWeek(days);
        }
    } else if (key == QStringLiteral("BYMONTHDAY")) {
        QSet<int> days = parseInts(value, -31, 31);
        if (!days.isEmpty()) {
            rule->setDaysOfMonth(days);
        }
    } else if (key == QStringLiteral("BYWEEKNO")) {
        QSet<int> weeks = parseInts(value, -53, 53);
        if (!weeks.isEmpty()) {
            rule->setWeeksOfYear(weeks);
        }
    } else if (key == QStringLiteral("BYMONTH")) {
        QSet<QOrganizerRecurrenceRule::Month> months;
        QStringList month_parts = value.split(QLatin1Char(','));
        foreach (const QString& month_part, month_parts) {
            bool ok;
            int month = month_part.toInt(&ok);
            if (ok && month >= 1 && month <= 12) {
                months << (QOrganizerRecurrenceRule::Month)month;
            }
        }
        if (!months.isEmpty()) {
            rule->setMonthsOfYear(months);
        }
    } else if (key == QStringLiteral("BYYEARDAY")) {
        QSet<int> days = parseInts(value, -366, 366);
        if (!days.isEmpty()) {
            rule->setDaysOfYear(days);
        }
    } else if (key == QStringLiteral("BYSETPOS")) {
        QSet<int> poss = parseInts(value, -366, 366);
        if (!poss.isEmpty()) {
            rule->setPositions(poss);
        }
    } else if (key == QStringLiteral("WKST")) {
        int day = parseDayOfWeek(value);
        if (day != -1) {
            rule->setFirstDayOfWeek((Qt::DayOfWeek)day);
        }
    }
}

QSet<int> RecurrenceRuleParser::parseInts(const QString &str, int min, int max)
{
    QSet<int> values;
    QStringList parts = str.split(QLatin1Char(','));
    foreach (const QString& part, parts) {
        bool ok;
        int value = part.toInt(&ok);
        if (ok && value >= min && value <= max && value != 0) {
            values << value;
        }
    }

    return values;
}

int RecurrenceRuleParser::parseDayOfWeek(const QString &str)
{
    if (str == QStringLiteral("MO")) {
        return Qt::Monday;
    } else if (str == QStringLiteral("TU")) {
        return Qt::Tuesday;
    } else if (str == QStringLiteral("WE")) {
        return Qt::Wednesday;
    } else if (str == QStringLiteral("TH")) {
        return Qt::Thursday;
    } else if (str == QStringLiteral("FR")) {
        return Qt::Friday;
    } else if ( str == QStringLiteral("SA")) {
        return Qt::Saturday;
    } else if (str == QStringLiteral("SU")) {
        return Qt::Sunday;
    } else {
        return -1;
    }
}

QDateTime RecurrenceRuleParser::parseDateTime(QString str)
{
    bool utc = str.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive);
    if (utc) {
        str.chop(1);
    }
    QDateTime dt(QDateTime::fromString(str, QStringLiteral("yyyyMMddTHHmmss")));
    if (utc) {
        dt.setTimeSpec(Qt::UTC);
    }
    return dt;
}

QString RecurrenceRuleParser::encodeRecurrenceRule(const QOrganizerRecurrenceRule &rule)
{
    QString value = QStringLiteral("FREQ=");
    switch (rule.frequency()) {
    case QOrganizerRecurrenceRule::Daily:
        value.append(QStringLiteral("DAILY"));
        break;
    case QOrganizerRecurrenceRule::Weekly:
        value.append(QStringLiteral("WEEKLY"));
        break;
    case QOrganizerRecurrenceRule::Monthly:
        value.append(QStringLiteral("MONTHLY"));
        break;
    case QOrganizerRecurrenceRule::Yearly:
        value.append(QStringLiteral("YEARLY"));
        break;
    case QOrganizerRecurrenceRule::Invalid:
    default:
        return QString();
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::CountLimit) {
        value.append(QStringLiteral(";COUNT="));
        value.append(QString::number(rule.limitCount()));
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::DateLimit) {
        value.append(QStringLiteral(";UNTIL="));
        value.append(rule.limitDate().toString(QStringLiteral("yyyyMMdd")));
    }
    if (rule.interval() > 1) {
        value.append(QStringLiteral(";INTERVAL="));
        value.append(QString::number(rule.interval()));
    }
    if (!rule.daysOfWeek().isEmpty()) {
        value.append(QStringLiteral(";BYDAY="));
        bool first = true;
        QList<Qt::DayOfWeek> days_of_week(QList<Qt::DayOfWeek>::fromSet(rule.daysOfWeek()));
        std::sort(days_of_week.begin(), days_of_week.end());
        foreach (Qt::DayOfWeek day, days_of_week) {
            if (!first) {
                value.append(QStringLiteral(","));
            }
            first = false;
            value.append(encodeWeekString(day));
        }
    }
    if (!rule.daysOfMonth().isEmpty()) {
        value.append(QStringLiteral(";BYMONTHDAY="));
        value.append(encodeInts(rule.daysOfMonth()));
    }
    if (!rule.daysOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYYEARDAY="));
        value.append(encodeInts(rule.daysOfYear()));
    }
    if (!rule.weeksOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYWEEKNO="));
        value.append(encodeInts(rule.weeksOfYear()));
    }
    if (!rule.monthsOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYMONTH="));
        bool first = true;
        QList<QOrganizerRecurrenceRule::Month> months(
                    QList<QOrganizerRecurrenceRule::Month>::fromSet(rule.monthsOfYear()));
        std::sort(months.begin(), months.end());
        foreach (QOrganizerRecurrenceRule::Month month, months) {
            if (!first) {
                value.append(QStringLiteral(","));
            }
            first = false;
            value.append(QString::number(month));
        }
    }
    if (!rule.positions().isEmpty()) {
        value.append(QStringLiteral(";BYSETPOS="));
        value.append(encodeInts(rule.positions()));
    }
    if (rule.firstDayOfWeek() != Qt::Monday && rule.firstDayOfWeek() > 0) {
        value.append(QStringLiteral(";WKST="));
        value.append(encodeWeekString(rule.firstDayOfWeek()));
    }

    return value;
}

QString RecurrenceRuleParser::encodeInts(const QSet<int> &ints)
{
    QString value = QString();
    bool first = true;
    QList<int> int_list(QList<int>::fromSet(ints));
    std::sort(int_list.begin(), int_list.end());
    foreach (int n, int_list) {
        if (!first) {
            value.append(QStringLiteral(","));
        }
        first = false;
        value.append(QString::number(n));
    }
    return value;
}

QString RecurrenceRuleParser::encodeWeekString(Qt::DayOfWeek day)
{
    switch (day) {
    case Qt::Monday:
        return QStringLiteral("MO");
    case Qt::Tuesday:
        return QStringLiteral("TU");
    case Qt::Wednesday:
        return QStringLiteral("WE");
    case Qt::Thursday:
        return QStringLiteral("TH");
    case Qt::Friday:
        return QStringLiteral("FR");
    case Qt::Saturday:
        return QStringLiteral("SA");
    case Qt::Sunday:
        return QStringLiteral("SU");
    default:
        return QString();
    }
}

