#include "YesNoConfirmPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

YesNoConfirmPopup::YesNoConfirmPopup()
{
    this->_callback = nullptr;
}

YesNoConfirmPopup::~YesNoConfirmPopup()
{
}

bool YesNoConfirmPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void YesNoConfirmPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/YesNoConfirmPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));

    this->_buttonYes = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonYes"));
    this->_buttonYes->addClickEventListener(CC_CALLBACK_1(YesNoConfirmPopup::onYesButtonClick, this));

    this->_buttonNo = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonNo"));
    this->_buttonNo->addClickEventListener(CC_CALLBACK_1(YesNoConfirmPopup::onNoButtonClick, this));

    auto labelYes = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonYes, "Label"));
    auto labelNo  = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonNo, "Label"));

    labelYes->setString(GET_TEXT_BY_ID(TXT_YESNOCONFIRM_YES_BUTTON));
    labelNo->setString(GET_TEXT_BY_ID(TXT_YESNOCONFIRM_NO_BUTTON));
}

void YesNoConfirmPopup::onYesButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (this->_callback != nullptr)
        this->_callback(YesNoConfirmResult::CONFIRM_YES);
}

void YesNoConfirmPopup::onNoButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (this->_callback != nullptr)
        this->_callback(YesNoConfirmResult::CONFIRM_NO);
}

void YesNoConfirmPopup::setCallback(const YesNoCallback& callback)
{
    this->_callback = callback;
}

void YesNoConfirmPopup::setContent(const std::string& content)
{
    this->_textContent->setString(content);
}

void YesNoConfirmPopup::setSize(float width, float height)
{
    this->_layoutRoot->setContentSize(Size(width, height));
    ui::Helper::doLayout(this);
}

cocos2d::Size YesNoConfirmPopup::getSize()
{
    return this->_layoutRoot->getContentSize();
}

void YesNoConfirmPopup::setButtonLabel(const std::string& yesStr, const std::string& noStr)
{
    auto labelYes = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonYes, "Label"));
    auto labelNo  = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonNo, "Label"));
    labelYes->setString(yesStr);
    labelNo->setString(noStr);
}

void YesNoConfirmPopup::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_applyBackKey && _state == InterfacePopupBoard::State::OPENED)
                onNoButtonClick(nullptr);
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}

void YesNoConfirmPopup::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    if (_applyTouchOutClose && _state == InterfacePopupBoard::State::OPENED)
        if (!_layoutRoot->getBoundingBox().containsPoint(touch->getLocation()))
            onNoButtonClick(nullptr);
}
