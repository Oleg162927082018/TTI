#ifndef TAGDISPATCHERCOLLECTIONWIDGET_H
#define TAGDISPATCHERCOLLECTIONWIDGET_H

#include <QWidget>

namespace Ui {
class TagDispatcherCollectionWidget;
}

class TagDispatcherCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TagDispatcherCollectionWidget(QWidget *parent = nullptr);
    ~TagDispatcherCollectionWidget();

    //
    void setData(QString description);
private:
    Ui::TagDispatcherCollectionWidget *ui;
};

#endif // TAGDISPATCHERCOLLECTIONWIDGET_H
