#ifndef CREATETESTCASEDIALOG_H
#define CREATETESTCASEDIALOG_H

#include "UI/TestCase/Create/Controller/create-testcase-template-adapter.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class CreateTestCaseDialog;
}

class CreateTestCaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTestCaseDialog(QString fileName, QStringList IDs, QWidget *parent = nullptr);
    ~CreateTestCaseDialog();

    //Dialog return pointer to object. If this object is not used future it must be deleted
    //by owner of the dialog.
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
