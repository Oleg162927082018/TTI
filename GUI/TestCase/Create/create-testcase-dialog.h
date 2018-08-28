#ifndef CREATETESTCASEDIALOG_H
#define CREATETESTCASEDIALOG_H

#include "create-testcase-model.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class CreateTestCaseDialog;
}

class CreateTestCaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTestCaseDialog(QString fileName, QStringList IDs, QWidget *parent = 0);
    ~CreateTestCaseDialog();

    TestCase *GetTestCase();
private slots:
    void on_CancelButton_clicked();

    void on_OkButton_clicked();

    void on_templateSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

private:
    Ui::CreateTestCaseDialog *ui;

    QString testCaseFullFileName;
    QStringList testCaseIDs;
    TestCaseTemplateListAdapter templateListAdapter;
};

#endif // CREATETESTCASEDIALOG_H
