#ifndef LOGADAPTER_H
#define LOGADAPTER_H

#include <QAbstractListModel>

class LogAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LogAdapter(QObject *parent = 0);

    void Init(QStringList *d);

    void beginResetModel();
    void endResetModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList *dataSource;

};

#endif // LOGADAPTER_H
