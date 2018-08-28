#ifndef OPENTESTCASEDIALOG_H
#define OPENTESTCASEDIALOG_H

#include "Common/DBManager/dbmanager.h"

#include "GUI/TestCase/Open/Controller/open-test-case-list-controller.h"
#include "GUI/Run/Common/RunDescription/View/run-description-table-widget.h"

#include <QDialog>
#include <QString>
#include <QItemSelection>

/*
 * Класс OpenTestCaseDialog создает массив объектов TestCase в которых
 * загружает RunDescription. Затем пользователь помечает какие запуски
 * он хотел бы загрузить.
 *
 * Метод GetTestCases очищает списки RunDescription'ов от тех которые
 * пользователь не захотел грузить и возвращает список указателей на
 * TestCase'ы.
 *
 * Главное окно или любое другое обязано удалить созданные OpenTestCaseDialog
 * объекты TestCase после использования.
*/


namespace Ui {
class OpenTestCaseDialog;
}

class OpenTestCaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenTestCaseDialog(QWidget *parent = 0);
    ~OpenTestCaseDialog();

    QList<TestCase *> *GetTestCaseList();

private slots:
    void on_addBtn_clicked();

    void on_delBtn_clicked();

    void on_testCaseSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    void on_currentConfigBox_currentIndexChanged(const QString &arg1);

    void on_isLoadResultBox_stateChanged(int arg1);

private:
    Ui::OpenTestCaseDialog *ui;

    void BeginDialogUpdate();
    void EndDialogUpdate();
    bool isDialogUpdating;

    TestCaseListController testCaseListController;
    RunDescriptionTableWidget *runDescriptionTableWidget;

    OpenTestCaseModel dataModel;
};

#endif // OPENTESTCASEDIALOG_H
