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
#include <QLabel>
#include <QComboBox>

#include "GUI/MainWindow/Controller/test-tree-adapter.h"
#include "GUI/MainWindow/Controller/test-table-adapter.h"

#include "GUI/Run/PlanDispatcher-New/View/plan-new-dialog.h"

#include <GUI/Search/View/set-filter-dialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PlanNewDialog *createPlanNewDialog(QWidget *parent = nullptr);

private slots:
    void on_testTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);
    void on_testTreeView_customContextMenuRequested(const QPoint &pos);
    void on_testTableViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);
    void on_testTableHeaderClicked(int arg1);
    void on_leftCompareComboBox_activated(int index);
    void on_rightCompareComboBox_activated(int index);

    void on_leftPerfectBox_clicked(bool checked);
    void on_leftTheBestBox_clicked(bool checked);
    void on_rightTheBestBox_clicked(bool checked);
    void on_rightPerfectBox_clicked(bool checked);

    void on_testTableView_customContextMenuRequested(const QPoint &pos);
    void on_testTableHeaderView_customContextMenuRequested(const QPoint &pos);

    void on_tagBox_anchorClicked(const QUrl &arg1);
    void on_attachTagBtn_clicked();

    //Menu actions

    //File
        //Test Case
            void on_actionNewTestCase_triggered();
            void on_actionOpenTestCase_triggered();
            //Load results for
                void on_actionLoadAllResults_triggered();
                void on_actionLoadTestCaseResults_triggered();
                void on_actionLoadFolderResults_triggered();
                void on_actionLoadRunResults_triggered();
                void on_actionLoadTestResult_triggered();
                void on_actionLoadOneResult_triggered();
            //Run results
                void on_actionShow_RunHeader_triggered();

    void on_actionExit_triggered();


    //Search
    void on_actionSet_Filter_triggered();

    //Run
    void on_actionRun_dispatcher_triggered();

    //Tags
    void on_actionTagCollections_triggered();
    void on_actionAttachTag_triggered();

    //Tools

    //View


    //Help
    void on_actionContentsHelp_triggered();
    void on_actionIndexHelp_triggered();

private:
    Ui::MainWindow *ui;

    QModelIndex backupTreeIndex;
    void saveTreeState();
    void saveExpandedState(QModelIndex &index);
    void loadTreeState();
    void loadExpandedState(QModelIndex &index);

    QMap<QString, IResultCompareWidget *> compareWidgets;

    QMenu tableContextMenu;
    QMenu tableHeadContextMenu;
    QMenu treeContextMenu;

    enum SideOfCompareWidget {LeftSideOfCompareWidget, RightSideOfCompareWidget};

    void updateComparePanel(int index, QComboBox *compareComboBox, QPlainTextEdit *logBox, QLabel *infoLabel,
                            QCheckBox *perfectBox, QCheckBox *theBestBox, SideOfCompareWidget sideOfCompareWidget);
    void updateTestStatus(int runNum, QString newStatus, bool checked);
    void updateTagList(MainWindowTableItem *tableItem);

    void attachTag();

    QMap<int, MainWindowTableHeader *> *compareComboBox_dataSource = nullptr;

    SetFilterDialog *filterDlg = nullptr;
};

#endif // MAINWINDOW_H
