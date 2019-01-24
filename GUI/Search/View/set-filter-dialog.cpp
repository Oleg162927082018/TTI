#include "set-filter-dialog.h"
#include "set-filter-tag-folder-widget.h"
#include "set-filter-tag-unknown-widget.h"
#include "set-filter-tag-collection-widget.h"
#include "ui_set-filter-dialog.h"

SetFilterDialog::SetFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFilterDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    model.Init();
    tagCollectionTreeAdapter.InitDataSource(&(model.tagCollections));
    ui->tagTreeView->setModel(&tagCollectionTreeAdapter);

    connect(ui->tagTreeView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_tagTreeViewSelectionChanged(QItemSelection,QItemSelection)));

    SetFilterTagUnknownWidget *tagViewUnknownWidget = new SetFilterTagUnknownWidget();
    tagDisplayWidgetBuffer.insert("unknown", tagViewUnknownWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewUnknownWidget);

    SetFilterTagCollectionWidget *tagViewCollectionWidget = new SetFilterTagCollectionWidget();
    tagDisplayWidgetBuffer.insert("collection", tagViewCollectionWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewCollectionWidget);

    SetFilterTagFolderWidget *tagViewFolderWidget = new SetFilterTagFolderWidget();
    tagDisplayWidgetBuffer.insert("folder", tagViewFolderWidget);
    ui->tagItemWidgetsStack->addWidget(tagViewFolderWidget);

    QStringList tagTemplateIDs = TagManager::GetTagAdapterIDList();
    for(int i = 0; i < tagTemplateIDs.count(); i++)
    {
        QString key = tagTemplateIDs.at(i);
        ITagAdapter *adapter = TagManager::GetTagAdapter(key);
        ITagWidget *tagWidget = adapter->GetTagViewWidget(this);
        tagDisplayWidgetBuffer.insert(key, tagWidget);
        if(tagWidget != nullptr)
        {
            ui->tagItemWidgetsStack->addWidget(tagWidget);
        }
    }

    QModelIndex firstTagIndex = tagCollectionTreeAdapter.index(0,0);
    ui->tagTreeView->setCurrentIndex(firstTagIndex);
}

SetFilterDialog::~SetFilterDialog()
{
    delete ui;

    if(checkedTagLinks != nullptr) { delete checkedTagLinks; }
}

void SetFilterDialog::checkEqualCondition(int equalIndex, int equalResult, int equalValue, bool &isCondition)
{
    switch(equalIndex) {
        case 0: // ==
            if(equalResult == equalValue) { isCondition = true; }
            break;
        case 1: // <=
           if(equalResult <= equalValue) { isCondition = true; }
            break;
        case 2: // >=
            if(equalResult >= equalValue) { isCondition = true; }
            break;
        case 3: // !=
            if(equalResult != equalValue) { isCondition = true; }
            break;
    }
}

void SetFilterDialog::appendCondition(QString orAnd, bool condition, bool &sumCondition)
{
    if((orAnd.compare("AND") == 0)) {
        sumCondition &= condition;
    } else {
        sumCondition |= condition;
    }
}

bool SetFilterDialog::isCondition(MainWindowTableItem *item)
{

    bool isMarkChecked = (ui->markCheckBox->checkState() == Qt::Checked);
    bool isMarkCondition = false;

    if(isMarkChecked && (item->checked == (ui->markComboBox->currentText().compare("checked") == 0))) {
        isMarkCondition = true;
    }


    bool isNameChecked = (ui->nameCheckBox->checkState() == Qt::Checked);
    bool isNameCondition = false;

    if(isNameChecked && (item->name.contains(ui->nameComboBox->currentText()))) {
        isNameCondition = true;
    }


    bool isChangedChecked = (ui->changedCheckBox->checkState() == Qt::Checked);
    bool isChangedCondition = false;

    bool isExitCodeChecked = (ui->exitCodeCheckBox->checkState() == Qt::Checked);
    bool isExitCodeCondition = false;

    bool isTheBestChecked = (ui->diffTheBestCheckBox->checkState() == Qt::Checked);
    bool isTheBestCondition = false;

    bool isPreviousChecked = (ui->diffPreviousCheckBox->checkState() == Qt::Checked);
    bool isPreviousCondition = false;

    bool isTagChecked = (ui->tagCheckBox->checkState() == Qt::Checked);
    bool isTagCondition = false;

    int visHeaderCount = item->visibleTableHeaders->count();
    for(int i = 1; i <= qMin(ui->deepSpinBox->value(), visHeaderCount); i++) {

        int resultKey = item->visibleTableHeaders->keys().at(visHeaderCount - i);
        TestResult *result = item->results.value(resultKey);

        TestResult *previous = nullptr;
        if(i < visHeaderCount) {
            int previousKey = item->visibleTableHeaders->keys().at(visHeaderCount - i - 1);
            previous = item->results.value(previousKey);
        }

        if((isExitCodeChecked) && (result != nullptr)) {
            checkEqualCondition(ui->exitCodeEqualComboBox->currentIndex(), result->exitCode,
                                ui->exitCodeSpinBox->value(), isExitCodeCondition); }

        if((isTheBestChecked) && (result != nullptr) && (result->benchmark != nullptr)) {
            checkEqualCondition(ui->diffTheBestEqualComboBox->currentIndex(), result->benchmark->compareResult,
                                ui->diffTheBestSpinBox->value(), isTheBestCondition); }

        if((isPreviousChecked) && (result != nullptr) && (result->previous != nullptr)) {
            checkEqualCondition(ui->diffPreviousEqualComboBox->currentIndex(), result->previous->compareResult,
                                ui->diffPreviousSpinBox->value(), isPreviousCondition); }

        if((isChangedChecked) && (result != nullptr) && (previous != nullptr)) {
            if((result->color != previous->color) || (result->status.compare(previous->status) != 0)) {
                isChangedCondition = true;
            }
        }
    }

    if(isTagChecked) {
        if(checkedTagLinks == nullptr) { checkedTagLinks = model.getCheckedTagLinks(); }

        for(int i = 0; i < checkedTagLinks->length(); i++) {
            if(item->status->tags.contains(checkedTagLinks->at(i))) {
                isTagCondition = true;
                break;
            }
        }
    }

    bool summaryCondition = true;
    if(isMarkChecked && !isMarkCondition) { summaryCondition = false; }
    if(isNameChecked) { appendCondition(ui->nameOrAndBox->currentText(), isNameCondition, summaryCondition); }
    if(isChangedChecked) { appendCondition(ui->changedOrAndBox->currentText(), isChangedCondition, summaryCondition); }
    if(isExitCodeChecked) { appendCondition(ui->exitCodeOrAndBox->currentText(), isExitCodeCondition, summaryCondition); }
    if(isTheBestChecked) { appendCondition(ui->diffTheBestOrAndBox->currentText(), isTheBestCondition, summaryCondition); }
    if(isPreviousChecked) { appendCondition(ui->diffPreviousOrAndBox->currentText(), isPreviousCondition, summaryCondition); }
    if(isTagChecked) { appendCondition(ui->tagOrAndBox->currentText(), isTagCondition, summaryCondition); }

    return summaryCondition;
}

void SetFilterDialog::on_markCheckBox_clicked(bool checked)
{
    ui->markComboBox->setEnabled(checked);
    ui->markLabel->setEnabled(checked);
}

void SetFilterDialog::on_nameCheckBox_clicked(bool checked)
{
    ui->nameOrAndBox->setEnabled(checked);
    ui->nameComboBox->setEnabled(checked);

}

void SetFilterDialog::on_exitCodeCheckBox_clicked(bool checked)
{
    ui->exitCodeOrAndBox->setEnabled(checked);
    ui->exitCodeEqualComboBox->setEnabled(checked);
    ui->exitCodeSpinBox->setEnabled(checked);
}

void SetFilterDialog::on_diffTheBestCheckBox_clicked(bool checked)
{
    ui->diffTheBestOrAndBox->setEnabled(checked);
    ui->diffTheBestEqualComboBox->setEnabled(checked);
    ui->diffTheBestSpinBox->setEnabled(checked);
}

void SetFilterDialog::on_diffPreviousCheckBox_clicked(bool checked)
{
    ui->diffPreviousOrAndBox->setEnabled(checked);
    ui->diffPreviousEqualComboBox->setEnabled(checked);
    ui->diffPreviousSpinBox->setEnabled(checked);
}

void SetFilterDialog::on_tagCheckBox_clicked(bool checked)
{
    ui->tagOrAndBox->setEnabled(checked);
    ui->tagCollectionSplitter->setEnabled(checked);
}

void SetFilterDialog::on_statusCheckBox_clicked(bool checked)
{
    ui->statusOrAndBox->setEnabled(checked);
    ui->statusTableView->setEnabled(checked);
}

void SetFilterDialog::on_changedCheckBox_clicked(bool checked)
{
    ui->changedOrAndBox->setEnabled(checked);
}

void SetFilterDialog::on_tagTreeViewSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    if(newSelection.length() > 0)
    {
        QModelIndex newSelectionFirstIndex = newSelection.at(0).indexes().at(0);
        SetFilterTagItem *item = static_cast<SetFilterTagItem*>(newSelectionFirstIndex.internalPointer());

        QString config;

        if(item->tag != nullptr)
        {
            QWidget *widget = tagDisplayWidgetBuffer.value(item->tag->type);
            if(widget == nullptr)
            {
                widget = tagDisplayWidgetBuffer.value("unknown");
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
            SetFilterTagFolderWidget *widget = static_cast<SetFilterTagFolderWidget *>(tagDisplayWidgetBuffer.value("folder"));
            widget->setData(item->folder->description);
            ui->tagItemWidgetsStack->setCurrentWidget(widget);
        }
        else
        {
            SetFilterTagCollectionWidget *widget = static_cast<SetFilterTagCollectionWidget *>(tagDisplayWidgetBuffer.value("collection"));
            widget->setData(item->collection->description);
            ui->tagItemWidgetsStack->setCurrentWidget(widget);
        }
    }
}

void SetFilterDialog::on_SetFilterDialog_accepted()
{
    if(checkedTagLinks != nullptr) {
        delete checkedTagLinks;
        checkedTagLinks = nullptr;
    }
}
