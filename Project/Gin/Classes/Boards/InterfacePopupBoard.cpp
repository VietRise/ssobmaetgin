#include "InterfacePopupBoard.h"
#include "Define/InGameDefine.h"

USING_NS_CC;
using namespace std;

InterfacePopupBoard::~InterfacePopupBoard()
{
    NotificationManager::getInstance()->unregisterNotification(this);
}

InterfacePopupBoard::InterfacePopupBoard()
{
    _screenSize         = Director::getInstance()->getWinSize();
    _color              = Color4B(0, 0, 0, 0);
    _width              = _screenSize.width;
    _height             = _screenSize.height;
    _layoutRoot         = nullptr;
    _state              = InterfacePopupBoard::State::CLOSED;
    _applyBackKey       = true;
    _applyTouchOutClose = true;

    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan     = CC_CALLBACK_2(InterfacePopupBoard::onTouchBegan, this);
    listener->onTouchMoved     = CC_CALLBACK_2(InterfacePopupBoard::onTouchMoved, this);
    listener->onTouchEnded     = CC_CALLBACK_2(InterfacePopupBoard::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(InterfacePopupBoard::onTouchCancelled, this);
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    this->setTouchEnabled(true);
    setKeypadEnabled(true);
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_CHANGE_LANGUAGE, NOTIFYCALLBACK_SELECTOR(this, InterfacePopupBoard::onNotify));
}

bool InterfacePopupBoard::init()
{
    if (!LayerColor::initWithColor(_color, _width, _height))
        return false;
    return true;
}

void InterfacePopupBoard::onEnter()
{
    LayerColor::onEnter();
    this->open();
}

void InterfacePopupBoard::onExit()
{
    LayerColor::onExit();
}

cocos2d::Color4B InterfacePopupBoard::getColor()
{
    return _color;
}

void InterfacePopupBoard::setColor(cocos2d::Color4B color)
{
    if (_color != color)
    {
        _color = color;
        this->initWithColor(_color, _width, _height);
    }
}

void InterfacePopupBoard::open()
{
    float scale = 1.0f;
    if (s_resolution_ratio <= RESOLUTION_1_4)
        scale = RESOLUTION_1_4_SCALE;
    else if (s_resolution_ratio <= RESOLUTION_1_6)
        scale = RESOLUTION_1_6_SCALE;
    _layoutRoot->setScale(scale - 0.25f);
    _state = State::OPENED;
    _layoutRoot->stopAllActions();
    _layoutRoot->setOpacity(0);
    _layoutRoot->runAction(Sequence::create(Spawn::create(FadeIn::create(0.2f), ScaleTo::create(0.1f, scale), nullptr), nullptr));
}

void InterfacePopupBoard::close(bool forceClose /*= false*/)
{
    if (forceClose == false)
    {
        _state = State::CLOSED;
        _layoutRoot->stopAllActions();
        CallFunc* endAnimFunc = CallFunc::create([=] {
            _state = State::CLOSED;
            this->removeFromParent();
        });
        _layoutRoot->runAction(Sequence::create(Spawn::create(ScaleBy::create(0.1f, 0.95f), FadeOut::create(0.1f), nullptr), endAnimFunc, nullptr));
    }
    else
    {
        _state = State::CLOSED;
        _layoutRoot->stopAllActions();
        this->removeFromParent();
    }
}

void InterfacePopupBoard::attach(Node* node, int zOrder)
{
    this->removeFromParent();
    node->addChild(this, zOrder);
}

bool InterfacePopupBoard::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    return true;
}

void InterfacePopupBoard::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
}

void InterfacePopupBoard::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_applyTouchOutClose && _state == InterfacePopupBoard::State::OPENED)
        if (!_layoutRoot->getBoundingBox().containsPoint(touch->getLocation()))
            this->close();
}

void InterfacePopupBoard::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
}

void InterfacePopupBoard::changeLanguage(Language language)
{
}

void InterfacePopupBoard::onNotify(void* sender, int id, const cocos2d::Value& data)
{
    switch (id)
    {
        case EVENT_CHANGE_LANGUAGE:
        {
            ValueMap arr  = data.asValueMap();
            Language lang = (Language) arr[LANGUAGE_TYPE_KEY].asInt();
            changeLanguage(lang);
            break;
        }
        default:
            break;
    }
}

void InterfacePopupBoard::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
                close();
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}