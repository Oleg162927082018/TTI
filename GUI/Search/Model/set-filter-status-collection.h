#ifndef SETFILTERSTATUSTESTCASEITEM_H
#define SETFILTERSTATUSTESTCASEITEM_H

#include <QColor>
#include <QString>

struct SetFilterStatusItem
{
    bool checked;
    QString label;
    QColor color;
    QString description;
};

struct SetFilterStatusCollection
{
    QString name;

    QList<SetFilterStatusItem *> statuses;
};

#endif // SETFILTERSTATUSTESTCASEITEM_H
