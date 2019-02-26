#ifndef SETFILTERSTATUSTESTCASEADAPTER_H
#define SETFILTERSTATUSTESTCASEADAPTER_H

#include <QAbstractListModel>

#include <GUI/Search/Model/set-filter-status-collection.h>

class SetFilterTestCaseListAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    SetFilterTestCaseListAdapter();
    void Init(QList<SetFilterStatusCollection *> *d);

// QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<SetFilterStatusCollection *> *dataSource;

};

#endif // SETFILTERSTATUSTESTCASEADAPTER_H
