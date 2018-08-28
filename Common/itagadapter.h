#ifndef ITAGADAPTER_H
#define ITAGADAPTER_H

#include <QtXml/QDomDocument>
#include <QString>
#include <QWidget>
#include <QMap>
#include <QtCore/qglobal.h>

class ITagWidget : public QWidget
{
public:
    explicit ITagWidget(QWidget *parent = 0) : QWidget(parent) { }
    virtual ~ITagWidget() { }

    virtual void SetData(QDomDocument *config, QDomDocument *params) = 0;
    virtual void GetData(QDomDocument *config, QDomDocument *params) = 0;
};

class ITagAdapter
{
public:
    virtual ~ITagAdapter() { }

    virtual const QString ID() = 0;
    virtual const QString Name() = 0;
    virtual const QString Description() = 0;

    virtual ITagWidget *GetTagCreateWidget(QWidget *parent) = 0;
    virtual ITagWidget *GetTagEditWidget(QWidget *parent) = 0;
    virtual ITagWidget *GetTagViewWidget(QWidget *parent) = 0;
};

extern "C" Q_DECL_EXPORT QList<ITagAdapter *> GetTagAdapters(void);

//--------------------------------------------------------------

#define GetTagAdaptersFunctionName "GetTagAdapters"

typedef QList<ITagAdapter *> (*GetTagAdaptersFunction)();

#endif // ITAGADAPTER_H
