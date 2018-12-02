#ifndef SETFILTERDIALOG_H
#define SETFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class SetFilterDialog;
}

class SetFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetFilterDialog(QWidget *parent = nullptr);
    ~SetFilterDialog();

private:
    Ui::SetFilterDialog *ui;
};

#endif // SETFILTERDIALOG_H
