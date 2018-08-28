#ifndef TESTCASEFILE_H
#define TESTCASEFILE_H

#include "run-description.h"

#include <QString>
#include <QMap>
#include <QList>
#include <QtXml/QDomDocument>

// Test Case it is multi purpose structure for transfer data

struct TestCaseConfig
{
    //For read\write to disk
    int WaitingTime;
    int MaxThreads;
    int CompressionLevel;
    bool isSaveOutput;
    QString description;
    QDomDocument ExtraParams;

    //For displaying
    bool Checked;
};

struct TestCase
{
    //For read\write to disk
    QString ID;
    QString CurrentConfigName;
    QMap<QString, TestCaseConfig> ConfigList;

    //For displaying
    bool Checked;
    QString Name;
    QString FullFileName;
    QMap<int, RunDescription *> RunDescriptions;
};

#endif // TESTCASEFILE_H
