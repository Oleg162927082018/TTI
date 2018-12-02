#ifndef OPENTESTCASECONTROLLER_H
#define OPENTESTCASECONTROLLER_H

#include "GUI/TestCase/Open/Model/open-testcase-item.h"

#include <QAbstractListModel>
#include <QList>

class OpenTestCaseListController : public QAbstractListModel {

    Q_OBJECT

public:
    QList<OpenTestCaseItem *> *dataSource = nullptr;

    OpenTestCaseListController(QObject* parent = nullptr);

    void beginResetModel();
    void endResetModel();

    // QAbstractItemModel interface
private:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // OPENTESTCASECONTROLLER_H
