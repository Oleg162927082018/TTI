#ifndef TESTSTATUSFILE_H
#define TESTSTATUSFILE_H

#include <QtXml/QDomDocument>
#include <QStringList>

struct BenchmarkInfo
{
    QString status;
    QString outMark;
};

struct TestStatus
{
    QDomDocument data;
    QMap<int, BenchmarkInfo> benchmarks;
    QStringList tags;
};

#endif // TESTSTATUSFILE_H
