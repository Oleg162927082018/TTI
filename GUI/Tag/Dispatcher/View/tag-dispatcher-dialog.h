#ifndef TAGCOLLECTIONDIALOG_H
#define TAGCOLLECTIONDIALOG_H

#include <QDialog>
#include <QItemSelection>

#include "../Controller/tag-collection-tree-adapter.h"

namespace Ui {
class TagDispatcherDialog;
}

class TagDispatcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagDispatcherDialog(int action, QWidget *parent = 0);
    ~TagDispatcherDialog();

    static const int ACTION_EDIT = 0;
    static const int ACTION_SELECT = 1;

    TagItem *getSelectResult();

private slots:
    void on_tagTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection);

    void on_newCollectionBtn_clicked();

    void on_newFolderBtn_clicked();

    void on_closeBtn_clicked();

    void on_removeBtn_clicked();

    void on_searchBtn_clicked();

    void on_searchNextBtn_clicked();

    void on_editBtn_clicked();

    void on_newTagBtn_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::TagDispatcherDialog *ui;

    TagItem *selectResult = NULL;

    bool isDialogUpdating = false;
    void BeginDialogUpdate();
    void EndDialogUpdate();

    TagCollectionTreeAdapter tagCollectionTreeAdapter;
    QMap<QString, QWidget *> tagDisplayWidgetBuffer;

    QModelIndex currentIndex;
    void saveTreeState();
    void saveExpandedState(QModelIndex &index);
    void loadTreeState(QString name);
    void loadExpandedState(QModelIndex &index);

    QString removeTagSubItems(TagItem *tagItem);
};

#endif // TAGCOLLECTIONDIALOG_H
