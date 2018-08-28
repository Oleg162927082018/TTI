#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Common/itestcasetemplate.h"
#include "Common/DBManager/test-case.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QMenu>
#include <QList>
#include <QCheckBox>
#include <QPlainTextEdit>

#include "GUI/MainWindow/Controller/test-tree-adapter.h"
#include "GUI/MainWindow/Controller/test-table-adapter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionNewTestCase_triggered();

    void on_actionOpenTestCase_triggered();

    void on_actionRun_dispatcher_triggered();

    void on_testTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    void on_testTableViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    void on_leftCompareComboBox_activated(int index);

    void on_rightCompareComboBox_activated(int index);

    void on_leftTheBestBox_stateChanged(int arg1);

    void on_rightTheBestBox_stateChanged(int arg1);

    void on_leftPerfectBox_stateChanged(int arg1);

    void on_rightPerfectBox_stateChanged(int arg1);

    void on_actionLoadTestCaseResults_triggered();

    void on_actionLoadFolderResults_triggered();

    void on_actionLoadAllResults_triggered();

    void on_actionLoadTestResult_triggered();

    void on_actionLoadRunResults_triggered();

    void on_actionLoadOneResult_triggered();

    void on_actionShowRun_triggered();

    void on_testTableView_customContextMenuRequested(const QPoint &pos);

    void on_testTableHeaderView_customContextMenuRequested(const QPoint &pos);

    void on_testTreeView_customContextMenuRequested(const QPoint &pos);

    void on_tagBox_anchorClicked(const QUrl &arg1);

    void on_menuTagCollections_triggered();

    void on_menyAttachTag_triggered();

    void on_attachTagBtn_clicked();

    void on_menuContentsHelp_triggered();

    void on_menuIndexHelp_triggered();

private:
    Ui::MainWindow *ui;

    bool isMainWindowUpdating = false;
    void BeginMainWindowUpdate();
    void EndMainWindowUpdate();

    QModelIndex bakupTreeIndex;
    void saveTreeState();
    void saveExpandedState(QModelIndex &index);
    void loadTreeState();
    void loadExpandedState(QModelIndex &index);

    void FreeTreeModel(TestCaseFolder *folder);

    TestTreeAdapter testTreeAdapter;
    TestTableAdapter testTableAdapter;

    QMap<QString, IResultCompareWidget *> compareWidgets;

    void UpdateTestStatus(int runNum, QString newStatus, bool checked);

    QMenu tableContextMenu;
    QMenu tableHeadContextMenu;
    QMenu treeContextMenu;

    void UpdateTagListForCurrentTest();

    void AttachTag();
    void FillLeftDetailPanel(TestCaseItem *testItem);
    void FillRightDetailPanel(TestCaseItem *testItem);
    TestCaseItem *GetCurrentTestItem();
};

#endif // MAINWINDOW_H
