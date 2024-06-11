#include "LobbySettingBoard.h"
#include "UserData/UserData.h"
#include "Manager/TextManager.h"
#include "Define/TextDefine.h"
#include "Define/GameDefine.h"
#include "Manager/SoundManager.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/NotificationManager.h"
#include "Manager/BoardManager.h"
#include "Helper/fmt/format.h"
#include "FirebaseGG/Firebase.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Helper/GameUtils.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LobbySettingBoard::~LobbySettingBoard()
{
}

LobbySettingBoard::LobbySettingBoard() : InterfacePopupBoard()
{
}

bool LobbySettingBoard::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void LobbySettingBoard::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/LobbySettingBoard.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _textSettingsTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelTitle"));
    _textLanguageTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelLanguage"));
    _textSoundTitle    = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelSound"));

    _cbLanguageEN      = static_cast<CheckBox*>(Helper::seekWidgetByName(_layoutRoot, "CheckBoxLanguageEN"));
    _cbLanguageID      = static_cast<CheckBox*>(Helper::seekWidgetByName(_layoutRoot, "CheckBoxLanguageID"));
    _cbSoundEffect     = static_cast<CheckBox*>(Helper::seekWidgetByName(_layoutRoot, "CheckBoxSoundEffect"));
    _cbSoundBackground = static_cast<CheckBox*>(Helper::seekWidgetByName(_layoutRoot, "CheckBoxSoundBackground"));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));

    _buttonMoreGame = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonMoreGame"));
    _buttonMoreGame->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));
    _buttonMoreGame->setVisible(UserData::getInstance()->isEnableGameCenterPopup());

    _buttonPolicy = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonPrivacyPolicy"));
    _buttonPolicy->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));

    _buttonShare = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonShare"));
    _buttonShare->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));

    _buttonRate = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonLikeUs"));
    _buttonRate->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));
    _buttonRate->setVisible(UserData::getInstance()->isEnableButtonRating());

    _buttonExit = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonExit"));
    _buttonExit->addClickEventListener(CC_CALLBACK_1(LobbySettingBoard::onButtonClick, this));
    _cbLanguageEN->addEventListener(CC_CALLBACK_2(LobbySettingBoard::onCheckBoxClick, this));
    _cbLanguageID->addEventListener(CC_CALLBACK_2(LobbySettingBoard::onCheckBoxClick, this));
    _cbSoundEffect->addEventListener(CC_CALLBACK_2(LobbySettingBoard::onCheckBoxClick, this));
    _cbSoundBackground->addEventListener(CC_CALLBACK_2(LobbySettingBoard::onCheckBoxClick, this));

    _textLanguageEN      = static_cast<Text*>(Helper::seekWidgetByName(_cbLanguageEN, "Label"));
    _textLanguageID      = static_cast<Text*>(Helper::seekWidgetByName(_cbLanguageID, "Label"));
    _textSoundEffect     = static_cast<Text*>(Helper::seekWidgetByName(_cbSoundEffect, "Label"));
    _textSoundBackground = static_cast<Text*>(Helper::seekWidgetByName(_cbSoundBackground, "Label"));

    auto functionCheckbox = [=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        Node* node = (Node*) sender;
        if (type == cocos2d::ui::Widget::TouchEventType::BEGAN)
        {
            SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            if (node == _textLanguageEN)
                selectLanguage(Language::LANGUAGE_ENGLISH);
            if (node == _textLanguageID)
                selectLanguage(Language::LANGUAGE_INDONESIAN);
            else if (node == _textSoundEffect)
                selectSoundEffect(!_cbSoundEffect->isSelected());
            else if (node == _textSoundBackground)
                selectSoundBackground(!_cbSoundBackground->isSelected());
        }
    };

    _textLanguageEN->setTouchEnabled(true);
    _textLanguageEN->addTouchEventListener(functionCheckbox);

    _textLanguageID->setTouchEnabled(true);
    _textLanguageID->addTouchEventListener(functionCheckbox);

    _textSoundEffect->setTouchEnabled(true);
    _textSoundEffect->addTouchEventListener(functionCheckbox);

    _textSoundBackground->setTouchEnabled(true);
    _textSoundBackground->addTouchEventListener(functionCheckbox);

    selectLanguage(TextManager::getInstance()->getLanguageType(), true);
    selectSoundEffect(UserData::getInstance()->getSoundEffect(), true);
    selectSoundBackground(UserData::getInstance()->getSoundBackground(), true);
    changeLanguage(TextManager::getInstance()->getLanguageType());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _buttonExit->setVisible(false);
#endif
}

void LobbySettingBoard::changeLanguage(Language language)
{
    _textSettingsTitle->setString(GET_TEXT_BY_ID(TXT_SETTINGS_TITLE));
    _textLanguageTitle->setString(GET_TEXT_BY_ID(TXT_LANGUAGE_TITLE));
    _textSoundTitle->setString(GET_TEXT_BY_ID(TXT_SOUND_TITLE));
    _textLanguageEN->setString(GET_TEXT_BY_ID(TXT_LANGUAGE_EN));
    _textLanguageID->setString(GET_TEXT_BY_ID(TXT_LANGUAGE_ID));
    _textSoundEffect->setString(GET_TEXT_BY_ID(TXT_SOUND_EFFECT));
    _textSoundBackground->setString(GET_TEXT_BY_ID(TXT_SOUND_BACKGROUND));

    auto labelExit = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonExit, "Label"));
    labelExit->setString(GET_TEXT_BY_ID(TXT_BUTTON_EXIT_GAME));
    auto labelMoreGame = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonMoreGame, "Label"));
    labelMoreGame->setString(GET_TEXT_BY_ID(TXT_MORE_GAME));
    auto labelPolicy = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonPolicy, "Label"));
    labelPolicy->setString(GET_TEXT_BY_ID(TXT_PRIVACY_POLICY));
    auto labelShare = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonShare, "Label"));
    labelShare->setString(GET_TEXT_BY_ID(TXT_SHARE));
    auto labelRate = static_cast<Text*>(Helper::seekWidgetByName(this->_buttonRate, "Label"));
    labelRate->setString(GET_TEXT_BY_ID(TXT_LIKE_US));
    auto labelVersion = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoot, "TextVersion"));
    labelVersion->setString(fmt::format(GET_TEXT_BY_ID(TXT_VERSION_AND_COPYRIGHT), s_versionName + "." + fmt::format("{0}", s_versionCode), GameUtils::getDeveloperName()));
}

void LobbySettingBoard::notifyChangeLanguage(Language language)
{
    if (language != TextManager::getInstance()->getLanguageType())
    {
        TextManager::getInstance()->setLanguageType(language);
        cocos2d::ValueMap arr;
        arr[LANGUAGE_TYPE_KEY] = (int) language;
        cocos2d::Value data(arr);
        NotificationManager::getInstance()->postNotification(EVENT_CHANGE_LANGUAGE, data);
        playChangeLanguageEffect();
    }
}

void LobbySettingBoard::playChangeLanguageEffect()
{
    Layout* layout = Layout::create();
    layout->setContentSize(_screenSize + Size(5, 5));
    layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    layout->setPosition(_screenSize / 2);
    layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    layout->setBackGroundColor(Color3B(POPUP_BACKGROUND_COLOR));
    layout->setBackGroundColorOpacity(255);
    layout->setOpacity(0);
    addChild(layout, kAboveForeground);
    layout->runAction(Sequence::create(FadeIn::create(0.05f), FadeOut::create(0.1f), RemoveSelf::create(), nullptr));
}

void LobbySettingBoard::initValue(int type/* = 0*/)
{
    selectLanguage(TextManager::getInstance()->getLanguageType(), true);
    selectSoundEffect(UserData::getInstance()->getSoundEffect(), true);
    selectSoundBackground(UserData::getInstance()->getSoundBackground(), true);

    if (type == 0) //lobby
    {
        _buttonExit->setColor(Color3B::WHITE);
        _buttonExit->setEnabled(true);
    }
    else if (type == 1) //ingame
    {
        _buttonExit->setColor(Color3B(100, 100, 100));
        _buttonExit->setEnabled(false);
    }
}

void LobbySettingBoard::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
    {
        close();
    }
    else if (sender == _buttonExit)
    {
        ActionConfirmPopup* confirmPopup = BoardManager::createActionConfirmPopup();
        confirmPopup->initValue(GET_TEXT_BY_ID(TXT_EXIT_GAME_CONFIRM), GET_TEXT_BY_ID(TXT_YESNOCONFIRM_NO_BUTTON), GET_TEXT_BY_ID(TXT_BUTTON_EXIT_GAME), [=](ActionConfirmResult result) {
            if (result == ActionConfirmResult::CONFIRM_ACTION)
                Director::getInstance()->end();
            else
                confirmPopup->close();
        });
        confirmPopup->attach(getParent(), kAboveForeground);
        this->close(true);
    }
    else if (sender == _buttonMoreGame)
    {
        if (UserData::getInstance()->isEnableGameCenterPopup())
        {
            GameCenterPopup* gameCenterPopup = BoardManager::createGameCenterPopup();
            gameCenterPopup->attach(getParent(), kAboveForeground);
            this->close(true);
        }
        else
        {
            CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_GAME_CENTER_IS_MAINTAINING));
        }
    }
    else if (sender == _buttonShare)
    {
        std::string data = RemoteConfig::getString(REMOTECONFIG_FUNCTION_SHARE_GAME_LINK, SHARE_GAME_URL_JSON_DATA);
        if (data != "")
        {
            const rapidjson::Document& doc = JsonHelper::parseStringToJson(data);
            std::string urlApp  = JsonHelper::getString(doc, "data", "");
            std::string message = fmt::format(GET_TEXT_BY_ID(TXT_SHARE_APP_MESSAGE), GameUtils::getGameName().c_str(), urlApp.c_str());
            CallToOtherLanguage::getInstance()->shareDataToMessenger(GameUtils::getGameName(), message);
        }
    }
    else if (sender == _buttonPolicy)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        Application::getInstance()->openURL(RemoteConfig::getString(REMOTECONFIG_FUNCTION_PRIVACY_POLICY_LINK, PRIVACY_POLICY_LINK_IOS));
#else
        Application::getInstance()->openURL(RemoteConfig::getString(REMOTECONFIG_FUNCTION_PRIVACY_POLICY_LINK, PRIVACY_POLICY_LINK_ANDROID));
#endif
    }
    else if (sender == _buttonRate)
    {
        Firebase::logOpenAppRating();
        CallToOtherLanguage::getInstance()->openAppInStore();
    }
}

void LobbySettingBoard::selectLanguage(Language language, bool init)
{
    if (language != TextManager::getInstance()->getLanguageType())
        notifyChangeLanguage(language);
    _cbLanguageEN->stopAllActions();
    _cbLanguageEN->setScale(1.0f);
    _cbLanguageEN->setSelected(false);

    _cbLanguageID->stopAllActions();
    _cbLanguageID->setScale(1.0f);
    _cbLanguageID->setSelected(false);

    if (language == Language::LANGUAGE_ENGLISH)
    {
        _cbLanguageEN->setSelected(true);
        if (!init)
            _cbLanguageEN->runAction(Sequence::create(ScaleBy::create(0.1f, 0.95f), ScaleTo::create(0.05f, 1.0f), nullptr));
    }
    else if (language == Language::LANGUAGE_INDONESIAN)
    {
        _cbLanguageID->setSelected(true);
        if (!init)
            _cbLanguageID->runAction(Sequence::create(ScaleBy::create(0.1f, 0.95f), ScaleTo::create(0.05f, 1.0f), nullptr));
    }
}

void LobbySettingBoard::selectSoundEffect(bool state, bool init)
{
    _cbSoundEffect->stopAllActions();
    _cbSoundEffect->setScale(1.0f);
    _cbSoundEffect->setSelected(state);
    if (UserData::getInstance()->getSoundEffect() != state)
        UserData::getInstance()->setSoundEffect(state);
    if (!init)
        _cbSoundEffect->runAction(Sequence::create(ScaleBy::create(0.1f, 0.95f), ScaleTo::create(0.05f, 1.0f), nullptr));
}

void LobbySettingBoard::selectSoundBackground(bool state, bool init)
{
    _cbSoundBackground->stopAllActions();
    _cbSoundBackground->setScale(1.0f);
    _cbSoundBackground->setSelected(state);
    if (UserData::getInstance()->getSoundBackground() != state)
    {
        UserData::getInstance()->setSoundBackground(state);
        if (state)
        {
            if (UserData::getInstance()->getRestoreSoundBG())
            {
                UserData::getInstance()->setRestoreSoundBG(false);
                CallToOtherLanguage::getInstance()->forceToRegisterAudioFocusListenner();
            }
            SoundManager::getInstance()->playBackgroundMusicAuto();
        }
        else
        {
            SoundManager::getInstance()->stopBackgroundMusicAuto();
        }
    }
    if (!init)
        _cbSoundBackground->runAction(Sequence::create(ScaleBy::create(0.1f, 0.95f), ScaleTo::create(0.05f, 1.0f), nullptr));
}

void LobbySettingBoard::onCheckBoxClick(cocos2d::Ref* ref, cocos2d::ui::CheckBox::EventType type)
{
    if (ref == _cbLanguageEN)
        selectLanguage(Language::LANGUAGE_ENGLISH);
    if (ref == _cbLanguageID)
        selectLanguage(Language::LANGUAGE_INDONESIAN);
    else if (ref == _cbSoundEffect)
    {
        if (type == cocos2d::ui::CheckBox::EventType::SELECTED)
            selectSoundEffect(true);
        else if (type == cocos2d::ui::CheckBox::EventType::UNSELECTED)
            selectSoundEffect(false);
    }
    else if (ref == _cbSoundBackground)
    {
        if (type == cocos2d::ui::CheckBox::EventType::SELECTED)
            selectSoundBackground(true);
        else if (type == cocos2d::ui::CheckBox::EventType::UNSELECTED)
            selectSoundBackground(false);
    }
}
