#ifndef TESTRESULTFILE_H
#define TESTRESULTFILE_H

#include <QString>
#include <QColor>

struct TestCompareResult
{
    QString status;
    QString runMark;
    QString outMark;
    int compareResult;
};

struct TestResult
{
    ~TestResult() {
        if(benchmark != nullptr) { delete benchmark; }
        if(previous != nullptr) { delete previous; }
    }

    QString outMark;

    TestCompareResult *benchmark = nullptr;
    TestCompareResult *previous = nullptr;

    QString exitStatus;
    int exitCode;

    qint64 execTimeMs;

    QString status;
    QColor color;
};

#endif // TESTRESULTFILE_H
