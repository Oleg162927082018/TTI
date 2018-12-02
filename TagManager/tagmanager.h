#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include "../Common/itagadapter.h"

#include "../Common/DBManager/dbmanager.h"

class TagManager
{
public:
    static void Init();
    static void FreeResources();
    static void FreeTagItem(TagItem *ti);

    //Adapter methods
    static QStringList GetTagAdapterIDList();
    static ITagAdapter *GetTagAdapter(QString id);

    //Collection methods
    static int GetTagCollectionCount();
    static int GetTagCollectionInd(QString name);
    static TagItem *GetTagCollection(int ind);

    static void CreateTagCollection(QString fullFileName);
    static void LoadTagCollection(QString fullFileName);
    static void SaveTagCollection(TagItem *tagCollection);
    static void RemoveTagCollection(QString fullFileName);
    static void DeleteTagCollection(QString fullFileName);

    //Folder methods
    static TagItem *CreateTagFolder(TagItem *parent, QString name);
    /*static void EditTagFolder(QString fullFileNameCollection, QString oldFolderName, QString newFolderName);
    static void DeleteTagFolder(QString fullFileNameCollection, QString folderName);*/

    //Tag methods
    static TagItem *AddTag(TagItem *parent, QString tagType, QString tagName, QString tagData);
    /*static void EditTag(QString fullFileNameCollection, QString tagTemplateID, QString tagID, QString newTagName, QDomDocument newTagData);
    static void DeleteTag(QString fullFileNameCollection, QString tagID);*/
    static TagItem *FindTagByPath(QString tagPath);

private:
    static QHash<QString, ITagAdapter *> tagAdapterCollection;
    //QString - ID of tag adapter
    //ITagAdapter - object created by dll module
    static QHash<QString, TagItem *> tagCollections;
    //QString - name of the collection (name of the file)
    //TagFolder * - pointer to the tree of folders and tags where
    //First level element is a tag collection.
    //End level element is a tag.
    //Other levels is folder elements.
};

#endif // TAGMANAGER_H
