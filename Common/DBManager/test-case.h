#ifndef TESTCASEFILE_H
#define TESTCASEFILE_H

//#include "run-description.h"

#include <QString>
#include <QMap>
#include <QList>
#include <QDomDocument>

struct TestCaseConfig
{
    int waitingTime;
    int maxThreads;
    int compressionLevel;
    bool isSaveOutput;
    QString description;
    QString extraParams;
};

struct TestCase
{
    QString ID;
    QString fullFileName;

    QString currConfigName;
    QMap<QString, TestCaseConfig> configList;
};

#endif // TESTCASEFILE_H
