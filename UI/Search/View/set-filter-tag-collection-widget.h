#ifndef SETFILTERTAGCOLLECTIONWIDGET_H
#define SETFILTERTAGCOLLECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class SetFilterTagCollectionWidget;
}

class SetFilterTagCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetFilterTagCollectionWidget(QWidget *parent = nullptr);
    ~SetFilterTagCollectionWidget();

    void setData(QString description);

private:
    Ui::SetFilterTagCollectionWidget *ui;
};

#endif // SETFILTERTAGCOLLECTIONWIDGET_H
