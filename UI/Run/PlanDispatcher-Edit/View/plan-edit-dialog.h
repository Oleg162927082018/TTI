#ifndef EDITPLANDIALOG_H
#define EDITPLANDIALOG_H

#include <QDialog>

namespace Ui {
class PlanEditDialog;
}

class PlanEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanEditDialog(QWidget *parent = nullptr);
    ~PlanEditDialog();

private:
    Ui::PlanEditDialog *ui;
};

#endif // EDITPLANDIALOG_H
