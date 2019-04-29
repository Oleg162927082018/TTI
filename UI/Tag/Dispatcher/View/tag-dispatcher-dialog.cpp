#include "tag-dispatcher-dialog.h"
#include "ui_tag-dispatcher-dialog.h"

#include "tag-dispatcher-unknown-widget.h"
#include "tag-dispatcher-collection-widget.h"
#include "tag-dispatcher-folder-widget.h"

#include <UI/Tag/Create/View/tag-collection-create-dialog.h>
#include <UI/Tag/Create/View/tag-folder-create-dialog.h>
#include "UI/Tag/Create/View/tag-create-dialog.h"
#include "UI/Tag/Edit/View/tag-edit-dialog.h"

#include <QDir>
#include <QMessageBox>
#include <QStack>


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
            ui->cancelBtn->setVisible(true);
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

    model.Init();
    treeAdapter.InitDataSource(&(model.tagCollections));
    ui->tagTreeView->setModel(&treeAdapter);

    connect(ui->tagTreeView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_tagTreeViewSelectionChanged(QItemSelection,QItemSelection)));

    TagDispatcherUnknownWidget *tagViewUnknownWidget = new TagDispatcherUnknownWidget();
    widgetBuffer.insert("unknown", tagViewUnknownWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewUnknownWidget);

    TagDispatcherCollectionWidget *tagViewCollectionWidget = new TagDispatcherCollectionWidget();
    widgetBuffer.insert("collection", tagViewCollectionWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewCollectionWidget);

    TagDispatcherFolderWidget *tagViewFolderWidget = new TagDispatcherFolderWidget();
    widgetBuffer.insert("folder", tagViewFolderWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewFolderWidget);

    QStringList tagTemplateIDs = TagManager::GetTagAdapterIDList();
    for(int i = 0; i < tagTemplateIDs.count(); i++)
    {
        QString key = tagTemplateIDs.at(i);
        ITagAdapter *adapter = TagManager::GetTagAdapter(key);
        ITagWidget *tagWidget = adapter->GetTagViewWidget(this);
        widgetBuffer.insert(key, tagWidget);
        if(tagWidget != nullptr)
        {
            ui->tagItemWidgetsStack->addWidget(tagWidget);
        }
    }

    QModelIndex firstTagIndex = treeAdapter.index(0,0);
    ui->tagTreeView->setCurrentIndex(firstTagIndex);

    //Create context menu for tag tree
    //Create, Load, Remove, Delete - collection
    //New, Edit, Delete - Folder
    //New, Edit, View, Delete - Tag
}

TagDispatcherDialog::~TagDispatcherDialog()
{
    delete ui;
}

TagDispatcherItem *TagDispatcherDialog::getSelectedResult()
{
    return selectedResult;
}

void TagDispatcherDialog::on_tagTreeViewSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    if(newSelection.length() > 0)
    {
        QModelIndex newSelectionFirstIndex = newSelection.at(0).indexes().at(0);
        TagDispatcherItem *item = static_cast<TagDispatcherItem*>(newSelectionFirstIndex.internalPointer());

        QString config;

        if(item->tag != nullptr)
        {
            QWidget *widget = widgetBuffer.value(item->tag->type);
            if(widget == nullptr)
            {
                widget = widgetBuffer.value("unknown");
            }
            else
            {
                ITagWidget *tagWidget = static_cast<ITagWidget *>(widget);
                tagWidget->SetData(config, item->tag->data);
            }

            ui->tagItemWidgetsStack->setCurrentWidget(widget);
        }
        else if(item->folder != nullptr)
        {
            TagDispatcherFolderWidget *widget = static_cast<TagDispatcherFolderWidget *>(widgetBuffer.value("folder"));
            widget->setData(item->folder->description);
            ui->tagItemWidgetsStack->setCurrentWidget(widget);
        }
        else
        {
            TagDispatcherCollectionWidget *widget = static_cast<TagDispatcherCollectionWidget *>(widgetBuffer.value("collection"));
            widget->setData(item->collection->description);
            ui->tagItemWidgetsStack->setCurrentWidget(widget);
        }
    }
}

void TagDispatcherDialog::saveTreeState()
{
    currentIndex = ui->tagTreeView->currentIndex();

    for(int i = 0; i < model.tagCollections.length(); i++)
    {
        QModelIndex rootIndex = treeAdapter.index(i, 0);
        saveExpandedState(rootIndex);
    }
}

void TagDispatcherDialog::saveExpandedState(QModelIndex &index)
{
    TagDispatcherItem* item = static_cast<TagDispatcherItem*>(index.internalPointer());
    item->expanded = ui->tagTreeView->isExpanded(index);

    for(int i = 0; i < item->subItems.length(); i++)
    {
        QModelIndex subIndex = treeAdapter.index(i, 0, index);
        saveExpandedState(subIndex);
    }
}

void TagDispatcherDialog::loadTreeState(TagDispatcherItem *selectedItem)
{
    for(int i = 0; i < model.tagCollections.length(); i++)
    {
        QModelIndex rootIndex = treeAdapter.index(i,0);
        loadExpandedState(rootIndex);
    }

    if(selectedItem == nullptr)
    {
        if(currentIndex.isValid())
        {
            ui->tagTreeView->setCurrentIndex(currentIndex);
        }
        else
        {
            ui->tagTreeView->setCurrentIndex(treeAdapter.index(0,0));
        }
    }
    else
    {
        QStack<TagDispatcherItem *> parentItems;
        TagDispatcherItem *parentItem = selectedItem;
        while(parentItem != nullptr) {
            parentItems.push(parentItem);
            parentItem = parentItem->parent;
        }

        QModelIndex index;
        while(!parentItems.isEmpty()) {
            TagDispatcherItem *item = parentItems.pop();
            if(item->parent == nullptr) {
                int i = model.tagCollections.indexOf(item);
                index = treeAdapter.index(i,0);
                if(parentItems.isEmpty()) { ui->tagTreeView->setCurrentIndex(index); }
                else { ui->tagTreeView->setExpanded(index, true); }

            } else {
                int i = item->parent->subItems.indexOf(item);
                index = treeAdapter.index(i,0,index);
                if(parentItems.isEmpty()) { ui->tagTreeView->setCurrentIndex(index); }
                else { ui->tagTreeView->setExpanded(index, true); }
            }
        }
    }
}

void TagDispatcherDialog::loadExpandedState(QModelIndex &index)
{
    TagDispatcherItem* item = static_cast<TagDispatcherItem*>(index.internalPointer());
    ui->tagTreeView->setExpanded(index, item->expanded);

    for(int i = 0; i < item->subItems.length(); i++)
    {
        QModelIndex subIndex = treeAdapter.index(i, 0, index);
        loadExpandedState(subIndex);
    }
}

void TagDispatcherDialog::on_newCollectionBtn_clicked()
{
    //Create default folder for save tag collections
    QDir tagFolder(qApp->applicationDirPath());
    tagFolder.mkdir("tags");

    //Get file name for tag collection
    TagCollectionCreateDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted) {
        QString collectionFileName = dlg.collectionFileName();
        QString collectionDescription = dlg.collectionDescription();
        if (!collectionFileName.isEmpty())
        {
            saveTreeState();
            treeAdapter.beginResetModel();

            TagDispatcherItem *newItem = model.NewTagCollection(collectionFileName, collectionDescription);

            treeAdapter.endResetModel();
            loadTreeState(newItem);
        }
    }
}

void TagDispatcherDialog::on_newFolderBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();

    TagDispatcherItem *tagItem = static_cast<TagDispatcherItem*>(index.internalPointer());
    if(tagItem == nullptr) { return; }

    if(tagItem->tag == nullptr)
    {
        TagFolderCreateDialog dlg(this);
        if(dlg.exec() == QDialog::Accepted) {
            QString folderName = dlg.folderName();
            QString folderDescription = dlg.folderDescription();
            if (!folderName.isEmpty())
            {
                saveTreeState();
                treeAdapter.beginResetModel();

                TagDispatcherItem *newItem = model.NewTagFolder(tagItem, folderName, folderDescription);

                treeAdapter.endResetModel();
                loadTreeState(newItem);
            }
        }
    }
}

void TagDispatcherDialog::on_newTagBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();

    TagDispatcherItem *tagItem = static_cast<TagDispatcherItem*>(index.internalPointer());
    if(tagItem == nullptr) { return; }

    if(tagItem->tag != nullptr) { tagItem = tagItem->parent; }

    TagCreateDialog dlg(this);
    if(dlg.exec() == QDialog::Accepted) {
        QString tagID = dlg.tagID();
        QString tagName = dlg.tagName();
        QString tagData = dlg.tagData();
        if (!tagName.isEmpty() && !tagID.isEmpty() && !tagData.isEmpty())
        {
            saveTreeState();
            treeAdapter.beginResetModel();

            TagDispatcherItem *newItem = model.NewTag(tagItem, tagID, tagName, tagData);

            treeAdapter.endResetModel();
            loadTreeState(newItem);
        }
    }
}

void TagDispatcherDialog::on_closeBtn_clicked()
{
    close();
}

void TagDispatcherDialog::on_removeBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();

    TagDispatcherItem *tagItem = static_cast<TagDispatcherItem*>(index.internalPointer());
    if(tagItem == nullptr) { return; }

    if(tagItem->collection != nullptr) {
        QMessageBox mb("Question",
                       "Delete collection permanently?",
                       QMessageBox::Question,
                       QMessageBox::Yes,
                       QMessageBox::No | QMessageBox::Default,
                       QMessageBox::Cancel);

        int mbResult = mb.exec();
        if(mbResult != QMessageBox::Cancel)
        {
            saveTreeState();
            treeAdapter.beginResetModel();

            TagDispatcherItem *parentItem = tagItem->parent;
            model.DeleteTagItem(tagItem, mbResult == QMessageBox::Yes);

            treeAdapter.endResetModel();
            loadTreeState(parentItem);
        }
    } else {
        QMessageBox mb("Confirmation",
                       "Are you shure to remove element? Element will be deleted permanently.",
                       QMessageBox::Question,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::Cancel,
                       QMessageBox::NoButton);

        if(mb.exec() == QMessageBox::Ok)
        {
            saveTreeState();
            treeAdapter.beginResetModel();

            TagDispatcherItem *parentItem = tagItem->parent;
            model.DeleteTagItem(tagItem, true);

            treeAdapter.endResetModel();
            loadTreeState(parentItem);
        }
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
    QModelIndex index = ui->tagTreeView->currentIndex();

    TagDispatcherItem *tagItem = static_cast<TagDispatcherItem*>(index.internalPointer());
    if(tagItem == nullptr) { return; }

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

void TagDispatcherDialog::on_okBtn_clicked()
{
    QModelIndex index = ui->tagTreeView->currentIndex();
    selectedResult = static_cast<TagDispatcherItem*>(index.internalPointer());

    if((selectedResult != nullptr) && (selectedResult->tag != nullptr))
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
    selectedResult = nullptr;
    close();
}
