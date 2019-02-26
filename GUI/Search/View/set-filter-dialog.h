#ifndef SETFILTERDIALOG_H
#define SETFILTERDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QMap>

#include <GUI/MainWindow/Model/mainwindow-tableitem.h>
#include <GUI/Search/Controller/set-filter-testcase-list-adapter.h>
#include <GUI/Search/Controller/set-filter-tag-tree-adapter.h>
#include <GUI/Search/Model/set-filter-dialog-model.h>
#include <GUI/Search/Controller/set-filter-status-table-adapter.h>

namespace Ui {
class SetFilterDialog;
}

class SetFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetFilterDialog(QWidget *parent = nullptr);
    ~SetFilterDialog();

    void appendUpdateTestCase(TestCase *tc);

    bool isCondition(MainWindowTableItem *item);

private slots:
    void on_markCheckBox_clicked(bool checked);
    void on_nameCheckBox_clicked(bool checked);
    void on_exitCodeCheckBox_clicked(bool checked);
    void on_diffTheBestCheckBox_clicked(bool checked);
    void on_diffPreviousCheckBox_clicked(bool checked);
    void on_tagCheckBox_clicked(bool checked);
    void on_statusCheckBox_clicked(bool checked);
    void on_changedCheckBox_clicked(bool checked);
    void on_tagTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);
    void on_SetFilterDialog_accepted();
    void on_testCaseListSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);
    void on_statusTableHeaderClicked(int arg1);

private:
    Ui::SetFilterDialog *ui;

    SetFilterDialogModel model;
    SetFilterTagTreeAdapter tagCollectionTreeAdapter;
    QMap<QString, QWidget *> tagDisplayWidgetBuffer;

    SetFilterTestCaseListAdapter testCaseListAdapter;
    QStringList headerCollection;
    SetFilterStatusTableAdapter statusTableAdapter;

    QStringList *checkedTagLinks = nullptr;
    void checkEqualCondition(int equalIndex, int equalResult, int equalValue, bool &isCondition);
    void appendCondition(QString orAnd, bool condition, bool &sumCondition);
};

#endif // SETFILTERDIALOG_H
