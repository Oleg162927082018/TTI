#ifndef TAGDISPATCHERFOLDERWIDGET_H
#define TAGDISPATCHERFOLDERWIDGET_H

#include <QWidget>

namespace Ui {
class TagDispatcherFolderWidget;
}

class TagDispatcherFolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagDispatcherFolderWidget(QWidget *parent = nullptr);
    ~TagDispatcherFolderWidget();

    void setData(QString description);
private:
    Ui::TagDispatcherFolderWidget *ui;
};

#endif // TAGDISPATCHERFOLDERWIDGET_H
