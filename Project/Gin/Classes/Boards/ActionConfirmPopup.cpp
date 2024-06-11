#include "ActionConfirmPopup.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

ActionConfirmPopup::ActionConfirmPopup()
{
    this->_callback = nullptr;
}

ActionConfirmPopup::~ActionConfirmPopup()
{
}

bool ActionConfirmPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void ActionConfirmPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/ActionConfirmPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    this->_buttonAction = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonAction"));
    this->_buttonAction->addClickEventListener(CC_CALLBACK_1(ActionConfirmPopup::onActionButtonClick, this));

    this->_buttonOk = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonOk"));
    this->_buttonOk->addClickEventListener(CC_CALLBACK_1(ActionConfirmPopup::onOkButtonClick, this));
}

void ActionConfirmPopup::onActionButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (this->_callback != nullptr)
        this->_callback(ActionConfirmResult::CONFIRM_ACTION);
}

void ActionConfirmPopup::onOkButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (this->_callback != nullptr)
        this->_callback(ActionConfirmResult::CONFIRM_OK);
}

void ActionConfirmPopup::setSize(float width, float height)
{
    this->_layoutRoot->setContentSize(Size(width, height));
    ui::Helper::doLayout(this);
}

void ActionConfirmPopup::initValue(const std::string& content, const std::string& strOk, const std::string& strAction, const ActionConfirmCallback& callback)
{
    this->_callback = callback;
    this->_textContent->setString(content);

    auto labelOk     = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonOk, "Label"));
    auto labelAction = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonAction, "Label"));

    labelOk->setString(strOk);
    labelAction->setString(strAction);
}

void ActionConfirmPopup::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_state == InterfacePopupBoard::State::OPENED)
        if (!_layoutRoot->getBoundingBox().containsPoint(touch->getLocation()))
            if (this->_callback != nullptr)
                this->_callback(ActionConfirmResult::CONFIRM_OK);
}

void ActionConfirmPopup::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
                if (this->_callback != nullptr)
                    this->_callback(ActionConfirmResult::CONFIRM_OK);
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}