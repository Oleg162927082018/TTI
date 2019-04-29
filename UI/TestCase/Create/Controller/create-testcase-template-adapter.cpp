#include "create-testcase-template-adapter.h"

#include "Common/DllManager/dllmanager.h"

TestCaseTemplateListAdapter::TestCaseTemplateListAdapter(QObject *parent)
{

}

void TestCaseTemplateListAdapter::Init(QStringList *d)
{
    dataSource = d;
}

int TestCaseTemplateListAdapter::rowCount(const QModelIndex &parent) const
{
    return dataSource->count();
}

QVariant TestCaseTemplateListAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= 10) return QVariant();

    if (role == Qt::DisplayRole) {
        QString id = dataSource->at(index.row());
        ITestCaseAdapter *tcTemplate = DLLManager::GetTestCaseAdapter(id);
        return QVariant(tcTemplate->Name());
    } else {
        return QVariant();
    }
}
