#ifndef CREATETESTCASEMODEL_H
#define CREATETESTCASEMODEL_H

#include "Common/itestcasetemplate.h"
#include "Common/DBManager/dbmanager.h"

#include <QAbstractListModel>

class TestCaseTemplateListAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    TestCaseTemplateListAdapter(QObject* parent = nullptr);
    void Init(QStringList *d);


public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void beginResetModel();
    void endResetModel();

private:
    QStringList *dataSource;
};

#endif // CREATETESTCASEMODEL_H
