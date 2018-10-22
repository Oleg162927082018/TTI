#include "tagmanager.h"

#include <QDir>
#include <QLibrary>
#include <QApplication>

//-- Variables -------------------------------------
QHash<QString, ITagAdapter *> TagManager::tagAdapterCollection;
QHash<QString, TagItem *> TagManager::tagCollections;

//-- Methods ---------------------------------------

void TagManager::Init()
{
    //Looking for and load tag adapters from dll
    QDir tagAdapterDir(QDir::cleanPath(qApp->applicationDirPath() + "/lib/tag"));
    tagAdapterDir.setFilter(QDir::Files);
    tagAdapterDir.setNameFilters(QStringList("*.dll"));
    tagAdapterDir.setSorting(QDir::Name);

    QFileInfoList tagLibList = tagAdapterDir.entryInfoList();
    for (int i = 0; i < tagLibList.size(); ++i)
    {
        QFileInfo fileInfo = tagLibList.at(i);
        QLibrary tagLib(fileInfo.absoluteFilePath());
        if (tagLib.load())
        {
            GetTagAdaptersFunction GetTagList =
                    (GetTagAdaptersFunction)tagLib.resolve(GetTagAdaptersFunctionName);
            {
                QList<ITagAdapter *> taList = GetTagList();
                for(int j = 0; j < taList.size(); j++)
                {
                    ITagAdapter* ta = taList[j];
                    TagManager::tagAdapterCollection.insert(ta->ID(), ta);
                }
            }
        }
    }

    //Looking for and load tag collections
    //Now there is not config, so simply scan tag\data folder
    QDir tagsDir(QDir::cleanPath(qApp->applicationDirPath() + "/tag/data"));
    tagsDir.setFilter(QDir::Files);
    tagsDir.setNameFilters(QStringList("*.xml"));
    tagsDir.setSorting(QDir::Name);

    QFileInfoList tagsFileInfoList = tagsDir.entryInfoList();
    for (int i = 0; i < tagsFileInfoList.size(); ++i)
    {
        TagManager::LoadTagCollection(tagsFileInfoList.at(i).absoluteFilePath());
    }
}

void TagManager::FreeResources()
{
    foreach(ITagAdapter *ta, TagManager::tagAdapterCollection) { delete ta; }
    foreach(TagItem *ti, TagManager::tagCollections)
    {
        delete ti->collection;
        FreeTagItem(ti);
    }
}

void TagManager::FreeTagItem(TagItem *ti)
{
    if( ti->tag != NULL) { delete ti->tag; }
    if( ti->folder != NULL) { delete ti->folder; }

    foreach(TagItem *tsi, ti->subItems)
    {
        FreeTagItem(tsi);
    }

    delete ti;
}

QStringList TagManager::GetTagAdapterIDList()
{
    return TagManager::tagAdapterCollection.keys();
}

ITagAdapter *TagManager::GetTagAdapter(QString id)
{
    return TagManager::tagAdapterCollection.value(id);
}

int TagManager::GetTagCollectionCount()
{
    return tagCollections.count();
}

int TagManager::GetTagCollectionInd(QString name)
{
    return tagCollections.keys().indexOf(name);
}

TagItem *TagManager::GetTagCollection(int ind)
{
    QString key = tagCollections.keys().at(ind);
    return tagCollections.value(key);
}

void TagManager::CreateTagCollection(QString fullFileName)
{
    TagItem *tagCollection = new TagItem();
    tagCollection->collection = new TagCollection();

    tagCollection->name = QFileInfo(fullFileName).baseName();
    tagCollection->path = tagCollection->name;
    tagCollection->collection->fullFileName = fullFileName;

    tagCollections.insert(tagCollection->name, tagCollection);

    DBManager::SaveTagCollection(tagCollection);
}

void TagManager::LoadTagCollection(QString fullFileName)
{
    if(QFileInfo(fullFileName).exists())
    {
        TagItem *tagCollection = DBManager::GetTagCollection(fullFileName);
        if(tagCollection != NULL) { tagCollections.insert(tagCollection->name, tagCollection); }
    }
}

void TagManager::SaveTagCollection(TagItem *tagCollection)
{
    DBManager::SaveTagCollection(tagCollection);
}

void TagManager::RemoveTagCollection(QString fullFileName)
{
    tagCollections.remove(QFileInfo(fullFileName).baseName());
}

void TagManager::DeleteTagCollection(QString fullFileName)
{
    tagCollections.remove(QFileInfo(fullFileName).baseName());
    DBManager::DeleteTagCollection(fullFileName);
}

TagItem *TagManager::CreateTagFolder(TagItem *parent, QString name)
{
    TagItem *tagFolder = new TagItem();
    tagFolder->folder = new TagFolder();

    tagFolder->name = name;
    tagFolder->collection = parent->collection;
    tagFolder->expanded = false;
    tagFolder->parent = parent;
    tagFolder->path = parent->path + ":" + tagFolder->name;

    parent->subItems.append(tagFolder);

    TagItem *tagCollection = tagFolder;
    while(tagCollection->parent != NULL) { tagCollection = tagCollection->parent; }

    DBManager::SaveTagCollection(tagCollection);

    return tagFolder;
}

TagItem *TagManager::AddTag(TagItem *parent, QString tagType, QString tagName, QDomDocument &tagData)
{
    TagItem *tagItem = new TagItem();
    tagItem->parent = parent;
    tagItem->collection = parent->collection;
    tagItem->name = tagName;
    tagItem->path = parent->path + ":" + tagItem->name;

    tagItem->tag = new Tag();
    tagItem->tag->type = tagType;
    tagItem->tag->data = tagData;

    parent->subItems.append(tagItem);

    TagItem *tagCollection = parent;
    while(tagCollection->parent != NULL) { tagCollection = tagCollection->parent; }

    DBManager::SaveTagCollection(tagCollection);

    return tagItem;
}

TagItem *TagManager::FindTagByPath(QString tagPath)
{
    int p = tagPath.indexOf(':');
    if(p < 0) { return NULL; }

    QString name = tagPath.left(p);
    QString tail = tagPath.right(tagPath.length() - p - 1);

    TagItem *tagItem = NULL;
    for(int i = 0; i < tagCollections.keys().length(); i++)
    {
        if(tagCollections.keys().at(i).compare(name, Qt::CaseInsensitive) == 0)
        {
            tagItem = tagCollections.value(tagCollections.keys().at(i));
        }
    }
    if(tagItem == NULL) { return NULL; }

    while(p > 0)
    {
        p = tail.indexOf(':');
        if(p >= 0)
        {
            name = tail.left(p);
            tail = tail.right(tail.length() - p - 1);
        }
        else
        {
            name = tail;
            tail = "";
        }

        bool isFound = false;
        for(int i = 0; i < tagItem->subItems.length(); i++)
        {
            if(tagItem->subItems.at(i)->name.compare(name, Qt::CaseInsensitive) == 0)
            {
                tagItem = tagItem->subItems.at(i);
                isFound = true;
                break;
            }
        }

        if(!isFound) { return NULL; }
    }

    return tagItem;
}
