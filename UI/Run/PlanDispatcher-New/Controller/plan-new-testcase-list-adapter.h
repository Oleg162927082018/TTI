#ifndef PLANNEWTESTCASELISTADAPTER_H
#define PLANNEWTESTCASELISTADAPTER_H

#include <QAbstractListModel>

#include <Common/DBManager/test-case.h>

class PlanNewTestCaseListAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PlanNewTestCaseListAdapter(QObject *parent = nullptr);

    void Init(QStringList *d);

    void beginResetModel();
    void endResetModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList *dataSource;
};

#endif // PLANNEWTESTCASELISTADAPTER_H
