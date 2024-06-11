#include "LobbyInfoUserBoard.h"
#include "Helper/GameUtils.h"
#include "Manager/TextManager.h"
#include "Manager/MyActionsManager.h"
#include "UserData/UserData.h"
#include "Network/MessageSender.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/BoardManager.h"
#include "Network/MessageHandler.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LobbyInfoUserBoard::~LobbyInfoUserBoard()
{
    NotificationManager::getInstance()->unregisterNotification(this);
}

LobbyInfoUserBoard::LobbyInfoUserBoard()
{
}

bool LobbyInfoUserBoard::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void LobbyInfoUserBoard::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/LobbyUserInfoBoard.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelContent"));
    _textUsername  = static_cast<Text*>(Helper::seekWidgetByName(_layoutContent, "TextUsername"));

    _layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "PanelAvatar"));
    _layoutAvatar->addClickEventListener(CC_CALLBACK_1(LobbyInfoUserBoard::onLayoutClick, this));

    _buttonConfirm = static_cast<Button*>(Helper::seekWidgetByName(_layoutContent, "ButtonConfirm"));
    _buttonConfirm->setPressedActionEnabled(false);
    _buttonConfirm->addClickEventListener(CC_CALLBACK_1(LobbyInfoUserBoard::onButtonClick, this));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(LobbyInfoUserBoard::onButtonClick, this));

    _buttonEdit = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonEdit"));
    _buttonEdit->addClickEventListener(CC_CALLBACK_1(LobbyInfoUserBoard::onButtonClick, this));

    _textProfileTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTitle"));

    _textTotalTitle        = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotalTitle"));
    _textTotalWinTitle     = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotalWinTitle"));
    _textTotalWinRateTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotalWinRateTitle"));

    _textTotal        = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotal"));
    _textTotalWin     = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotalWin"));
    _textTotalWinRate = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTotalWinRate"));
    _textMoney        = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "LabelMoney"));
    _textLevel        = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "LabelLevel"));

    _buttonSignOut = static_cast<Button*>(Helper::seekWidgetByName(_layoutContent, "ButtonSignOut"));
    _buttonSignOut->addClickEventListener([=](cocos2d::Ref* sender) {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        CallToOtherLanguage::getInstance()->signoutPlayGameService();
    });
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _buttonSignOut->setEnabled(false);
#endif
    _buttonSignIn = static_cast<Button*>(Helper::seekWidgetByName(_layoutContent, "ButtonSignIn"));
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ((ImageView*)_buttonSignIn->getChildByName("Image_9_0"))->loadTexture("ui/ImagesCommon/icon_apple_game_center.png", ui::Widget::TextureResType::PLIST);
    ((ImageView*)_buttonSignIn->getChildByName("Image_9"))->loadTexture("ui/ImagesCommon/icon_apple_game_center.png", ui::Widget::TextureResType::PLIST);
#endif
    _buttonSignIn->addClickEventListener([=](cocos2d::Ref* sender) {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        CallToOtherLanguage::getInstance()->signinPlayGameService();
    });

    auto layoutPlayerGameService = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "LayoutPlayGameService"));
    layoutPlayerGameService->setVisible(false);
    if (CallToOtherLanguage::getInstance()->havePlayGameService())
        layoutPlayerGameService->setVisible(true);

    auto layoutKillerAds = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelAdKiller"));
    layoutKillerAds->setVisible(false);
    if (!UserData::getInstance()->isEnableInterferingAds())
        layoutKillerAds->setVisible(true);

    changeLanguage(TextManager::getInstance()->getLanguageType());
}

void LobbyInfoUserBoard::onEnter()
{
    InterfacePopupBoard::onEnter();
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_NAME_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyInfoUserBoard::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_AVATAR_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyInfoUserBoard::onServerResponse));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_PLAYGAME_SIGNIN, NOTIFYCALLBACK_SELECTOR(this, LobbyInfoUserBoard::onReceiveNotificationValue));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_PLAYGAME_SIGNOUT, NOTIFYCALLBACK_SELECTOR(this, LobbyInfoUserBoard::onReceiveNotificationValue));
}

void LobbyInfoUserBoard::onExit()
{
    InterfacePopupBoard::onExit();
    NotificationManager::getInstance()->unregisterNotification(this, EVENT_PLAYGAME_SIGNIN);
    NotificationManager::getInstance()->unregisterNotification(this, EVENT_PLAYGAME_SIGNOUT);
    MessageHandler::getInstance()->unregisterHandleMessage(this);
}

void LobbyInfoUserBoard::onReceiveNotificationValue(void* sender, int id, const cocos2d::Value& data)
{
    switch (id)
    {
        case EVENT_PLAYGAME_SIGNIN:
        {
            auto layoutSigned    = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "SignedIn"));
            auto layoutNotSigned = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "NotSigned"));
            layoutSigned->setVisible(true);
            layoutNotSigned->setVisible(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            layoutSigned->setVisible(false);
#endif
            break;
        }
        case EVENT_PLAYGAME_SIGNOUT:
        {
            auto layoutSigned    = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "SignedIn"));
            auto layoutNotSigned = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "NotSigned"));
            layoutSigned->setVisible(false);
            layoutNotSigned->setVisible(true);
            break;
        }
    }
}

void LobbyInfoUserBoard::initValue(const std::string& name, Avatar* avatar)
{
    _name = name;
    _textUsername->setString(_name);
    _layoutAvatar->removeChildByTag(AVATAR_TAG);
    avatar->setFrameUrl(UserData::getInstance()->getAvatarLink(), _layoutAvatar->getContentSize());
    avatar->setPosition(Point(_layoutAvatar->getContentSize().width / 2, _layoutAvatar->getContentSize().height / 2));
    avatar->setTag(AVATAR_TAG);
    _layoutAvatar->addChild(avatar, -1);

    _buttonConfirm->setEnabled(false);
    _buttonConfirm->setColor(BUTTON_COLOR_DISABLE);

    _textTotal->setString(StringUtils::format("%d", s_winNumber + s_loseNumber));
    _textTotalWin->setString(StringUtils::format("%d", s_winNumber));
    _textTotalWinRate->setString(StringUtils::format("%.2f%%", (s_winNumber + s_loseNumber) == 0 ? 0 : (s_winNumber) * 100.0f / (s_winNumber + s_loseNumber)));

    _textMoney->setString(StringUtils::format("%s", GameUtils::MoneyFormat(UserData::getInstance()->getMoney(), "$").c_str()));
    _textLevel->setString(StringUtils::format("Lv. %d", UserData::getInstance()->getLevel()));

    if (CallToOtherLanguage::getInstance()->havePlayGameService())
    {
        auto layoutSigned    = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "SignedIn"));
        auto layoutNotSigned = static_cast<Layout*>(Helper::seekWidgetByName(_layoutContent, "NotSigned"));
        if (CallToOtherLanguage::getInstance()->isPlayGameServiceSignedIn())
        {
            layoutSigned->setVisible(true);
            layoutNotSigned->setVisible(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            layoutSigned->setVisible(false);
#endif
        }
        else
        {
            layoutSigned->setVisible(false);
            layoutNotSigned->setVisible(true);
        }
    }
}

void LobbyInfoUserBoard::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonConfirm)
    {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        if (_state == InterfacePopupBoard::State::OPENED)
            this->close();
    }
    else if (button == _buttonClose)
    {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        this->close();
    }
    else if (button == _buttonEdit)
    {
        onLayoutClick(_layoutAvatar);
    }
}

void LobbyInfoUserBoard::onLayoutClick(cocos2d::Ref* sender)
{
    Layout* layout = (Layout*) sender;
    if (layout == _layoutAvatar)
    {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        ChangeAvatarPopup* notifyPopup = BoardManager::createChangeAvatarPopup();
        notifyPopup->initValue();
        notifyPopup->attach(this->getParent(), kAboveForeground);
    }
}

void LobbyInfoUserBoard::changeLanguage(Language language)
{
    _textProfileTitle->setString(GET_TEXT_BY_ID(TXT_PROFILE_TITLE));
    _textTotalTitle->setString(GET_TEXT_BY_ID(TXT_PROFILE_TOTAL_MATCH));
    _textTotalWinTitle->setString(GET_TEXT_BY_ID(TXT_PROFILE_TOTAL_WIN));
    _textTotalWinRateTitle->setString(GET_TEXT_BY_ID(TXT_PROFILE_TOTAL_WINRATE));

    auto labelBtnSignOut = static_cast<Text*>(Helper::seekWidgetByName(_buttonSignOut, "Label"));
    labelBtnSignOut->setString(GET_TEXT_BY_ID(TXT_BUTTON_GGPGS_SIGN_OUT));

    auto titleSigned = static_cast<Text*>(Helper::seekWidgetByName(_layoutContent, "TitleSigned"));
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    titleSigned->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_SIGNED));
#else
    titleSigned->setString(GET_TEXT_BY_ID(TXT_GGPGS_SIGNED));
#endif

    auto labelEdit = static_cast<Text*>(Helper::seekWidgetByName(_buttonEdit, "Label"));
    labelEdit->setString(GET_TEXT_BY_ID(TXT_CHANGE_AVATAR_EDIT));
}

void LobbyInfoUserBoard::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_CHANGE_USER_NAME_RESPONSE:
        {
            this->onChangeUserName(data);
            break;
        }
        case MessageID::MESSAGE_CHANGE_USER_AVATAR_RESPONSE:
        {
            this->onChangeUserAvatar(data);
            break;
        }
        default:
            break;
    }
}

void LobbyInfoUserBoard::onChangeUserName(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        const std::string& name = data[KEY_NAME].GetString();
        UserData::getInstance()->setUserName(name);
        _textUsername->setString(name);
    }
}

void LobbyInfoUserBoard::onChangeUserAvatar(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        const std::string& name = data[KEY_AVATAR].GetString();
        UserData::getInstance()->setAvatarLink(name);
        ((Avatar*) _layoutAvatar->getChildByTag(AVATAR_TAG))->setFrameUrl(UserData::getInstance()->getAvatarLink(), _layoutAvatar->getContentSize());
    }
}
