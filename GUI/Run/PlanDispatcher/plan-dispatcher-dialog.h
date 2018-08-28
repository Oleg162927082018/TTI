#ifndef PLANDISPATCHERDIALOG_H
#define PLANDISPATCHERDIALOG_H

#include "Common/DBManager/test-case.h"
#include "Common/DBManager/test-case-cache.h"
#include "Common/RunManager/runmanager.h"

#include "task-table-adapter.h"
#include "log-adapter.h"

#include <QDialog>
#include <QItemSelection>

namespace Ui {
class PlanDispatcherDialog;
}

class PlanDispatcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanDispatcherDialog(QList<TestCaseFolder *> *testCasesCache, QWidget *parent = 0);
    ~PlanDispatcherDialog();

private slots:
    void on_newBtn_clicked();

    void on_addBtn_clicked();

    void on_editBtn_clicked();

    void on_removeBtn_clicked();

    void on_clearBtn_clicked();


    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_pauseBtn_clicked();


    void on_moveUpBtn_clicked();

    void on_moveDownBtn_clicked();


    void on_closeBtn_clicked();


    void on_hideBtn_clicked();

    void on_showLog14Btn_clicked();

    void on_showLog24Btn_clicked();

    void on_showLog34Btn_clicked();

    void on_showLog44Btn_clicked();


    void onQueueTableSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    void onLogChanged(int planInd);

    void onTestStarted(int planInd);
    void onTestFinished(int planInd);

    void onTestCaseStarted(int planInd);
    void onTestCaseFinished(int planInd);

    void onPlanStarted(int planInd);
    void onPlanFinished(int planInd);

    void onAllFinished();

private:
    Ui::PlanDispatcherDialog *ui;

    QList<TestCaseFolder *> *testCaseList = 0;

    TaskTableAdapter taskTableAdapter;
    LogAdapter logAdapter;

};

#endif // PLANDISPATCHERDIALOG_H
