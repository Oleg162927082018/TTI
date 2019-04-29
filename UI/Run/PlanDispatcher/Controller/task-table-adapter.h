#ifndef TASKTABLEADAPTER_H
#define TASKTABLEADAPTER_H

#include <QAbstractTableModel>

class TaskTableAdapter : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TaskTableAdapter(QObject *parent = nullptr);

    void beginResetModel();
    void endResetModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // TASKTABLEADAPTER_H
