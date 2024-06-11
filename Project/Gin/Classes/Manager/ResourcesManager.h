#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__

#include "cocos2d.h"
#include "Manager/SoundManager.h"
#include "ui/CocosGUI.h"
#include "Manager/SceneManager.h"
#include <functional>

class ResourcesDefine;

#define LIST_RESOURCES_TYPE cocos2d::Vector<ResourcesDefine*>*

typedef std::function<void()> ResourceLoadDoneCallback;

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Resources Define
//////////////////////////////////////////////////////////////////////////////////////
class ResourcesDefine : public cocos2d::Ref
{
public:
    enum class ResourcesType
    {
        Image,
        Plist,
        Sound
    };

    enum class LoadingType
    {
        Synchronous,
        Asynchronous
    };

    ResourcesDefine(const std::string& name, LoadingType loadingType);

    void load();
    void unload();
    std::string getTextureNameFromPlist();

    std::string   _name;
    ResourcesType _resourcesType;
    LoadingType   _loadingType;
};


//////////////////////////////////////////////////////////////////////////////////////
/////////////////////   Resources Manager
//////////////////////////////////////////////////////////////////////////////////////
class ResourcesManager
{
private:
    int                                _numResourcesLoading;
    std::map<int, LIST_RESOURCES_TYPE> _listSceneResources;

    LIST_RESOURCES_TYPE _listUniqueResrouces;
    LIST_RESOURCES_TYPE _listLoadResources;
    LIST_RESOURCES_TYPE _listUnloadResources;

    ResourceLoadDoneCallback _syncDoneCallback;
    ResourceLoadDoneCallback _asyncDoneCallback;

    ~ResourcesManager();
    ResourcesManager();
    void defineSceneResources();
    void addResourceToScene(SceneID sceneID, const std::string& resourceName, ResourcesDefine::LoadingType loadingType = ResourcesDefine::LoadingType::Synchronous);
    LIST_RESOURCES_TYPE getUniqueResourcesFromList(LIST_RESOURCES_TYPE listSources, LIST_RESOURCES_TYPE listDest);
    void loadSyncResources(const ResourceLoadDoneCallback& callback);
    void loadAsyncResources(const ResourceLoadDoneCallback& callback);
    void loadAsyncTexture(const std::string& name);
    void onLoadTexture2D(cocos2d::Texture2D* texture2D);
public:
    void loadResources(const ResourceLoadDoneCallback& syncDoneCallback = nullptr, const ResourceLoadDoneCallback& asyncDoneCallback = nullptr);
    void unloadUnusedResources();

    static ResourcesManager* getInstance();
    static void destroyInstance();
};

#endif // !__RESOURCES_MANAGER_H__

