#ifndef TAGVIEWFOLDERWIDGET_H
#define TAGVIEWFOLDERWIDGET_H

#include <QWidget>

namespace Ui {
class TagViewFolderWidget;
}

class TagViewFolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagViewFolderWidget(QWidget *parent = 0);
    ~TagViewFolderWidget();

    void setData(QString description);
private:
    Ui::TagViewFolderWidget *ui;
};

#endif // TAGVIEWFOLDERWIDGET_H
