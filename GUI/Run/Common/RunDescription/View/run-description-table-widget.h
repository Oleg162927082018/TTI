#ifndef RUNDESCRIPTIONLISTWIDGET_H
#define RUNDESCRIPTIONLISTWIDGET_H

#include "../Controller/run-description-table-widget-controller.h"

#include <QWidget>

namespace Ui {
class RunDescriptionTableWidget;
}

class RunDescriptionTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RunDescriptionTableWidget(QWidget *parent = 0);
    ~RunDescriptionTableWidget();

    void SetDataSource(QMap<int, RunDescription *>  *runDescriptionList);

private slots:
    void on_runDescriptionTableHeaderClicked(int arg1);

private:
    Ui::RunDescriptionTableWidget *ui;
    RunDescriptionTableWidgetController runDescriptionTableController;
};

#endif // RUNDESCRIPTIONLISTWIDGET_H
