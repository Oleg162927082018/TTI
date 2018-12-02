#ifndef OPENTESTCASERUNDESCRIPTIONSCONTROLLER_H
#define OPENTESTCASERUNDESCRIPTIONSCONTROLLER_H

#include "GUI/TestCase/Open/Model/open-testcase-runitem.h"

#include <QList>
#include <QAbstractListModel>

class OpenTestCaseRunDescriptionsController : public QAbstractTableModel {

    Q_OBJECT

public:
    QMap<int, OpenTestCaseRunItem *> *dataSource = nullptr;

    explicit OpenTestCaseRunDescriptionsController(QObject* parent = nullptr);

    void beginResetModel();
    void endResetModel();
    void setAllChecked();

// QAbstractItemModel interface
private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

};

#endif // OPENTESTCASERUNDESCRIPTIONSCONTROLLER_H
