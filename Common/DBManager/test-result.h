#ifndef TESTRESULTFILE_H
#define TESTRESULTFILE_H

#include <QString>
#include <QColor>

struct TestResult
{
    QString outMark;

    QString benchmarkStatus;
    QString benchmarkRunMark;
    QString benchmarkOutMark;
    int benchmarkCompareResult;

    QString previousRunMark;
    QString previousOutMark;
    int previousCompareResult;

    QString exitStatus;
    int exitCode;

    qint64 execTimeMs;

    QString status;
    QColor color;
};

#endif // TESTRESULTFILE_H
