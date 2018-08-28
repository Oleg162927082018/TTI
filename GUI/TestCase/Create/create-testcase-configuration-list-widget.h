#ifndef CREATETESTCASETEMPLATEDETAILWIDGET_H
#define CREATETESTCASETEMPLATEDETAILWIDGET_H

#include <QWidget>

namespace Ui {
class CreateTestCaseConfigurationListWidget;
}

class CreateTestCaseConfigurationListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTestCaseConfigurationListWidget(QString id, QWidget *parent = 0);
    ~CreateTestCaseConfigurationListWidget();

    bool SaveTestCase(QString fullFileName);

private slots:
    void on_newConfigurationButton_clicked();

    void on_configTabs_tabCloseRequested(int index);

private:
    Ui::CreateTestCaseConfigurationListWidget *ui;

    QString ID;
};

#endif // CREATETESTCASETEMPLATEDETAILWIDGET_H
