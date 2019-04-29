#include "tagmanager.h"

#include <QDir>
#include <QLibrary>
#include <QApplication>

//-- Variables -------------------------------------
QHash<QString, ITagAdapter *> TagManager::tagAdapterCollection;
QList<TagCollection *> TagManager::tagCollections;

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
                    reinterpret_cast<GetTagAdaptersFunction>(tagLib.resolve(GetTagAdaptersFunctionName));
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
    //Now there is not config, so simply scan tags folder
    QDir tagsDir(QDir::cleanPath(qApp->applicationDirPath() + "/tags"));
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
    foreach(ITagAdapter *tagAdapter, TagManager::tagAdapterCollection) { delete tagAdapter; }
    foreach(TagCollection *tagCollection, TagManager::tagCollections) { delete tagCollection; }
}

//-- Tag Adapter methods ---------------------------

QStringList TagManager::GetTagAdapterIDList()
{
    return TagManager::tagAdapterCollection.keys();
}

ITagAdapter *TagManager::GetTagAdapter(QString id)
{
    return TagManager::tagAdapterCollection.value(id);
}

//-- Tag collection methods ------------------------

void TagManager::LoadTagCollection(QString fullFileName)
{
    if(QFileInfo(fullFileName).exists())
    {
        TagCollection *tagCollection = DBManager::LoadTagCollection(fullFileName);
        if(tagCollection != nullptr) { tagCollections.append(tagCollection); }
    }
}

void TagManager::SaveTagCollection(TagCollection *tagCollection)
{
    DBManager::SaveTagCollection(tagCollection);
}

int TagManager::GetTagCollectionCount()
{
    return tagCollections.count();
}

TagCollection *TagManager::GetTagCollection(int ind)
{
    return tagCollections.at(ind);
}

TagCollection *TagManager::NewTagCollection(QString fullFileName, QString description)
{
    TagCollection *tagCollection = new TagCollection();

    tagCollection->name = QFileInfo(fullFileName).baseName();
    tagCollection->owner = tagCollection;
    tagCollection->fullFileName = fullFileName;
    tagCollection->description = description;

    tagCollections.append(tagCollection);

    DBManager::SaveTagCollection(tagCollection);

    return tagCollection;
}


void TagManager::DeleteTagCollection(TagCollection *collection, bool isPermanently)
{
    tagCollections.removeOne(collection);
    if(isPermanently) { DBManager::DeleteTagCollection(collection->fullFileName); }
    delete collection;
}

TagFolder *TagManager::NewTagFolder(TagFolder *parent, QString name, QString description)
{
    TagFolder *tagFolder = new TagFolder();

    tagFolder->name = name;
    tagFolder->owner = parent->owner;
    tagFolder->parent = parent;
    tagFolder->description = description;
    parent->folders.append(tagFolder);

    DBManager::SaveTagCollection(tagFolder->owner);

    return tagFolder;
}

void TagManager::DeleteTagFolder(TagFolder *folder)
{
    folder->parent->folders.removeOne(folder);
    DBManager::SaveTagCollection(folder->owner);
    delete folder;
}

Tag *TagManager::NewTag(TagFolder *parent, QString tagType, QString tagName, QString tagData)
{
    Tag *tag = new Tag();
    tag->owner = parent->owner;
    tag->parent = parent;
    tag->name = tagName;
    tag->type = tagType;
    tag->data = tagData;

    parent->tags.append(tag);

    DBManager::SaveTagCollection(tag->owner);

    return tag;
}

void TagManager::DeleteTag(Tag *tag)
{
    tag->parent->tags.removeOne(tag);
    DBManager::SaveTagCollection(tag->owner);
    delete tag;
}

QString TagManager::GetTagLink(Tag *tag)
{
    QString link = "";
    TagFolder *parent = tag->parent;

    while(parent->parent != nullptr) {
        link = parent->name + "/" + link;
        parent = parent->parent;
    }

    //Have to use "tag" because QUrl parse incorrectly a long schema name
    //Have to use ":///" instead "://" because QUrl convert upper case in host name to lower case
    //So all important info will be in the path property of QUrl
    return "tag:///" + parent->name + "/" + link + tag->name;
}

Tag *TagManager::getTagByLink(QString tagLink)
{
    int p = tagLink.indexOf('/', 7);
    if(p < 0) { return nullptr; }

    QString collectionName = tagLink.mid(7, p - 7);
    QString tail = tagLink.mid(p + 1);

    TagCollection *tagCollection = nullptr;
    for(int i = 0; i < tagCollections.length(); i++)
    {
        if(tagCollections.at(i)->name.compare(collectionName, Qt::CaseInsensitive) == 0)
        {
            tagCollection = tagCollections.at(i);
        }
    }

    if(tagCollection == nullptr) { return nullptr; }

    QString folderName = "";
    TagFolder *parentTagFolder = tagCollection;
    while(tail.contains('/') && parentTagFolder != nullptr)
    {
        p = tail.indexOf('/');
        folderName = tail.left(p);
        tail = tail.mid(p + 1);
        TagFolder *tagFolder = nullptr;
        for(int i = 0; i < parentTagFolder->folders.length(); i++) {
            if(parentTagFolder->folders.at(i)->name.compare(folderName, Qt::CaseInsensitive) == 0) {
                tagFolder = parentTagFolder->folders.at(i);
            }
        }
        parentTagFolder = tagFolder;
    }

    if(parentTagFolder == nullptr) { return nullptr; }

    QString tagName = tail;
    Tag *tag = nullptr;
    for(int i = 0; i < parentTagFolder->tags.length(); i++) {
        if(parentTagFolder->tags.at(i)->name.compare(tagName, Qt::CaseInsensitive) == 0) {
            tag = parentTagFolder->tags.at(i);
        }
    }

    return tag;
}
