#ifndef PLANNEWDIALOG_H
#define PLANNEWDIALOG_H

#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

#include <QDialog>

namespace Ui {
class PlanNewDialog;
}

class PlanNewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanNewDialog(QList<TestCaseFolder *> *testCases, QWidget *parent = 0);
    ~PlanNewDialog();

private slots:
    void on_planBtn_clicked();

    void on_PlanNewDialog_accepted();

private:
    Ui::PlanNewDialog *ui;

    QList<TestCaseFolder *> *testCaseList = 0;

    void closeEvent(QCloseEvent *e);
};

#endif // PLANNEWDIALOG_H
