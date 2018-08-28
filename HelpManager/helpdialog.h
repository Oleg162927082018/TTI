#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QHelpEngine>
#include <QTextBrowser>
#include <QTabWidget>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QHelpEngine *helpEng, QWidget *parent = 0);
    ~HelpDialog();

    void showHelp(int activeTab);
    //0 - Contents
    //1 - Index

private:
    Ui::HelpDialog *ui;
    QHelpEngine *helpEngine;
    QTextBrowser *textViewer;
    QTabWidget* tabWidget;
};

#endif // HELPDIALOG_H
