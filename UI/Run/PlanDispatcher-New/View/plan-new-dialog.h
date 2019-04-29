#ifndef PLANNEWDIALOG_H
#define PLANNEWDIALOG_H

#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

#include <QDialog>
#include <QMap>

#include <UI/Run/PlanDispatcher-New/Controller/plan-new-testcase-list-adapter.h>

namespace Ui {
class PlanNewDialog;
}

class PlanNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanNewDialog(QWidget *parent = nullptr);
    ~PlanNewDialog();

    void addTest(TestCase *testCase, TestStatus * testStatus);

private slots:
    void on_fileNameBtn_clicked();

    void on_isAddImediatelyBox_stateChanged(int arg1);

    void on_okCancelBtns_accepted();

private:
    Ui::PlanNewDialog *ui;

    QStringList testCaseNames;
    QList<TestCase *> originalTestCases;
    QList<TestCase *> overrideTestCases;
    QList<QPair<TestCase *, TestStatus *>> tests;

    PlanNewTestCaseListAdapter testCaseListAdapter;
};

#endif // PLANNEWDIALOG_H
