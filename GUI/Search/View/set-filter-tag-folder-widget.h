#ifndef SETFILTERTAGFOLDERWIDGET_H
#define SETFILTERTAGFOLDERWIDGET_H

#include <QWidget>

namespace Ui {
class SetFilterTagFolderWidget;
}

class SetFilterTagFolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetFilterTagFolderWidget(QWidget *parent = nullptr);
    ~SetFilterTagFolderWidget();

    void setData(QString description);

private:
    Ui::SetFilterTagFolderWidget *ui;
};

#endif // SETFILTERTAGFOLDERWIDGET_H
