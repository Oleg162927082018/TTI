#ifndef RUNDESCRIPTIONLISTVIEWCONTROLLER_H
#define RUNDESCRIPTIONLISTVIEWCONTROLLER_H

#include "Common/DBManager/run-description.h"

#include <QList>
#include <QAbstractListModel>

class RunDescriptionTableWidgetController : public QAbstractTableModel
{

    Q_OBJECT

public:
    explicit RunDescriptionTableWidgetController(QObject* parent = 0);
    void setDataSource(QMap<int, RunDescription *>  *runDescriptionList);
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
    QMap<int, RunDescription *> *dataSource = NULL;
};

#endif // RUNDESCRIPTIONLISTVIEWCONTROLLER_H
