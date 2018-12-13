#include "set-filter-dialog.h"
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
}

SetFilterDialog::~SetFilterDialog()
{
    delete ui;
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

        if((isTheBestChecked) && (result != nullptr)) {
            checkEqualCondition(ui->diffTheBestEqualComboBox->currentIndex(), result->benchmarkCompareResult,
                                ui->diffTheBestSpinBox->value(), isTheBestCondition); }

        if((isPreviousChecked) && (result != nullptr)) {
            checkEqualCondition(ui->diffPreviousEqualComboBox->currentIndex(), result->previousCompareResult,
                                ui->diffPreviousSpinBox->value(), isPreviousCondition); }

        if((isChangedChecked) && (result != nullptr) && (previous != nullptr)) {
            if((result->color != previous->color) || (result->status.compare(previous->status) != 0)) {
                isChangedCondition = true;
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
    ui->tagTableView->setEnabled(checked);
    ui->tagTreeView->setEnabled(checked);
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
