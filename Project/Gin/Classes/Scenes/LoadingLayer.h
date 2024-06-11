#ifndef __LOADING_LAYER_H__
#define __LOADING_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define SHOW_LOADING_DELAYTIME 0.0f

class LoadingLayer : public cocos2d::Layer
{
private:
    cocos2d::ui::Layout* _layoutRoot;
    cocos2d::Sprite    * _circle;

    bool                  _show;
    bool                  _autoHide;
    std::function<void()> _callback;
CC_SYNTHESIZE(float, _delayTime, DelayTime);

    void runCountTimeAction();
protected:
public:
    LoadingLayer();
    ~LoadingLayer();

    static LoadingLayer* createInstance();
    static LoadingLayer* getInstance();
    static void destroyInstance();

    virtual bool init() override;

    void attach(cocos2d::Node* node);
    void hide();
    void show();
    void show(std::string text);
    void showAdsLoading(float delay, const std::function<void()>& callback);
    void setAutoHide(bool autoHide);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    CREATE_FUNC(LoadingLayer);
};

#endif //__LOADING_LAYER_H__