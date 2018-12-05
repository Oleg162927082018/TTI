#include "tag-view-dialog.h"
#include "ui_tag-view-dialog.h"

#include "TagManager/tagmanager.h"

TagViewDialog::TagViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagViewDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);
}

TagViewDialog::~TagViewDialog()
{
    delete ui;
}

void TagViewDialog::setTag(QString tagPath)
{
    QString config;

    ui->tagNameBox->setText(tagPath);
    TagItem *tagItem = TagManager::FindTagByPath(tagPath);
    if(tagItem == nullptr) { return; }

    ITagAdapter *tagAdapter = TagManager::GetTagAdapter(tagItem->tag->type);
    if(tagAdapter == nullptr) { return; }
    ITagWidget *viewTagWidget = tagAdapter->GetTagViewWidget(this);
    if(viewTagWidget == nullptr) { return; }

    viewTagWidget->SetData(config, tagItem->tag->data);
    ui->verticalLayout->insertWidget(1,viewTagWidget);
}

void TagViewDialog::on_closeBtn_clicked()
{
    close();
}
