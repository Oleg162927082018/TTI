#include "tag-view-dialog.h"
#include "ui_tag-view-dialog.h"

#include "TagManager/tagmanager.h"

TagViewDialog::TagViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagViewDialog)
{
    ui->setupUi(this);
}

TagViewDialog::~TagViewDialog()
{
    delete ui;
}

void TagViewDialog::setTag(QString tagPath)
{
    QDomDocument config;

    ui->tagNameBox->setText(tagPath);
    TagItem *tagItem = TagManager::FindTagByPath(tagPath);
    if(tagItem == NULL) { return; }

    ITagAdapter *tagAdapter = TagManager::GetTagAdapter(tagItem->tag->type);
    if(tagAdapter == NULL) { return; }
    ITagWidget *viewTagWidget = tagAdapter->GetTagViewWidget(this);
    if(viewTagWidget == NULL) { return; }

    viewTagWidget->SetData(&config, &(tagItem->tag->data));
    ui->verticalLayout->insertWidget(1,viewTagWidget);
}

void TagViewDialog::on_closeBtn_clicked()
{
    close();
}
