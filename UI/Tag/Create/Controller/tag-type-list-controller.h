#ifndef TAGTYPELISTCONTROLLER_H
#define TAGTYPELISTCONTROLLER_H

#include <QAbstractListModel>

class TagTypeListController : public QAbstractListModel
{
    Q_OBJECT

public:
    TagTypeListController();
    void Init(QStringList *d);

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QStringList *dataSource;
};

#endif // TAGTYPELISTCONTROLLER_H
