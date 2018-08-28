#ifndef OPENTESTCASECONTROLLER_H
#define OPENTESTCASECONTROLLER_H

#include "Common/DBManager/dbmanager.h"
#include "GUI/TestCase/Open/Model/open-testcase-model.h"

#include <QAbstractListModel>
#include <QList>

class TestCaseListController : public QAbstractListModel {

    Q_OBJECT

public:
    TestCaseListController(QObject* parent = 0);
    void setDataSource(OpenTestCaseModel *model);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void beginResetModel();
    void endResetModel();

private:
    OpenTestCaseModel *dataSource = NULL;
};

#endif // OPENTESTCASECONTROLLER_H
