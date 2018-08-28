#ifndef TAGVIEWDIALOG_H
#define TAGVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class TagViewDialog;
}

class TagViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagViewDialog(QWidget *parent = 0);
    ~TagViewDialog();

    void setTag(QString tagPath);

private slots:
    void on_closeBtn_clicked();

private:
    Ui::TagViewDialog *ui;
};

#endif // TAGVIEWDIALOG_H
