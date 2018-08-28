#ifndef TAGUNKNOWNWIDGET_H
#define TAGUNKNOWNWIDGET_H

#include <QWidget>

namespace Ui {
class TagViewUnknownWidget;
}

class TagViewUnknownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagViewUnknownWidget(QWidget *parent = 0);
    ~TagViewUnknownWidget();

private:
    Ui::TagViewUnknownWidget *ui;
};

#endif // TAGUNKNOWNWIDGET_H
