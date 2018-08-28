#ifndef TAGVIEWCOLLECTIONWIDGET_H
#define TAGVIEWCOLLECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class TagViewCollectionWidget;
}

class TagViewCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagViewCollectionWidget(QWidget *parent = 0);
    ~TagViewCollectionWidget();

    //
    void setData(QString description);
private:
    Ui::TagViewCollectionWidget *ui;
};

#endif // TAGVIEWCOLLECTIONWIDGET_H
