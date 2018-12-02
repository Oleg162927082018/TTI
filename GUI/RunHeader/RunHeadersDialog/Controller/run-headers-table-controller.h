#ifndef RUNDESCRIPTIONLISTVIEWCONTROLLER_H
#define RUNDESCRIPTIONLISTVIEWCONTROLLER_H

#include "Common/DBManager/run-description.h"

#include <QList>
#include <QAbstractListModel>

#include "GUI/RunHeader/RunHeadersDialog/Model/run-header-item.h"

class RunHeadersTableController : public QAbstractTableModel
{

    Q_OBJECT

public:
    explicit RunHeadersTableController(QObject* parent = nullptr);

    void beginResetModel();
    void endResetModel();

    void setDataSource(QMap<int, RunHeaderItem *>  *runHeaderItems);
    void setAllChecked();

// QAbstractItemModel interface
private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QMap<int, RunHeaderItem *> *dataSource = nullptr;
};

#endif // RUNDESCRIPTIONLISTVIEWCONTROLLER_H
