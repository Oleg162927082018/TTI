#ifndef TAGVIEWFOLDERWIDGET_H
#define TAGVIEWFOLDERWIDGET_H

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

#endif // TAGVIEWFOLDERWIDGET_H
