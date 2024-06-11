#include "NotifyPopup.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"

USING_NS_CC;
using namespace ui;
using namespace std;

NotifyPopup::NotifyPopup()
{
    _okCallback = nullptr;
}

NotifyPopup::~NotifyPopup()
{
}

bool NotifyPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void NotifyPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/NotifyPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _buttonOk = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonOk"));
    _buttonOk->addClickEventListener(CC_CALLBACK_1(NotifyPopup::onButtonClick, this));

    _textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));
    setButtonLable("OK");
}

void NotifyPopup::initValue(const std::string& content, const NotifyOkCallback& callback)
{
    _textContent->setString(content);
    _okCallback = callback;
}

void NotifyPopup::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonOk)
    {
        if (_okCallback != nullptr)
            _okCallback();
        this->close();
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void NotifyPopup::setSize(float width, float height)
{
    this->_layoutRoot->setContentSize(Size(width, height));
    ui::Helper::doLayout(this);
}

void NotifyPopup::setButtonLable(const std::string& lb)
{
    auto label = static_cast<Text*>(Helper::seekWidgetByName(_buttonOk, "Label"));
    label->setString(lb);
}

void NotifyPopup::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_applyTouchOutClose && _state == InterfacePopupBoard::State::OPENED)
        if (!_layoutRoot->getBoundingBox().containsPoint(touch->getLocation()))
            onButtonClick(_buttonOk);
}

void NotifyPopup::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
                onButtonClick(_buttonOk);
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}