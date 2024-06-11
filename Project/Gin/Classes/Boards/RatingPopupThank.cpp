#include "RatingPopupThank.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

RatingPopupThank::~RatingPopupThank()
{
}

RatingPopupThank::RatingPopupThank()
{
    _callbackOK = nullptr;
}

bool RatingPopupThank::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void RatingPopupThank::setType(RatingPopupThank::Type type)
{
    _type = type;
    if (_type == RatingPopupThank::Type::THANK_LOW_STAR)
        this->_textContent->setString(GET_TEXT_BY_ID(TXT_RATING_POPUP_THANK_CONTENT_LOW_STAR));
    else if (_type == RatingPopupThank::Type::THANK_HIGH_STAR)
        this->_textContent->setString(GET_TEXT_BY_ID(TXT_RATING_POPUP_THANK_CONTENT_HIGH_STAR));
}

void RatingPopupThank::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/RatingPopupThank.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textTitle = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "TextTitle"));
    this->_textTitle->setString(GET_TEXT_BY_ID(TXT_RATING_POPUP_THANK_TITLE));

    this->_textContent = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextContent"));
    this->_textContent->setString(GET_TEXT_BY_ID(TXT_RATING_POPUP_THANK_CONTENT_HIGH_STAR));

    this->_buttonOK = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonOK"));
    this->_buttonOK->addClickEventListener(CC_CALLBACK_1(RatingPopupThank::onOKButtonClick, this));
}

void RatingPopupThank::onOKButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (_callbackOK != nullptr)
        _callbackOK();
    this->close();
}