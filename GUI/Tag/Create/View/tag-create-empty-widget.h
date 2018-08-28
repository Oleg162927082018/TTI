#ifndef TAGCREATEEMPTYWIDGET_H
#define TAGCREATEEMPTYWIDGET_H

#include <QWidget>

namespace Ui {
class TagCreateEmptyWidget;
}

class TagCreateEmptyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagCreateEmptyWidget(QWidget *parent = 0);
    ~TagCreateEmptyWidget();

private:
    Ui::TagCreateEmptyWidget *ui;
};

#endif // TAGCREATEEMPTYWIDGET_H
