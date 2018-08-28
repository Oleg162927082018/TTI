#ifndef RUNDESCRIPTIONSHOWDIALOG_H
#define RUNDESCRIPTIONSHOWDIALOG_H

#include "GUI/Run/Common/RunDescription/View/run-description-table-widget.h"

#include <QDialog>

namespace Ui {
class RunDescriptionShowDialog;
}

class RunDescriptionShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunDescriptionShowDialog(QMap<int, RunDescription *> *runDescriptionList, QWidget *parent = 0);
    ~RunDescriptionShowDialog();

private:
    Ui::RunDescriptionShowDialog *ui;
    RunDescriptionTableWidget *runDescriptionTableWidget;
};

#endif // RUNDESCRIPTIONSHOWDIALOG_H
