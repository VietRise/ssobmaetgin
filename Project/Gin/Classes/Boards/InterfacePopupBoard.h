#ifndef __INTERFACE_POPUP_BOARD_H__
#define __INTERFACE_POPUP_BOARD_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "Manager/SoundManager.h"
#include "Manager/TextManager.h"
#include "Manager/NotificationManager.h"

class InterfacePopupBoard : public cocos2d::LayerColor
{
public:
    enum class State
    {
        OPENED,
        CLOSED
    };
private:
    virtual void changeLanguage(Language language);
protected:
    cocos2d::Size _screenSize;

    cocos2d::Color4B _color;
    GLfloat          _width;
    GLfloat          _height;

    cocos2d::ui::Layout* _layoutRoot;
    bool _applyBackKey;
    bool _applyTouchOutClose;

    virtual void onNotify(void* sender, int id, const cocos2d::Value& data);
public:
CC_SYNTHESIZE(State, _state, State);

    ~InterfacePopupBoard();
    InterfacePopupBoard();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    cocos2d::Color4B getColor();
    void setColor(cocos2d::Color4B color);

    virtual void open();
    virtual void close(bool forceClose = false);

    void attach(Node* node, int zOrder);

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;

    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
};

#endif // !__INTERFACE_POPUP_BOARD_H__