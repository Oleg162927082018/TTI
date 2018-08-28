#ifndef PLANADDDIALOG_H
#define PLANADDDIALOG_H

#include <QDialog>

namespace Ui {
class PlanAddDialog;
}

class PlanAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlanAddDialog(QWidget *parent = 0);
    ~PlanAddDialog();

private slots:
    void on_planBtn_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PlanAddDialog *ui;
};

#endif // PLANADDDIALOG_H
