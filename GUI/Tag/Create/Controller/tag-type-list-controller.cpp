#include "tag-type-list-controller.h"

#include "Common/itagadapter.h"
#include "TagManager/tagmanager.h"

TagTypeListController::TagTypeListController()
{

}

void TagTypeListController::Init(QStringList *d)
{
    dataSource = d;
}

int TagTypeListController::rowCount(const QModelIndex &parent) const
{
    return dataSource->count();
}

QVariant TagTypeListController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= 10) return QVariant();

    if (role == Qt::DisplayRole) {
        QString id = dataSource->at(index.row());
        ITagAdapter *tagAdapter = TagManager::GetTagAdapter(id);
        return QVariant(tagAdapter->Name());
    } else {
        return QVariant();
    }
}

void TagTypeListController::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}

void TagTypeListController::endResetModel()
{
    QAbstractListModel::endResetModel();
}
