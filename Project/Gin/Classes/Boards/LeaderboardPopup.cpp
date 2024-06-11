#include "LeaderboardPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Manager/MyActionsManager.h"
#include "Native/CallToOtherLanguage.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LeaderboardPopup::LeaderboardPopup()
{
}

LeaderboardPopup::~LeaderboardPopup()
{
}

bool LeaderboardPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void LeaderboardPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/LeaderboardPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(LeaderboardPopup::onButtonClick, this));

    _buttonLeaderboard1 = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonBillionaireClub"));
    _buttonLeaderboard1->addClickEventListener(CC_CALLBACK_1(LeaderboardPopup::onButtonClick, this));

    _buttonLeaderboard2 = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonMillionaireClub"));
    _buttonLeaderboard2->addClickEventListener(CC_CALLBACK_1(LeaderboardPopup::onButtonClick, this));

    _buttonLeaderboard3 = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonLevelClub"));
    _buttonLeaderboard3->addClickEventListener(CC_CALLBACK_1(LeaderboardPopup::onButtonClick, this));

    _textHint  = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoot, "TextHint"));
    _textTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoot, "LabelTitle"));

    this->changeLanguage(TextManager::getInstance()->getLanguageType());
}

void LeaderboardPopup::onButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
    else if (sender == _buttonLeaderboard1)
        CallToOtherLanguage::getInstance()->showLeaderBoardBillionaireClub();
    else if (sender == _buttonLeaderboard2)
        CallToOtherLanguage::getInstance()->showLeaderBoardMillionaireClub();
    else if (sender == _buttonLeaderboard3)
        CallToOtherLanguage::getInstance()->showLeaderBoardLevel();
}

void LeaderboardPopup::changeLanguage(Language)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _textHint->setString(GET_TEXT_BY_ID(TXT_SIGN_IN_GAME_CENTER_VIEW_LEADERBOARD));
#else
    _textHint->setString(GET_TEXT_BY_ID(TXT_SIGN_IN_GGPGS_VIEW_LEADERBOARD));
#endif
    _textTitle->setString(GET_TEXT_BY_ID(TXT_LEADERBOARD_TITLE));

    auto label = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonLeaderboard1, "Label"));
    label->setString(GET_TEXT_BY_ID(TXT_BUTTON_BILLIONAIRE_CLUB));
    label = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonLeaderboard2, "Label"));
    label->setString(GET_TEXT_BY_ID(TXT_BUTTON_MILLIONAIRE_CLUB));
    label = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonLeaderboard3, "Label"));
    label->setString(GET_TEXT_BY_ID(TXT_BUTTON_LEVEL_CLUB));
}
