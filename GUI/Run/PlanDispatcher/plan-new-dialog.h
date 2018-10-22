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
    void on_fileNameBtn_clicked();

    void on_isAddImediatelyBox_stateChanged(int arg1);

    void on_okCancelBtns_accepted();

private:
    Ui::PlanNewDialog *ui;

    QList<TestCaseFolder *> *testCaseList = 0;
};

#endif // PLANNEWDIALOG_H
