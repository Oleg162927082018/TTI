#include "tag-dispatcher-dialog.h"
#include "ui_tag-dispatcher-dialog.h"

#include "tag-view-unknown-widget.h"
#include "tag-view-collection-widget.h"
#include "tag-view-folder-widget.h"
#include "../../../Tag/Create/View/tag-create-dialog.h"
#include "../../../Tag/Edit/View/tag-edit-dialog.h"

#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

TagDispatcherDialog::TagDispatcherDialog(int action, QWidget *parent) :
    QDialog(parent), ui(new Ui::TagDispatcherDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    switch(action)
    {
        case TagDispatcherDialog::ACTION_SELECT:
            ui->closeBtn->setVisible(false);
            ui->okBtn->setDefault(true);
            break;
        case TagDispatcherDialog::ACTION_EDIT:
            ui->okBtn->setVisible(false);
            ui->cancelBtn->setVisible(false);
            ui->closeBtn->setDefault(true);
            break;
        default:
            break;
    }

    ui->tagTreeView->setModel(&tagCollectionTreeAdapter);

    connect(ui->tagTreeView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_tagTreeViewSelectionChanged(QItemSelection,QItemSelection)));

    TagViewUnknownWidget *tagViewUnknownWidget = new TagViewUnknownWidget();
    tagDisplayWidgetBuffer.insert("unknown", tagViewUnknownWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewUnknownWidget);

    TagViewCollectionWidget *tagViewCollectionWidget = new TagViewCollectionWidget();
    tagDisplayWidgetBuffer.insert("collection", tagViewCollectionWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewCollectionWidget);

    TagViewFolderWidget *tagViewFolderWidget = new TagViewFolderWidget();
    tagDisplayWidgetBuffer.insert("folder", tagViewFolderWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewFolderWidget);

    QStringList tagTemplateIDs = TagManager::GetTagAdapterIDList();
    for(int i = 0; i < tagTemplateIDs.count(); i++)
    {
        QString key = tagTemplateIDs.at(i);
        ITagAdapter *adapter = TagManager::GetTagAdapter(key);
        ITagWidget *tagWidget = adapter->GetTagViewWidget(this);
        tagDisplayWidgetBuffer.insert(key, tagWidget);
        if(tagWidget != NULL)
        {
            ui->tagItemWidgetsStack->addWidget(tagWidget);
        }
    }

    //Create context menu for tag tree
    //Create, Load, Remove, Delete - collection
    //New, Edit, Delete - Folder
    //New, Edit, View, Delete - Tag
}

TagDispatcherDialog::~TagDispatcherDialog()
{
    delete ui;
}

TagItem *TagDispatcherDialog::getSelectResult()
{
    return selectResult;
}

void TagDispatcherDialog::on_tagTreeViewSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    if(!isDialogUpdating)
    {
        if(newSelection.length() > 0)
        {
            QModelIndex newSelectionFirstIndex = newSelection.at(0).indexes().at(0);
            TagItem *item = static_cast<TagItem*>(newSelectionFirstIndex.internalPointer());

            QDomDocument config;

            if(item->tag != NULL)
            {
                QWidget *widget = tagDisplayWidgetBuffer.value(item->tag->type);
                if(widget == NULL)
                {
                    widget = tagDisplayWidgetBuffer.value("unknown");
                }
                else
                {
                    ITagWidget *tagWidget = static_cast<ITagWidget *>(widget);
                    tagWidget->SetData(&config, &(item->tag->data));
                }

                ui->tagItemWidgetsStack->setCurrentWidget(widget);
            }
            else if(item->folder != NULL)
            {
                TagViewFolderWidget *widget = static_cast<TagViewFolderWidget *>(tagDisplayWidgetBuffer.value("folder"));
                widget->setData(item->folder->description);
                ui->tagItemWidgetsStack->setCurrentWidget(widget);
            }
            else
            {
                TagViewCollectionWidget *widget = static_cast<TagViewCollectionWidget *>(tagDisplayWidgetBuffer.value("collection"));
                widget->setData(item->collection->description);
                ui->tagItemWidgetsStack->setCurrentWidget(widget);
            }
        }
    }
}

void TagDispatcherDialog::saveTreeState()
{
    currentIndex = ui->tagTreeView->currentIndex();

    int tagCollections = TagManager::GetTagCollectionCount();
    if(tagCollections > 0)
    {
        for(int i = 0; i < tagCollections; i++)
        {
            QModelIndex rootIndex = tagCollectionTreeAdapter.index(i, 0);
            saveExpandedState(rootIndex);
        }
    }
}

void TagDispatcherDialog::saveExpandedState(QModelIndex &index)
{
    TagItem* item = static_cast<TagItem*>(index.internalPointer());
    item->expanded = ui->tagTreeView->isExpanded(index);

    for(int i = 0; i < item->subItems.length(); i++)
    {
        QModelIndex subIndex = tagCollectionTreeAdapter.index(i, 0, index);
        saveExpandedState(subIndex);
    }
}

void TagDispatcherDialog::loadTreeState(QString name)
{
    if(TagManager::GetTagCollectionCount() > 0)
    {
        for(int i = 0; i < TagManager::GetTagCollectionCount(); i++)
        {
            QModelIndex rootIndex = tagCollectionTreeAdapter.index(i,0);
            loadExpandedState(rootIndex);
        }

        if(name.isEmpty() || name.isNull())
        {
            if(currentIndex.isValid())
            {
                ui->tagTreeView->setCurrentIndex(currentIndex);
            }
            else
            {
                ui->tagTreeView->setCurrentIndex(tagCollectionTreeAdapter.index(0,0));
            }
        }
        else
        {
            ui->tagTreeView->setCurrentIndex(
                        tagCollectionTreeAdapter.indexByParth(name));
        }
    }

}

void TagDispatcherDialog::loadExpandedState(QModelIndex &index)
{
    TagItem* item = static_cast<TagItem*>(index.internalPointer());
    ui->tagTreeView->setExpanded(index, item->expanded);

    for(int i = 0; i < item->subItems.length(); i++)
    {
        QModelIndex subIndex = tagCollectionTreeAdapter.index(i, 0, index);
        loadExpandedState(subIndex);
    }
}

void TagDispatcherDialog::on_newCollectionBtn_clicked()
{
    //Create default folder for save tag collections
    QDir tagFolder(qApp->applicationDirPath() + "/tag");
    tagFolder.mkdir("data");

    //Get file name for tag collection
    QString tagCollectionFullFileName = QFileDialog::getSaveFileName(this, "New Tag collection",
                               tagFolder.filePath("data"), "Tag collection (*.xml)");

    if(!tagCollectionFullFileName.isEmpty()&& !tagCollectionFullFileName.isNull())
    {
        saveTreeState();
        BeginDialogUpdate();
        tagCollectionTreeAdapter.beginResetModel();

        TagManager::CreateTagCollection(tagCollectionFullFileName);

        tagCollectionTreeAdapter.endResetModel();
        EndDialogUpdate();
        loadTreeState(QFileInfo(tagCollectionFullFileName).baseName());
    }
}

void TagDispatcherDialog::BeginDialogUpdate()
{
    isDialogUpdating = true;
}

void TagDispatcherDialog::EndDialogUpdate()
{
    isDialogUpdating = false;
}

void TagDispatcherDialog::on_newFolderBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();
    TagItem *tagItem = static_cast<TagItem*>(index.internalPointer());

    if(tagItem->tag == NULL)
    {
        bool ok;
        QString folderName = QInputDialog::getText(this, tr("New folder"), tr("Folder name:"), QLineEdit::Normal, "", &ok);

        if (ok && !folderName.isEmpty())
        {
            saveTreeState();
            BeginDialogUpdate();
            tagCollectionTreeAdapter.beginResetModel();

            TagItem *newItem = TagManager::CreateTagFolder(tagItem, folderName);

            tagCollectionTreeAdapter.endResetModel();
            EndDialogUpdate();
            loadTreeState(newItem->path);
        }
    }
}

void TagDispatcherDialog::on_closeBtn_clicked()
{
    close();
}

QString TagDispatcherDialog::removeTagSubItems(TagItem *tagItem)
{
    for(int i = 0; i < tagItem->subItems.length(); i++)
    {
        removeTagSubItems(tagItem->subItems.at(i));
    }

    if(tagItem->tag != NULL)
    {

        TagItem *parent = tagItem->parent;
        parent->subItems.removeOne(tagItem);

        TagItem *tagCollection = parent;
        while(tagCollection->parent != NULL) { tagCollection = tagCollection->parent; }
        TagManager::SaveTagCollection(tagCollection);

        delete tagItem->tag;
        delete tagItem;

        return parent->path;
    }

    if(tagItem->folder != NULL)
    {
        TagItem *parent = tagItem->parent;
        parent->subItems.removeOne(tagItem);

        TagItem *tagCollection = parent;
        while(tagCollection->parent != NULL) { tagCollection = tagCollection->parent; }
        TagManager::SaveTagCollection(tagCollection);

        delete tagItem->folder;
        delete tagItem;

        return parent->path;
    }

    if(tagItem->collection != NULL)
    {
        TagManager::DeleteTagCollection(tagItem->collection->fullFileName);
        delete tagItem->collection;
        delete tagItem;

        return "";
    }

    return NULL;
}

void TagDispatcherDialog::on_removeBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();
    TagItem *tagItem = static_cast<TagItem*>(index.internalPointer());

    QMessageBox mb("Confirmation",
                   "Are you shure to remove element? Element will be deleted permanently.",
                   QMessageBox::Question,
                   QMessageBox::Ok | QMessageBox::Default,
                   QMessageBox::Cancel,
                   QMessageBox::NoButton);

    if(mb.exec() == QMessageBox::Ok)
    {
        saveTreeState();
        BeginDialogUpdate();
        tagCollectionTreeAdapter.beginResetModel();

        QString new_path = removeTagSubItems(tagItem);

        tagCollectionTreeAdapter.endResetModel();
        EndDialogUpdate();
        loadTreeState(new_path);
    }
}

void TagDispatcherDialog::on_searchBtn_clicked()
{
}

void TagDispatcherDialog::on_searchNextBtn_clicked()
{
}
void TagDispatcherDialog::on_editBtn_clicked()
{
    TagEditDialog tagEditDialog;
    if(tagEditDialog.exec() == QDialog::Accepted)
    {
        QMessageBox mb("Accepted",
                       "Accepted edit tag!",
                       QMessageBox::Information,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();
    }
}

void TagDispatcherDialog::on_newTagBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();
    TagItem *tagItem = static_cast<TagItem*>(index.internalPointer());
    if(tagItem->tag != NULL) { tagItem = tagItem->parent; }

    saveTreeState();
    BeginDialogUpdate();

    TagItem *newItem = TagCreateDialog::createTag(this, tagItem);

    tagCollectionTreeAdapter.beginResetModel();
    tagCollectionTreeAdapter.endResetModel();

    EndDialogUpdate();
    loadTreeState(newItem->path);
}

void TagDispatcherDialog::on_okBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();
    selectResult = static_cast<TagItem*>(index.internalPointer());

    if((selectResult != NULL) && (selectResult->tag != NULL))
    {
        close();
    }
    else
    {
        QMessageBox mb("Warning",
                       "You should select tag or press Cancel button.",
                       QMessageBox::Warning,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();
    }
}

void TagDispatcherDialog::on_cancelBtn_clicked()
{
    //selectResult = NULL;
    close();
}
