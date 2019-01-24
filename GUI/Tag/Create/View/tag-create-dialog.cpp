#include "tag-create-dialog.h"
#include "ui_tag-create-dialog.h"

#include "tag-create-empty-widget.h"

TagCreateDialog::TagCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagCreateDialog)
{
    ui->setupUi(this);

    tagTypeIDList = TagManager::GetTagAdapterIDList();
    tagTypeListController.Init(&tagTypeIDList);

    ui->tagTypeList->setModel(&tagTypeListController);

    connect(ui->tagTypeList->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_tagTypeListSelectionChanged(QItemSelection,QItemSelection)));

    for(int i = 0; i < tagTypeIDList.length(); i++)
    {
        ITagAdapter *tagAdapter = TagManager::GetTagAdapter(tagTypeIDList.at(i));
        if(tagAdapter != nullptr)
        {
            ITagWidget *tagWidget = tagAdapter->GetTagCreateWidget(this);
            if(tagWidget != nullptr)
            {
                ui->editorStacked->insertWidget(ui->editorStacked->count(), tagWidget);
            }
            else
            {
                QWidget *emptyWidget = new TagCreateEmptyWidget(this);
                ui->editorStacked->insertWidget(ui->editorStacked->count(), emptyWidget);
            }
        }
    }

    QModelIndex firstAdapterIndex = tagTypeListController.index(0);
    ui->tagTypeList->setCurrentIndex(firstAdapterIndex);
}

TagCreateDialog::~TagCreateDialog()
{
    delete ui;
}

QString TagCreateDialog::tagID()
{
    return tagTypeIDList.at(ui->tagTypeList->currentIndex().row());
}

QString TagCreateDialog::tagName()
{
    return ui->nameBox->text();
}

QString TagCreateDialog::tagData()
{
    QString config;
    QString data;
    ITagWidget *tagWidget = static_cast<ITagWidget *>(ui->editorStacked->currentWidget());
    tagWidget->GetData(config, data);

    return data;
}

void TagCreateDialog::on_tagTypeListSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    QModelIndex newSelectionFirstIndex = newSelection.at(0).indexes().at(0);
    int pos = newSelectionFirstIndex.row();

    if(pos < tagTypeIDList.length())
    {
        QString key = tagTypeIDList.at(pos);
        ITagAdapter *tagAdapter = TagManager::GetTagAdapter(key);
        if(tagAdapter != nullptr)
        {
            ui->descBox->setText(tagAdapter->Description());
        }
    }

    ui->editorStacked->setCurrentIndex(pos);
}
