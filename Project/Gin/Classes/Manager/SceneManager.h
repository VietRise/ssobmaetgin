#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
#include <vector>

#define DATA_CITY_TYPE_INDEX 0
#define DATA_TABLE_TYPE_INDEX 1
#define DATA_BET_MONEY_INDEX 2
#define DATA_TARGET_SCORE_INDEX 4
#define DATA_MAX_MULTIPLY_VALUE_INDEX 5

enum class SceneID
{
    UNKNOWN          = 0,
    INGAME_GIN_SCENE = 1,
    COMMON_SCENE     = 2,
    LOBBY_SCENE      = 4,
    INGAME_TL_SCENE  = 6
};

struct SceneData
{
    SceneData()
    {
        _sceneID = SceneID::UNKNOWN;
        _scene   = nullptr;
    }
    SceneData(SceneID sceneID, cocos2d::Scene* scene)
    {
        _sceneID = sceneID;
        _scene   = scene;
    }
    ~SceneData()
    {
    }

    SceneID _sceneID;
    cocos2d::Scene* _scene;
};

class SceneManager
{
private:
    SceneID _currentSceneID;
    cocos2d::Scene* createNewScene(SceneID sceneId, cocos2d::Value data = cocos2d::Value());
protected:
public:
    SceneManager();
    ~SceneManager();

    static SceneManager* getInstance();
    static void destroyInstance();

    SceneData getScene(SceneID sceneId, cocos2d::Value data = cocos2d::Value());
    void openScene(SceneID sceneId, cocos2d::Value data = cocos2d::Value());
    SceneID getCurrentSceneID();
};


#endif
