#ifndef TESTSTATUSFILE_H
#define TESTSTATUSFILE_H

#include <QtXml/QDomDocument>
#include <QStringList>
#include <QMap>
#include <QColor>

struct BenchmarkInfo
{
    QString status;
    QString outMark;

    QString label;
    QString comment;
    QColor color;
};

struct TestStatus
{
    QString relativeFileName; //Name from folder STATUS

    QString data; //Params used for run test. Send to dll for getting run command.
    QMap<int, BenchmarkInfo> benchmarks;
    QStringList tags;
};

#endif // TESTSTATUSFILE_H
