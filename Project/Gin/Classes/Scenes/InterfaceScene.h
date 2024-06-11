#ifndef __INTERFACE_SCENE_H__
#define __INTERFACE_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "Manager/ResourcesManager.h"

class InterfaceScene : public cocos2d::Layer
{
private:
protected:
    cocos2d::Size _screenSize;
public:
    virtual ~InterfaceScene();
    InterfaceScene();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onLoadResourcesSyncDone() = 0;
    virtual void onLoadResourcesAsyncDone() = 0;
    virtual SceneID getSceneID() = 0;
    void removePopup(int boardId);
};

#endif // !__INTERFACE_SCENE_H__

