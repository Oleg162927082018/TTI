#ifndef RUNHEADERSDIALOG_H
#define RUNHEADERSDIALOG_H

#include "Common/DBManager/run-description.h"
#include "GUI/RunHeader/RunHeadersDialog/Controller/run-headers-table-controller.h"

#include <QDialog>

namespace Ui {
class RunHeadersDialog;
}

class RunHeadersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunHeadersDialog(QWidget *parent = nullptr);
    ~RunHeadersDialog();

    void addRunDescription(QString name, bool visible, RunDescription *rd);
    QMap<int, RunHeaderItem *> *getRunDescriptions();

private slots:
    void on_descriptionCheckBox_stateChanged(int arg1);
    void on_fromCheckBox_stateChanged(int arg1);
    void on_toCheckBox_stateChanged(int arg1);
    void on_setFilterBtn_clicked();
    void on_clearFilterBtn_clicked();

private:
    Ui::RunHeadersDialog *ui;

    RunHeadersTableController runHeadersTableController;
    QMap<int, RunHeaderItem *> fullHeadersData;
    QMap<int, RunHeaderItem *> visibleHeadersData;
};

#endif // RUNHEADERSDIALOG_H
