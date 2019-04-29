#ifndef MAINWINDOWTABLEITEM_H
#define MAINWINDOWTABLEITEM_H

#include <QString>
#include <QMap>

#include <Common/DBManager/test-case.h>
#include <Common/DBManager/test-result.h>
#include <Common/DBManager/test-status.h>

#include "mainwindow-tableheader.h"

struct MainWindowTableItem
{
    bool checked = true;
    QString name;

    TestStatus *status;
    QMap<int, TestResult *> results;

    TestCase *ownerTestCase = nullptr;
    QMap<int, MainWindowTableHeader *> *fullTableHeaders = nullptr;
    QMap<int, MainWindowTableHeader *> *visibleTableHeaders = nullptr;
};


#endif // MAINWINDOWTABLEITEM_H
