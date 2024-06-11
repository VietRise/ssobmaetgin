#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "InterfaceScene.h"
#include "Define/ServerClientMessageDefine.h"

class SplashScene : public InterfaceScene
{
private:

    bool   _loadResourceDone;
    double _timeLoading;

    void onLoadSave(const rapidjson::Document& data);
    void onUserData(const rapidjson::Document& data);
protected:
public:
    ~SplashScene();
    SplashScene();
    static cocos2d::Scene* createScene();
    static SplashScene* create();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onLoadResourcesSyncDone() override;
    virtual void onLoadResourcesAsyncDone() override;
    virtual SceneID getSceneID() override;

    void updateCheckStartup(float dt);
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);
};

#endif // !__SPLASH_SCENE_H__