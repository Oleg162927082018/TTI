#ifndef SETFILTERTAGUNKNOWNWIDGET_H
#define SETFILTERTAGUNKNOWNWIDGET_H

#include <QWidget>

namespace Ui {
class SetFilterTagUnknownWidget;
}

class SetFilterTagUnknownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetFilterTagUnknownWidget(QWidget *parent = nullptr);
    ~SetFilterTagUnknownWidget();

private:
    Ui::SetFilterTagUnknownWidget *ui;
};

#endif // SETFILTERTAGUNKNOWNWIDGET_H
