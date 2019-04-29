#ifndef TAGFOLDERCREATEDIALOG_H
#define TAGFOLDERCREATEDIALOG_H

#include <QDialog>

namespace Ui {
class TagFolderCreateDialog;
}

class TagFolderCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagFolderCreateDialog(QWidget *parent = nullptr);
    ~TagFolderCreateDialog();

    QString folderName();
    QString folderDescription();


private:
    Ui::TagFolderCreateDialog *ui;
};

#endif // TAGFOLDERCREATEDIALOG_H
