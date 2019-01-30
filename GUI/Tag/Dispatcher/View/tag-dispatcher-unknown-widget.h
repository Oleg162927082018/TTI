#ifndef TAGDISPATCHERUNKNOWNWIDGET_H
#define TAGDISPATCHERUNKNOWNWIDGET_H

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

#endif // TAGDISPATCHERUNKNOWNWIDGET_H
