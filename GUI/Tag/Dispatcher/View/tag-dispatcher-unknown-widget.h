#ifndef TAGUNKNOWNWIDGET_H
#define TAGUNKNOWNWIDGET_H

#include <QWidget>

namespace Ui {
class TagDispatcherUnknownWidget;
}

class TagDispatcherUnknownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagDispatcherUnknownWidget(QWidget *parent = nullptr);
    ~TagDispatcherUnknownWidget();

private:
    Ui::TagDispatcherUnknownWidget *ui;
};

#endif // TAGUNKNOWNWIDGET_H
