#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include "../Common/itagadapter.h"

#include "../Common/DBManager/dbmanager.h"

class TagManager
{

public:
    static void Init();
    static void FreeResources();
private:
    static void FreeTagFolder(TagFolder *folder);

//Adapter methods
public:
    static QStringList GetTagAdapterIDList();
    static ITagAdapter *GetTagAdapter(QString id);

//Collection methods
public:
    static int GetTagCollectionCount();
    static TagCollection *GetTagCollection(int ind);

    static TagCollection *NewTagCollection(QString fullFileName, QString description);
    static void DeleteTagCollection(TagCollection *collection, bool isPermanently);

private:
    static void LoadTagCollection(QString fullFileName);
    static void SaveTagCollection(TagCollection *tagCollection);

//Folder methods
public:
    static TagFolder *NewTagFolder(TagFolder *parent, QString name, QString description);
    /*static void EditTagFolder(QString fullFileNameCollection, QString oldFolderName, QString newFolderName);*/
    static void DeleteTagFolder(TagFolder *folder);

//Tag methods
public:
    static Tag *NewTag(TagFolder *parent, QString tagType, QString tagName, QString tagData);
    /*static void EditTag(QString fullFileNameCollection, QString tagTemplateID, QString tagID, QString newTagName, QDomDocument newTagData);*/
    static void DeleteTag(Tag *tag);

    static Tag *getTagByLink(QString tagLink);
    static QString GetTagLink(Tag *tag);

private:
    static QHash<QString, ITagAdapter *> tagAdapterCollection;
    //QString - ID of tag adapter
    //ITagAdapter - object created by dll module
    static QList<TagCollection *> tagCollections;
    //QString - name of the collection (name of the file)
    //TagCollection * - pointer to the tree of folders and tags where
    //First level element is a tag collection.
    //End level element is a tag.
    //Other levels is folder elements.
};

#endif // TAGMANAGER_H
