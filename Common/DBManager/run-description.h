#ifndef RUNDESCRIPTIONFILE_H
#define RUNDESCRIPTIONFILE_H

#include <QDateTime>
#include <QString>
#include <QMap>

struct RunDescription
{
    int Num;
    QDateTime LocalDateTimeOfStart;
    QString TestPlanFullFileName;
    QString Comment;
    int WorkingTimeMs;
    int MinUsedThreads;
    int MaxUsedThreads;
    QMap<QString, int> Statistics;
};

#endif // RUNDESCRIPTIONFILE_H
