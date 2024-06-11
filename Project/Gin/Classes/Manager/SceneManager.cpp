#include "SceneManager.h"
#include "Scenes/Gin/GPGinScene.h"
#include "Scenes/SplashScene.h"
#include "Scenes/LobbyScene.h"

USING_NS_CC;
using namespace std;

static SceneManager* _instance = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new SceneManager();
    return _instance;
}

void SceneManager::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}

SceneData SceneManager::getScene(SceneID sceneId, Value data /*= Value()*/)
{
    Scene* newScene = createNewScene(sceneId, data);
    if (newScene != nullptr)
        return SceneData(sceneId, newScene);
    return SceneData();
}

Scene* SceneManager::createNewScene(SceneID sceneId, Value data /*= Value()*/)
{
    switch (sceneId)
    {
        case SceneID::UNKNOWN:
            return nullptr;
        case SceneID::COMMON_SCENE:
            return SplashScene::createScene();
        case SceneID::LOBBY_SCENE:
            return LobbyScene::createScene();
        case SceneID::INGAME_GIN_SCENE:
        {
            CityType  cityType         = (CityType) data.asIntKeyMap()[DATA_CITY_TYPE_INDEX].asInt();
            TableType tableType        = (TableType) data.asIntKeyMap()[DATA_TABLE_TYPE_INDEX].asInt();
            double    betMoney         = data.asIntKeyMap()[DATA_BET_MONEY_INDEX].asDouble();
            int       targetScore      = data.asIntKeyMap()[DATA_TARGET_SCORE_INDEX].asInt();
            float     maxMultiplyValue = data.asIntKeyMap()[DATA_MAX_MULTIPLY_VALUE_INDEX].asFloat();
            return GPGinScene::createScene(cityType, tableType, betMoney, targetScore, maxMultiplyValue);
        }
        default:
            break;
    }
    return nullptr;
}

void SceneManager::openScene(SceneID sceneId, Value data /*= cocos2d::Value()*/)
{
    SceneData sceneData = getScene(sceneId, data);
    if (sceneData._sceneID != SceneID::UNKNOWN)
    {
        _currentSceneID = sceneData._sceneID;
        Scene* scene = Director::getInstance()->getRunningScene();
        if (scene != nullptr)
            Director::getInstance()->replaceScene(TransitionFade::create(0.4f, sceneData._scene));
        else
            Director::getInstance()->runWithScene(sceneData._scene);
    }
}

SceneID SceneManager::getCurrentSceneID()
{
    return _currentSceneID;
}

