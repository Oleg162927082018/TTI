#ifndef OPENTESTCASEDIALOG_H
#define OPENTESTCASEDIALOG_H

#include "Common/DBManager/dbmanager.h"

#include "GUI/TestCase/Open/Controller/open-test-case-list-controller.h"
#include "GUI/TestCase/Open/Controller/open-testcase-rundescriptions-controller.h"
#include "GUI/TestCase/Open/Model/open-testcase-model.h"

#include <QDialog>
#include <QString>
#include <QItemSelection>

/*
 * OpenTestCaseDialog создает массив объектов TestCase в которых
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
    explicit OpenTestCaseDialog(QWidget *parent = nullptr);
    ~OpenTestCaseDialog();

    //Dialog return collection of pointers to objects which can contain pointers
    //to another objects. If this objects are not used future it must be deleted
    //by owner of the dialog.
    QList<OpenTestCaseItem *> &GetOpenTestCaseItemList();

private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void on_testCaseSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);
    void on_currentConfigBox_currentIndexChanged(const QString &arg1);
    void on_isLoadResultBox_stateChanged(int arg1);
    void on_runDescriptionTableHeaderClicked(int arg1);
    void on_descriptionCheckBox_stateChanged(int arg1);
    void on_fromCheckBox_stateChanged(int arg1);
    void on_toCheckBox_stateChanged(int arg1);
    void on_setFilterBtn_clicked();
    void on_clearFilterBtn_clicked();
    void on_fromDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_toDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);
    void on_descriptionComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::OpenTestCaseDialog *ui;

    OpenTestCaseListController openTestCaseListController;
    OpenTestCaseRunDescriptionsController openTestCaseRunDescriptionsController;

    OpenTestCaseModel dataModel;
};

#endif // OPENTESTCASEDIALOG_H
