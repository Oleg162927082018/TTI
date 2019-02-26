#ifndef TAGCOLLECTIONCREATEDIALOG_H
#define TAGCOLLECTIONCREATEDIALOG_H

#include <QDialog>

namespace Ui {
class TagCollectionCreateDialog;
}

class TagCollectionCreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagCollectionCreateDialog(QWidget *parent = nullptr);
    ~TagCollectionCreateDialog();

    QString collectionFileName();
    QString collectionDescription();

private slots:
    void on_fileNameBtn_clicked();

private:
    Ui::TagCollectionCreateDialog *ui;
};

#endif // TAGCOLLECTIONCREATEDIALOG_H
