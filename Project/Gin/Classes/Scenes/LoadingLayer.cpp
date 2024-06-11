#include "LoadingLayer.h"
#include "Define/GameDefine.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

#define TAG_ACTION_AUTO_HIDE 100
#define TAG_ACTION_AUTO_SHOW 101
#define TAG_ACTION_AUTO_SHOW_CALLBACK 102

#define AUTO_HIDE_DEFAULT_TIME 10.0f

static LoadingLayer* _instance = nullptr;

LoadingLayer::LoadingLayer()
{
    _show     = false;
    _autoHide = true;
}

LoadingLayer::~LoadingLayer()
{
}

LoadingLayer* LoadingLayer::createInstance()
{
    if (_instance == nullptr)
    {
        _instance = LoadingLayer::create();
        _instance->retain();
    }
    return _instance;
}

LoadingLayer* LoadingLayer::getInstance()
{
    return _instance;
}

void LoadingLayer::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParent();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

bool LoadingLayer::init()
{
    if (!Layer::init())
        return false;

    Size screenSize = Director::getInstance()->getWinSize();
    Node* node  = CSLoader::createNode("ui/Common/LoadingLayer.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));
    addChild(_layoutRoot, kMiddleground);
    _circle = (Sprite*) (_layoutRoot->getChildByName("sprite"));
    Helper::doLayout(this);
    this->_delayTime = SHOW_LOADING_DELAYTIME;
    setVisible(false);
    setKeypadEnabled(true);
    return true;
}

void LoadingLayer::attach(cocos2d::Node* node)
{
    removeFromParent();
    node->addChild(this, kAboveForeground);
}

void LoadingLayer::hide()
{
    if (getParent() != nullptr)
    {
        _show = false;
        stopActionByTag(TAG_ACTION_AUTO_HIDE);
        stopActionByTag(TAG_ACTION_AUTO_SHOW);
        stopActionByTag(TAG_ACTION_AUTO_SHOW_CALLBACK);
        removeFromParent();
        setVisible(false);
    }
}

void LoadingLayer::show()
{
    _show = true;
    stopAllActions();

    CallFunc* show = CallFunc::create([=] {
        if (_show == true)
        {
            this->setDelayTime(SHOW_LOADING_DELAYTIME);
            setVisible(true);
            if (_autoHide)
                runCountTimeAction();
            _circle->stopAllActions();
            _circle->runAction(RepeatForever::create(RotateBy::create(0.75f, 180)));
        }
    });
    if (this->_delayTime > 0)
    {
        Action* action = Sequence::create(DelayTime::create(this->_delayTime), show, nullptr);
        action->setTag(TAG_ACTION_AUTO_SHOW);
        runAction(action);
    }
    else
    {
        Action* action = Sequence::create(show, nullptr);
        action->setTag(TAG_ACTION_AUTO_SHOW);
        runAction(action);
    }
}

void LoadingLayer::show(std::string text)
{
    show();
}

void LoadingLayer::showAdsLoading(float delay, const std::function<void()>& callback)
{
    _callback = callback;
    show();
    if (_callback != nullptr)
    {
        stopActionByTag(TAG_ACTION_AUTO_SHOW_CALLBACK);
        CallFunc* cb     = CallFunc::create([=] {
            if (_callback != nullptr)
                _callback();
        });
        Action  * action = Sequence::create(DelayTime::create(delay), cb, DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(LoadingLayer::hide, this)), nullptr);
        action->setTag(TAG_ACTION_AUTO_SHOW_CALLBACK);
        runAction(action);
    }
}

void LoadingLayer::runCountTimeAction()
{
    stopActionByTag(TAG_ACTION_AUTO_HIDE);
    Action* action = Sequence::create(DelayTime::create(AUTO_HIDE_DEFAULT_TIME), CallFunc::create(CC_CALLBACK_0(LoadingLayer::hide, this)), nullptr);
    action->setTag(TAG_ACTION_AUTO_HIDE);
    runAction(action);
}

void LoadingLayer::setAutoHide(bool autoHide)
{
    _autoHide = autoHide;
}

void LoadingLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}