#include "ResourcesManager.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/SceneManager.h"
#include "Define/ResourcesDefine.h"

USING_NS_CC;
using namespace std;
using namespace ui;

static ResourcesManager* _instance = nullptr;

/////////////////////////////////////////// Implement ResourcesDefine /////////////////////////////////////////////////////
ResourcesDefine::ResourcesDefine(const std::string& name, LoadingType loadingType) : _name(name), _loadingType(loadingType)
{
    std::string extensionFile = FileUtils::getInstance()->getFileExtension(_name);
    if (extensionFile.compare(".plist") == 0)
        _resourcesType = ResourcesType::Plist;
    else if (extensionFile.compare(".png") == 0 || extensionFile.compare(".jpg") == 0)
        _resourcesType = ResourcesType::Image;
    else if (extensionFile.compare(".wav") == 0 || extensionFile.compare(".mp3") == 0)
        _resourcesType = ResourcesType::Sound;
}

void ResourcesDefine::load()
{
    switch (_resourcesType)
    {
        case ResourcesType::Image:
            Director::getInstance()->getTextureCache()->addImage(_name);
            break;
        case ResourcesType::Plist:
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_name);
            break;
        case ResourcesType::Sound:
            SoundManager::getInstance()->preloadEffect(_name.c_str());
            break;
        default:
            break;
    }
}

void ResourcesDefine::unload()
{
    switch (_resourcesType)
    {
        case ResourcesType::Image:
            Director::getInstance()->getTextureCache()->removeTextureForKey(_name);
            break;
        case ResourcesType::Plist:
            Director::getInstance()->getTextureCache()->removeTextureForKey(getTextureNameFromPlist());
            SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(_name);
            break;
        case ResourcesType::Sound:
            SoundManager::getInstance()->unloadEffect(_name.c_str());
            break;
        default:
            break;
    }
}

string ResourcesDefine::getTextureNameFromPlist()
{
    if (_resourcesType == ResourcesType::Plist)
    {
        string textureName = _name.substr(0, _name.find("."));
        textureName.append(".png");
        return textureName;
    }
    return "";
}

/////////////////////////////////////////// Implement ResourcesManager /////////////////////////////////////////////////////
ResourcesManager::~ResourcesManager()
{
    // Delete unique resource;
    CC_SAFE_DELETE(_listUniqueResrouces);
    // Delete map resource
    for (auto& sceneResources : _listSceneResources)
    {
        LIST_RESOURCES_TYPE                     resources = sceneResources.second;
        for (Vector<ResourcesDefine*>::iterator i         = resources->begin(); i != resources->end(); i++)
            CC_SAFE_RELEASE(*i);
        CC_SAFE_DELETE(resources);
    }
}

ResourcesManager::ResourcesManager()
{
    _numResourcesLoading = 0;
    _listUniqueResrouces = new Vector<ResourcesDefine*>();
    _listLoadResources   = nullptr;
    _listUnloadResources = nullptr;
    _syncDoneCallback    = nullptr;
    _asyncDoneCallback   = nullptr;
    defineSceneResources();
}

void ResourcesManager::defineSceneResources()
{
	this->addResourceToScene(SceneID::COMMON_SCENE, Resources::Plist::SpriteSheet_UI_Common, ResourcesDefine::LoadingType::Asynchronous);
	this->addResourceToScene(SceneID::COMMON_SCENE, Resources::Plist::SpriteSheetLobbyScene, ResourcesDefine::LoadingType::Asynchronous);
    this->addResourceToScene(SceneID::COMMON_SCENE, Resources::Plist::ui_GameplayScene, ResourcesDefine::LoadingType::Asynchronous);
    this->addResourceToScene(SceneID::COMMON_SCENE, Resources::Plist::SpriteSheetCard_GameplayScene, ResourcesDefine::LoadingType::Asynchronous);
    this->addResourceToScene(SceneID::COMMON_SCENE, Resources::Plist::SpriteSheet_GameIcon, ResourcesDefine::LoadingType::Asynchronous);
    // -- Sound
    const map<SoundEvent, string>& maps = SoundManager::getInstance()->getMaps();
    for (auto                    & kv : maps)
        this->addResourceToScene(SceneID::COMMON_SCENE, kv.second);
}

void ResourcesManager::addResourceToScene(SceneID sceneID, const std::string& resourceName, ResourcesDefine::LoadingType loadingType)
{
    std::map<int, LIST_RESOURCES_TYPE>::iterator itResources = _listSceneResources.find((int) sceneID);
    LIST_RESOURCES_TYPE listResources                        = (itResources != _listSceneResources.end()) ? itResources->second : nullptr;
    if (listResources == nullptr)
    {
        listResources = new Vector<ResourcesDefine*>();
        _listSceneResources.insert(std::pair<int, LIST_RESOURCES_TYPE>((int) sceneID, listResources));
    }
    listResources->pushBack(new ResourcesDefine(resourceName, loadingType));
}

LIST_RESOURCES_TYPE ResourcesManager::getUniqueResourcesFromList(LIST_RESOURCES_TYPE listSources, LIST_RESOURCES_TYPE listDest)
{
    _listUniqueResrouces->clear();
    for (int i = 0; i < listSources->size(); i++)
    {
        bool isDuplicate = false;
        auto sResouce    = (ResourcesDefine*) listSources->at(i);

        for (int j = 0; j < listDest->size(); j++)
        {
            auto dResouce = (ResourcesDefine*) listDest->at(j);
            if (sResouce->_name.compare(dResouce->_name) == 0)
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
            _listUniqueResrouces->pushBack(sResouce);
    }
    return _listUniqueResrouces;
}

void ResourcesManager::loadSyncResources(const ResourceLoadDoneCallback& callback)
{
    for (Vector<ResourcesDefine*>::iterator i = _listLoadResources->begin(); i != _listLoadResources->end();)
    {
        auto resource = (*i);
        if (resource->_loadingType == ResourcesDefine::LoadingType::Synchronous)
        {
            CCLOG("--- ResourcesManager: load sync resources: %s", resource->_name.c_str());
            resource->load();
            i = _listLoadResources->erase(i);
        }
        else
        {
            i++;
        }
    }
    if (callback != nullptr)
        callback();
}

void ResourcesManager::loadAsyncResources(const ResourceLoadDoneCallback& callback)
{
    _numResourcesLoading = _listLoadResources->size();
    if (_numResourcesLoading == 0)
    {
        if (callback != nullptr)
            callback();
        return;
    }
    // Load asynchronous
    for (Vector<ResourcesDefine*>::iterator i = _listLoadResources->begin(); i != _listLoadResources->end(); i++)
    {
        auto resource = (*i);
        if (resource->_loadingType == ResourcesDefine::LoadingType::Asynchronous)
        {
            CCLOG("--- ResourcesManager: load async resources: %s", resource->_name.c_str());
            if (resource->_resourcesType == ResourcesDefine::ResourcesType::Image)
                loadAsyncTexture(resource->_name);
            else if (resource->_resourcesType == ResourcesDefine::ResourcesType::Plist)
                loadAsyncTexture(resource->getTextureNameFromPlist());
        }
    }
}

void ResourcesManager::loadAsyncTexture(const std::string& name)
{
    Director::getInstance()->getTextureCache()->addImageAsync(name, CC_CALLBACK_1(ResourcesManager::onLoadTexture2D, this));
}

void ResourcesManager::onLoadTexture2D(cocos2d::Texture2D* texture2D)
{
    if (texture2D != nullptr)
    {
        // Get texture name from full path
        std::string filePath = Director::getInstance()->getTextureCache()->getTextureFilePath(texture2D);
        int         lastDash = filePath.find_last_of("/");
        std::string fileName = filePath.substr(lastDash + 1, filePath.length() - lastDash - 1);
        // Add sprite frames if needed
        for (int    i        = 0; i < _listLoadResources->size(); i++)
        {
            auto resource = (ResourcesDefine*) _listLoadResources->at(i);
            if (resource->_resourcesType != ResourcesDefine::ResourcesType::Plist)
                continue;
            std::string textureName = resource->getTextureNameFromPlist();
            if (textureName.find(fileName) != std::string::npos)
                SpriteFrameCache::getInstance()->addSpriteFramesWithFile(resource->_name, texture2D);
        }

    }
    _numResourcesLoading -= 1;
    if (_numResourcesLoading <= 0)
        if (_asyncDoneCallback != nullptr)
            _asyncDoneCallback();
}

void ResourcesManager::loadResources(const ResourceLoadDoneCallback& syncDoneCallback, const ResourceLoadDoneCallback& asyncDoneCallback)
{
    _syncDoneCallback  = syncDoneCallback;
    _asyncDoneCallback = asyncDoneCallback;
    int                                          currentSceneID     = (int) SceneManager::getInstance()->getCurrentSceneID();
    int                                          lastSceneID        = (int) SceneID::UNKNOWN;
    std::map<int, LIST_RESOURCES_TYPE>::iterator itCurrentResources = _listSceneResources.find(currentSceneID);
    std::map<int, LIST_RESOURCES_TYPE>::iterator itLastResources    = _listSceneResources.find(lastSceneID);
    LIST_RESOURCES_TYPE listCurrentResources                        = (itCurrentResources != _listSceneResources.end()) ? itCurrentResources->second : nullptr;
    LIST_RESOURCES_TYPE listLastResources                           = (itLastResources != _listSceneResources.end()) ? itLastResources->second : nullptr;
    // This scene doesn't load any resources. Should dispatch message immediately
    if (listCurrentResources == nullptr)
    {
        if (_syncDoneCallback != nullptr)
            _syncDoneCallback();
        if (_asyncDoneCallback != nullptr)
            _asyncDoneCallback();
        return;
    }
    // If last scene is none (first scene) then listLastResources must be null
    bool initListLastResources = false;
    if (listLastResources == nullptr)
    {
        initListLastResources = true;
        listLastResources     = new Vector<ResourcesDefine*>();
    }

    _listLoadResources = this->getUniqueResourcesFromList(listCurrentResources, listLastResources);
    if (initListLastResources)
        CC_SAFE_DELETE(listLastResources);
    loadSyncResources(_syncDoneCallback);
    loadAsyncResources(_asyncDoneCallback);
}

void ResourcesManager::unloadUnusedResources()
{
    int                                          currentSceneID     = (int) SceneManager::getInstance()->getCurrentSceneID();
    int                                          lastSceneID        = (int) SceneID::UNKNOWN;
    std::map<int, LIST_RESOURCES_TYPE>::iterator itCurrentResources = _listSceneResources.find(currentSceneID);
    std::map<int, LIST_RESOURCES_TYPE>::iterator itLastResources    = _listSceneResources.find(lastSceneID);
    LIST_RESOURCES_TYPE listCurrentResources                        = (itCurrentResources != _listSceneResources.end()) ? itCurrentResources->second : nullptr;
    LIST_RESOURCES_TYPE listLastResources                           = (itLastResources != _listSceneResources.end()) ? itLastResources->second : nullptr;

    if (listCurrentResources != nullptr && listLastResources != nullptr)
    {
        _listUnloadResources = this->getUniqueResourcesFromList(listLastResources, listCurrentResources);
        for (int i = 0; i < _listUnloadResources->size(); i++)
        {
            auto resources = (ResourcesDefine*) _listUnloadResources->at(i);
            resources->unload();
            CCLOG("--- ResourcesManager: unload resources: %s", resources->_name.c_str());
        }
    }
}

ResourcesManager* ResourcesManager::getInstance()
{
    if (!_instance)
        _instance = new ResourcesManager();
    return _instance;
}

void ResourcesManager::destroyInstance()
{
    if (_instance)
        CC_SAFE_DELETE(_instance);
}
