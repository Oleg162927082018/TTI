#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include "Common/DBManager/test-case.h"
#include "Common/DBManager/run-description.h"
#include "GUI/MainWindow/Model/mainwindow-treefolder.h"

#include <QList>


class MainWindowModel
{
public:
    MainWindowModel();

    static QList<MainWindowTreeFolder *> tree;

    // OpenTestCase load test-case data and run data
    // but not load results of testing.
    //
    // Return:
    // pointer of test-case folder or nullptr
    // errorCode = 0 - Success
    // errorCode = 1 - Test-case file not found
    // errorCode = 2 - Test-case already open
    static MainWindowTreeFolder *LoadTestCase(TestCase *tc);

    //For load new run results need to know number of last run
    static void UpdateRunDescriptions(MainWindowTreeFolder *testCaseFolder);

    // Load results of folder
    static void LoadFolderResults(MainWindowTreeFolder *tcFolder);
    static void LoadRunResults(MainWindowTreeFolder *folder, MainWindowTableHeader *run);
    static void LoadTestResults(MainWindowTreeFolder *folder, MainWindowTableItem *test);
    static void LoadOneResult(MainWindowTreeFolder *folder, MainWindowTableItem *test, MainWindowTableHeader *run);


    static void AddRunHeader(MainWindowTreeFolder *treeFolder, RunDescription *runDesc, bool isVisible);
    static void AddTableItem(MainWindowTreeFolder *treeFolder, MainWindowTableItem *tableItem);
    static void DeleteTree();

private:
    static void setTestCaseOwner(MainWindowTreeFolder *folder, TestCase *tc);
    static void LoadTreeFolder(MainWindowTreeFolder *treeFolder, QString relativeTreeFolderName);
    static void DeleteTreeFolder(MainWindowTreeFolder *treeFolder);
    static QString getTableHeader(RunDescription *rd);
};

#endif // MAINWINDOWMODEL_H
