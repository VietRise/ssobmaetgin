#include "LobbyScene.h"
#include "Boards/LobbyTicketFlightBoard.h"
#include "Boards/LobbySettingBoard.h"
#include "Boards/LobbyInfoUserBoard.h"
#include "Manager/BoardManager.h"
#include "Manager/TextManager.h"
#include "Network/MessageHandler.h"
#include "Network/MessageSender.h"
#include "Helper/JsonHelper.h"
#include "UserData/UserData.h"
#include "Component/Avatar.h"
#include "Manager/SceneManager.h"
#include "Manager/MyActionsManager.h"
#include "Helper/GameUtils.h"
#include "Scenes/DebugLayer.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/NotificationManager.h"
#include "FirebaseGG/Firebase.h"
#include "Boards/DailyBonusPopup.h"
#include "Boards/NewsPopup.h"
#include "Helper/fmt/format.h"
#include "Manager/DownloadManager.h"
#include "Define/GameDefine.h"
#include "Boards/VideoRewardsPopup.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/AdsManager.h"
#include "Define/ResourcesDefine.h"
#include "Component/TutorialView.h"
#include "Component/RuleScore.h"
#include "Component/RuleDetails.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace rapidjson;

#define TAG_ACTION_COUNTDOWN_HOURLY_BONUS 0
#define TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS 1
#define TAG_ACTION_DECOR_LAYOUT_TIMELINE 100

bool LobbyScene::_backFromGameScene = false;

LobbyScene::~LobbyScene()
{
    MessageHandler::getInstance()->unregisterHandleMessage(this);

    for (int i = 0; i < _listPlace.size(); i++)
        CC_SAFE_RELEASE(_listPlace.at(i));
    _listPlace.clear();
}

LobbyScene::LobbyScene()
{
    _lobbyTableSelected = nullptr;
    _listDataStr        = "";
    _listMailStr        = "";
    _ratingState        = 0;
    _ratingCondition    = "";
}

Scene* LobbyScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LobbyScene::create();
    scene->addChild(layer);
    return scene;
}

bool LobbyScene::init()
{
    if (!InterfaceScene::init())
        return false;

    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PROFILE_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_USER_DATA_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_UNLOCK_CITY_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_UNLOCK_TABLE_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_NAME_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_AVATAR_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_TICKET_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_JOIN_GAME_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_BONUS_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOG_IN_OFFLINE_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SUGGEST_UPDATE_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_MAILBOX_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE, MESSAGEHANDLE_SELECTOR(LobbyScene::onServerResponse));

    this->initScreen();
    return true;
}

void LobbyScene::initScreen()
{
    setKeypadEnabled(true);
    this->setContentSize(_screenSize);
    Node* node  = CSLoader::createNode("ui/LobbyScene/LobbyScene.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    _layoutRoot = static_cast<Layout*>(child);
    _layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    _layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kMiddleground);
    if (s_enableSafeArea)
    {
        float padding = SAFE_AREA_PADDING;

        LayoutComponent* layoutComponent = (LayoutComponent*) static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "TopPanel"))->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();

        layoutComponent = (LayoutComponent*) static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "PanelBottom"))->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();
    }

    ui::Helper::doLayout(this);

    _buttonSetting = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonSetting"));
    _buttonSetting->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    //_buttonSetting->setPressedActionEnabled(true);

    _buttonRating = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonRating"));
    _buttonRating->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _buttonRating->setVisible(UserData::getInstance()->isEnableButtonRating());
    //_buttonRating->setPressedActionEnabled(true);

    _buttonFreeGold = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonFreeGold"));
    _buttonFreeGold->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    //_buttonFreeGold->setPressedActionEnabled(true);

    _textBMFreeGold = static_cast<TextBMFont*>(Helper::seekWidgetByName(_buttonFreeGold, "TextBMFreeGold"));

    _buttonVideoBonusReward = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonVideoBonusReward"));
    _buttonVideoBonusReward->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _buttonVideoBonusReward->setVisible(false);

    _textBMVideoBonusReward      = static_cast<Text*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "TextBMMesage"));
    _imageVideoBonusReward       = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageBox"));
    _imageVideoBonusRewardGlow0  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow0"));
    _imageVideoBonusRewardGlow1  = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonVideoBonusReward, "ImageGlow1"));
    _imageVideoBonusRewardScaleX = _imageVideoBonusReward->getScaleX();
    _imageVideoBonusRewardScaleY = _imageVideoBonusReward->getScaleY();

    _buttonGameCenter = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonGameCenter"));
    _buttonGameCenter->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _buttonGameCenter->setVisible(UserData::getInstance()->isEnableGameCenterPopup());
    _textGameCenter = static_cast<Text*>(Helper::seekWidgetByName(_buttonGameCenter, "LabelGameCenter"));

    _buttonPromoteGame1 = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonPromoteGame1"));
    _buttonPromoteGame1->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _buttonPromoteGame1->setVisible(false);

    _buttonPromoteGame2 = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonPromoteGame2"));
    _buttonPromoteGame2->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _buttonPromoteGame2->setVisible(false);

    _buttonHelp = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonHelp"));
    _buttonHelp->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    //_buttonHelp->setPressedActionEnabled(true);

    _buttonLeaderboard = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonLeaderboard"));
    _buttonLeaderboard->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));

    auto leaderboardShadow = Helper::seekWidgetByName(_layoutRoot, "ButtonLeaderboardShadow");

    _buttonLeaderboard->setVisible(false);
    leaderboardShadow->setVisible(false);

    if (CallToOtherLanguage::getInstance()->havePlayGameService())
    {
        _buttonLeaderboard->setVisible(true);
        leaderboardShadow->setVisible(true);
    }

    _buttonMailbox = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonMail"));
    _buttonMailbox->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));

    _layoutBottomBar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelBottom"));

    _textVideoBonusHint   = static_cast<Text*>(Helper::seekWidgetByName(_layoutBottomBar, "TextVideoHint"));
    _textHourlyBonusTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutBottomBar, "BMLabelTitle"));

    _buttonShop = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonShop"));
    _buttonShop->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));

    _textBMMoneyUser = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "TextBMMoneyUser"));
    _layoutInfoUser  = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelInfoUser"));
    _layoutInfoUser->addTouchEventListener(CC_CALLBACK_2(LobbyScene::onLayoutClick, this));
    _layoutAvatarUser = static_cast<Layout*>(Helper::seekWidgetByName(_layoutInfoUser, "PanelAvatar"));
    _textBMNameUser   = static_cast<Text*>(Helper::seekWidgetByName(_layoutInfoUser, "TextBMNameUser"));

    _textUserLevel   = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutInfoUser, "TextLevel"));
    _sliderUserLevel = static_cast<Slider*>(Helper::seekWidgetByName(_layoutInfoUser, "LevelSlider"));
    _sliderUserLevel->setMaxPercent(100);

    _layoutChoosePlace = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelChoosePlace"));
    _layoutChoosePlace->setLocalZOrder(kForeground);

    auto layout3Players = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_1"));
    layout3Players->addTouchEventListener(CC_CALLBACK_2(LobbyScene::onTouchLayoutPlaceListener, this));
    layout3Players->setVisible(false);
    auto layout3PlayersHitpot = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_2"));
    layout3PlayersHitpot->addTouchEventListener(CC_CALLBACK_2(LobbyScene::onTouchLayoutPlaceListener, this));
    layout3PlayersHitpot->setVisible(false);
    auto layout3PlayersSpecialHitpot = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_3"));
    layout3PlayersSpecialHitpot->addTouchEventListener(CC_CALLBACK_2(LobbyScene::onTouchLayoutPlaceListener, this));
    layout3PlayersSpecialHitpot->setVisible(false);


    _layoutEventDecor = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelEventBackground"));
    checkAndEnableEventDecorLayer();

    _buttonHourlyBonus        = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonHourlyBonus"));
    _textHourlyBonusMoney     = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "BMLabelMoney"));
    _textHourlyBonusCountDown = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "BMLabelCountDown"));
    _textHourlyBonusTitle     = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "BMLabelTitle"));
    _imgHourlyBonusGet        = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, "ImgGet"));
    _textHourlyBonusGetNow    = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "BMLabelGetNow"));
    _buttonHourlyBonus->setPressedActionEnabled(false);
    _textHourlyBonusCountDown->setString("");
    _buttonHourlyBonus->addClickEventListener(CC_CALLBACK_1(LobbyScene::onButtonClick, this));
    _posButtonHourlyBonus = _buttonHourlyBonus->getPosition();

    changeLanguage(TextManager::getInstance()->getLanguageType());

#if (USE_DEBUG_VIEW == 1)
    DebugLayer::destroyInstance();
    DebugLayer::getInstance()->attach(this);
#endif

    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        float scale = 1.0f;
        if (s_resolution_ratio <= RESOLUTION_1_4)
            scale = RESOLUTION_1_4_SCALE;
        else if (s_resolution_ratio <= RESOLUTION_1_6)
            scale = RESOLUTION_1_6_SCALE;

        auto topRight = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "TopRight"));
        topRight->setScale(scale);
        auto topCenter = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "TopCenter"));
        topCenter->setScale(scale);
        auto topLeft = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "TopLeft"));
        topLeft->setScale(scale);

        auto botRight = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "BottomRight"));
        botRight->setScale(scale);
        auto botLeft = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "BottomLeft"));
        botLeft->setScale(scale);
        auto centerRight = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "CenterRight"));
        centerRight->setScale(scale);

        _layoutChoosePlace->setContentSize(Size(_layoutRoot->getContentSize().width * 0.75f, _layoutChoosePlace->getContentSize().height));
        _layoutChoosePlace->setScale(1.0f);
        Helper::doLayout(_layoutChoosePlace);
    }
    else if (_screenSize.width <= 1200)
    {
        _layoutChoosePlace->setContentSize(Size(_layoutRoot->getContentSize().width * 0.62, _layoutChoosePlace->getContentSize().height));
        Helper::doLayout(_layoutChoosePlace);
    }
}

void LobbyScene::onEnter()
{
    InterfaceScene::onEnter();

    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_CHANGE_LANGUAGE, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, NOTIFY_USERDATA_CHANGE_MONEY, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_PLAYGAME_SIGNIN, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_PLAYGAME_SIGNOUT, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_INTERSTITIAL_HIDE, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_SHOW_CUSTOM_APP_REVIEW, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_RATING_REVIEW_COMPLETED, NOTIFYCALLBACK_SELECTOR(this, LobbyScene::onNotify));

    if (UserData::getInstance()->getSoundBackground())
    {
        SoundManager::getInstance()->stopBackgroundMusicAuto();
        SoundManager::getInstance()->playBackgroundMusicAuto();
    }
    else
    {
        GameUtils::checkToRestoreSoundBackground();
    }
    s_showInterstitial = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_ADS_SHOW_INGAME_FREQUENCY, 2);
    MessageSender::requestUserProfile("lobby", s_versionCode);
    if (_backFromGameScene && UserData::getInstance()->isEnableShowRatingPopup())
        MessageSender::requestCheckShowRatingPopup(VALUE_SHOW_RATING_AT_LOBBY);
    MessageSender::requestVideoAdsMoney();
    MessageSender::requestLogInBonus();
    MessageSender::requestVideoRewardEnable();
    if (UserData::getInstance()->isEnableLocalNotification())
        MessageSender::requestLocalNotificationData();

    AdsManager::getInstance()->initAds();
    if (!CallToOtherLanguage::getInstance()->checkInternet())
        CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_1));
    else
        AdsManager::getInstance()->fetchRewardedAds();
}

void LobbyScene::onExit()
{
    InterfaceScene::onExit();
    MessageHandler::getInstance()->unregisterHandleMessage(this);
    NotificationManager::getInstance()->unregisterNotification(this);
}

void LobbyScene::onLoadResourcesSyncDone()
{
}

void LobbyScene::onLoadResourcesAsyncDone()
{
}

SceneID LobbyScene::getSceneID()
{
    return SceneID::LOBBY_SCENE;
}

void LobbyScene::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_PROFILE_RESPONSE:
        {
            this->onProfileInit(data);
            break;
        }
        case MessageID::MESSAGE_USER_DATA_RESPONSE:
        {
            this->onUserData(data);
            break;
        }
        case MessageID::MESSAGE_UNLOCK_CITY_RESPONSE:
        {
            this->onUnlockCity(data);
            break;
        }
        case MessageID::MESSAGE_UNLOCK_TABLE_RESPONSE:
        {
            this->onUnlockTable(data);
            break;
        }
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
        case MessageID::MESSAGE_TICKET_RESPONSE:
        {
            this->onTicket(data);
            break;
        }
        case MessageID::MESSAGE_JOIN_GAME_RESPONSE:
        {
            this->onJoinGame(data);
            break;
        }
        case MessageID::MESSAGE_BONUS_RESPONSE:
        {
            this->onGoldBonus(data);
            break;
        }
        case MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE:
        {
            this->onSaviorGoldBonus(data);
            break;
        }
        case MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE:
        {
            this->onSaviorGoldBonusPopup(data);
            break;
        }
        case MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE:
        {
            onVideoAdsMoney(data);
            break;
        }
        case MessageID::MESSAGE_LOG_IN_OFFLINE_RESPONSE:
        {
            onLogInBonus(data);
            break;
        }
        case MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE:
        {
            onHourlyBonusData(data);
            break;
        }
        case MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE:
        {
            onCheckRatingPopup(data);
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE:
        {
            onVideoRewardEnable(data);
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE:
        {
            onVideoRewardInfo(data);
            break;
        }
        case MessageID::MESSAGE_SUGGEST_UPDATE_RESPONSE:
        {
            onSuggestUpdate(data);
            break;
        }
        case MessageID::MESSAGE_MAILBOX_RESPONSE:
        {
            onMailbox(data);
            break;
        }
        case MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE:
        {
            onClaimMailboxReward(data);
            break;
        }
        case MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE:
        {
            onGetLocalNotificationData(data);
            break;
        }
        default:
            break;
    }
}

void LobbyScene::onVideoAdsMoney(const rapidjson::Document& data)
{
    double money = data[KEY_ADS_VIEW_MONEY].GetDouble();
    _textBMFreeGold->setString(StringUtils::format("+%s", GameUtils::MoneyFormat(money, "").c_str()));
}

void LobbyScene::onProfileInit(const rapidjson::Document& data)
{
    // Name
    UserData::getInstance()->setUserName(JsonHelper::getString(data, KEY_NAME, ""));
    _textBMNameUser->setString(UserData::getInstance()->getUserName().c_str());
    // Money
    UserData::getInstance()->setMoney(JsonHelper::getDouble(data, KEY_MONEY, 0));
    MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, 0, UserData::getInstance()->getMoney(), "", "");

    // Avatar Link
    UserData::getInstance()->setAvatarLink(JsonHelper::getString(data, KEY_AVATAR, ""));
    Avatar* avatar = Avatar::create();
    avatar->setFrameUrl(UserData::getInstance()->getAvatarLink(), _layoutAvatarUser->getContentSize());
    avatar->setTag(AVATAR_TAG);
    avatar->setPosition(Point(_layoutAvatarUser->getContentSize().width / 2, _layoutAvatarUser->getContentSize().height / 2));
    _layoutAvatarUser->addChild(avatar, -1);

    //Level
    UserData::getInstance()->setLevel(JsonHelper::getInt(data, KEY_LEVEL, 0));
    _textUserLevel->setString(StringUtils::format("Lv. %d", UserData::getInstance()->getLevel()));

    UserData::getInstance()->setLevelExp(JsonHelper::getInt(data, KEY_LEVELEXP, 0));
    UserData::getInstance()->setNextLevelExp(JsonHelper::getInt(data, KEY_NEXTLEVELEXP, 0));

    _sliderUserLevel->setPercent(UserData::getInstance()->getLevelExp() * 100.0f / UserData::getInstance()->getNextLevelExp());

    _listPlace.clear();
    const rapidjson::Value& listPlace = JsonHelper::getValue(data, KEY_LIST_PLACES);
    float             delayTimeOfLayoutChoosePlace = 0.5f;
    for (unsigned int i                            = 0; i < listPlace.Size(); i++)
    {
        const rapidjson::Value& placeVal = listPlace[i];
        int id = JsonHelper::getInt(placeVal, KEY_PLACE_ID, 0);
        // Init Places
        Layout* layoutEntireCity = nullptr;
        if (id == (int) CityType::CLASSIC_GIN)
            layoutEntireCity = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_1"));
        else if (id == (int) CityType::STRAIGHT_GIN)
            layoutEntireCity = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_2"));
        else if (id == (int) CityType::OKLAHOMA_GIN)
            layoutEntireCity = static_cast<Layout*>(Helper::seekWidgetByName(_layoutChoosePlace, "PanelRoom3_3"));

        if (layoutEntireCity)
        {
            layoutEntireCity->setVisible(true);
            LobbyPlace* place = new LobbyPlace(this, layoutEntireCity);
            place->initValue(id,
                             (LobbyPlace::State) JsonHelper::getInt(placeVal, KEY_PLACE_STATUS, 0),
                             JsonHelper::getString(placeVal, KEY_PLACE_NAME, ""),
                             JsonHelper::getInt(placeVal, KEY_PLACE_PRICE, 1.0f),
                             JsonHelper::getDouble(placeVal, KEY_PLACE_JACKPOT_RATIO_BET, 0),
                             JsonHelper::getDouble(placeVal, KEY_PLACE_REQUIRED_MONEY, 0));

            _listPlace.push_back(place);

            layoutEntireCity->stopAllActions();
            layoutEntireCity->setScaleX(0);
            layoutEntireCity->runAction(Sequence::create(DelayTime::create(delayTimeOfLayoutChoosePlace), EaseExponentialOut::create(ScaleTo::create(0.6f, 1.0f)), nullptr));
            delayTimeOfLayoutChoosePlace += 0.125f;
            layoutEntireCity->runAction(Sequence::create(DelayTime::create(1.2f),
                                                         EaseExponentialOut::create(ScaleTo::create(0.3f, 0.95f)),
                                                         EaseExponentialOut::create(ScaleTo::create(0.3f, 1.0f)),
                                                         nullptr));
        }
    }

    LobbyPlace::setListPlace(_listPlace);
    const rapidjson::Value& listDataTable = JsonHelper::getValue(data, KEY_LIST_TABLES);
    for (unsigned int i = 0; i < listDataTable.Size(); i++)
    {
        const rapidjson::Value& dataTable = listDataTable[i];
        if (JsonHelper::getInt(dataTable, KEY_TABLE_ID, 0) == UserData::getInstance()->getIdTableSelected())
            UserData::getInstance()->setLuckyValue(JsonHelper::getDouble(dataTable, KEY_TABLE_BONUS, 0) * 100);
    }

    auto notifyShop = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonShop, "ImageNotify"));
    notifyShop->setVisible(false);

    for (unsigned int i = 0; i < listDataTable.Size(); i++)
    {
        const rapidjson::Value& dataTable = listDataTable[i];
        if (JsonHelper::getInt(dataTable, KEY_TABLE_STATUS, 0) == 0 && UserData::getInstance()->getMoney() >= JsonHelper::getInt(dataTable, KEY_TABLE_PRICE, 0))
        {
            showShopNotify(true);
            break;
        }
    }
    _listDataStr = JsonHelper::convertDocumentToString(data);
    _listMailStr = JsonHelper::convertValueToString(JsonHelper::getValue(data, KEY_MAILBOX_LIST, rapidjson::Value(kNullType)));
    checkNewMail();

    // Load Gamecenter data
    GameCenterPopup::loadItemsData();

    // Check icon notification of game center
    auto notifyGameCenter = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonGameCenter, "ImageNotify"));
    notifyGameCenter->setVisible(GameCenterPopup::isNew());
    // Check to show which game should be promoted at lobby
    int      countPromoteGame = 0;
    for (int i                = 0; i < GameCenterPopup::_listGameItemsData.size(); i++)
    {
        GameCenterData gameCenterData = GameCenterPopup::_listGameItemsData.at(i);
        if (gameCenterData._isHighlight)
        {
            SpriteFrame* spriteFrame = nullptr;
            if (!GameUtils::validateURL(gameCenterData._avatarUrl))
            {
                spriteFrame     = SpriteFrameCache::getInstance()->getSpriteFrameByName(gameCenterData._avatarUrl);
                if (spriteFrame == nullptr)
                    spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(Resources::PNG::IconGameDefault);
            }

            if (GameUtils::validateURL(gameCenterData._avatarUrl) || spriteFrame != nullptr)
            {
                Button* buttonPromoteGame = nullptr;
                if (!_buttonPromoteGame1->isVisible())
                    buttonPromoteGame = _buttonPromoteGame1;
                else if (!_buttonPromoteGame2->isVisible())
                    buttonPromoteGame = _buttonPromoteGame2;

                if (buttonPromoteGame != nullptr)
                {
                    Sprite* spritePromoteGame = (Sprite*) buttonPromoteGame->getChildByName("SpriteGame");
                    if (spriteFrame != nullptr)
                    {
                        spritePromoteGame->setSpriteFrame(spriteFrame);
                        spritePromoteGame->setScaleX(buttonPromoteGame->getContentSize().width / spritePromoteGame->getContentSize().width);
                        spritePromoteGame->setScaleY(buttonPromoteGame->getContentSize().height / spritePromoteGame->getContentSize().height);
                    }
                    else
                    {
                        DownloadManager::getInstance()->downloadImage(spritePromoteGame, gameCenterData._avatarUrl, Resources::PNG::IconGameDefault, buttonPromoteGame->getContentSize());
                    }
                    buttonPromoteGame->setName(gameCenterData._packageID);
                    buttonPromoteGame->setVisible(true);
                    countPromoteGame++;
                }
            }
        }
    }
    UserData::getInstance()->setInterferingAdsEnable(JsonHelper::getBool(data, KEY_INTERFERING_ADS, true));
    Firebase::logUserProperties(s_versionName, GameUtils::getMoneyRangeName(UserData::getInstance()->getMoney()), s_winNumber, s_loseNumber, s_winHitpotNumber, UserData::getInstance()->getLevel());
    CallToOtherLanguage::getInstance()->submitLeaderboardMoney(UserData::getInstance()->getMoney());
    CallToOtherLanguage::getInstance()->submitLeaderboardLevel(UserData::getInstance()->getLevel());
}

void LobbyScene::onUserData(const rapidjson::Document& data)
{
    double      money  = data[KEY_MONEY].GetDouble();
    std::string name   = data[KEY_NAME].GetString();
    std::string avatar = data[KEY_AVATAR].GetString();

    double oldMoney = UserData::getInstance()->getMoney();
    UserData::getInstance()->setMoney(money);
    UserData::getInstance()->setInterferingAdsEnable(JsonHelper::getBool(data, KEY_INTERFERING_ADS, true));
    MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");
}

void LobbyScene::onUnlockCity(const rapidjson::Document& data)
{
    int    result       = data[KEY_RESULT].GetInt();
    int    idCity       = data[KEY_PLACE_ID].GetInt();
    double currentMoney = data[KEY_MONEY].GetDouble();
    if (result == RESULT_SUCCESS)
    {
        _listDataStr = JsonHelper::convertDocumentToString(data);

        // Update money user
        double oldMoney = UserData::getInstance()->getMoney();
        UserData::getInstance()->setMoney(currentMoney);
        MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");

        // Unlock city
        LobbyPlace* lobbyPlace = LobbyPlace::getLobbyPlaceByID(idCity);
        if (lobbyPlace)
        {
            lobbyPlace->unlock();
            lobbyPlace->reopenTicket();
            Firebase::logUnlockCity(LobbyPlace::getLobbyPlaceByID(idCity)->getName(), oldMoney);
        }
        MessageSender::requestShowGoldBonusPopup();
    }
    else if (result == RESULT_BUY_FAIL_NOT_ENOUGH_MONEY)
    {
        NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
        notifyPopup->initValue(GET_TEXT_BY_ID(TXT_NOT_ENOUGH_MONEY_TO_BUY), nullptr);
        notifyPopup->attach(this, kAboveForeground);
    }
}

void LobbyScene::onUnlockTable(const rapidjson::Document& data)
{
    int    result  = data[KEY_RESULT].GetInt();
    int    tableID = data[KEY_TABLE_ID].GetInt();
    double money   = data[KEY_MONEY].GetDouble();
    if (result == RESULT_SUCCESS)
    {
        _listDataStr = JsonHelper::convertDocumentToString(data);

        // Update money user
        double oldMoney = UserData::getInstance()->getMoney();
        UserData::getInstance()->setMoney(money);
        MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");

        // Update lobby shop table board
        for (int i = 0; i < data[KEY_LIST_TABLES].Size(); i++)
        {
            LobbyTableNew* table = LobbyTableNew::getLobbyTableByID(data[KEY_LIST_TABLES][i][KEY_TABLE_ID].GetInt());
            if (table)
            {
                table->updateDataFromUnlockCityOrTable(data[KEY_LIST_TABLES][i]);
                if (tableID == table->getId())
                    table->unlock();
            }
        }
        MessageSender::requestShowGoldBonusPopup();
        SoundManager::playSoundEffect(SoundEvent::UNLOCK_TABLE);

        Firebase::logUnlockTable(StringUtils::format("%d", tableID), oldMoney);
    }
    else if (result == RESULT_BUY_FAIL_NOT_ENOUGH_MONEY)
    {
        NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
        notifyPopup->initValue(GET_TEXT_BY_ID(TXT_NOT_ENOUGH_MONEY_TO_BUY), nullptr);
        notifyPopup->attach(this, kAboveForeground);
    }
}

void LobbyScene::onChangeUserName(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        std::string name = data[KEY_NAME].GetString();
        UserData::getInstance()->setUserName(name);
        _textBMNameUser->setString(UserData::getInstance()->getUserName().c_str());
    }
}

void LobbyScene::onChangeUserAvatar(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        std::string name = data[KEY_AVATAR].GetString();
        UserData::getInstance()->setAvatarLink(name);
        ((Avatar*) _layoutAvatarUser->getChildByTag(AVATAR_TAG))->setFrameUrl(UserData::getInstance()->getAvatarLink(), _layoutAvatarUser->getContentSize());
    }
}

void LobbyScene::onTicket(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        int    city         = data[KEY_PLACE_ID].GetInt();
        double minBet       = data[KEY_MIN_BET].GetDouble();
        double maxBet       = data[KEY_MAX_BET].GetDouble();

        LobbyTicketFlightBoard* lobbyTicketFlightBoard = (LobbyTicketFlightBoard*) getChildByTag((int) BoardID::LOBBY_TICKETFLIGHT_BOARD);
        if (lobbyTicketFlightBoard != nullptr)
            lobbyTicketFlightBoard->close();
        lobbyTicketFlightBoard = BoardManager::createLobbyTicketFlightBoard();
        lobbyTicketFlightBoard->initValue((CityType) city, minBet + (maxBet - minBet) * 0.5f, 1.0f);
        lobbyTicketFlightBoard->setColor(Color4B(0, 0, 0, 0));
        lobbyTicketFlightBoard->setLobbyScene(this);
        lobbyTicketFlightBoard->attach(this, kAboveForeground);
    }
    else
    {
        NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
        notifyPopup->initValue(fmt::format(GET_TEXT_BY_ID(TXT_NOT_ENOUGH_MONEY_TO_JOIN), GameUtils::MoneyFormat(data[KEY_JACKPOT].GetDouble(), "$")), nullptr);
        notifyPopup->attach(this, kAboveForeground);
    }
}

void LobbyScene::onJoinGame(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        cocos2d::ValueMapIntKey data2;
        data2[DATA_CITY_TYPE_INDEX]          = JsonHelper::getInt(data, KEY_PLACE_ID, 1);
        data2[DATA_TABLE_TYPE_INDEX]         = JsonHelper::getInt(data, KEY_TABLE_ID, 1);
        data2[DATA_BET_MONEY_INDEX]          = JsonHelper::getDouble(data, KEY_BET, 1);
        data2[DATA_TARGET_SCORE_INDEX]       = JsonHelper::getInt(data, KEY_TARGET_SCORE, 0);
        data2[DATA_MAX_MULTIPLY_VALUE_INDEX] = JsonHelper::getDouble(data, KEY_MAX_MULTIPLY_VALUE, 1);
        SceneManager::getInstance()->openScene(SceneID::INGAME_GIN_SCENE, cocos2d::Value(data2));
    }
}

void LobbyScene::onGoldBonus(const rapidjson::Document& data)
{
    double money    = data[KEY_MONEY].GetDouble();
    double adsMoney = data[KEY_ADS_VIEW_MONEY].GetDouble();
    onVideoAdsMoney(data);
    // Update Gold
    double oldMoney = UserData::getInstance()->getMoney();
    UserData::getInstance()->setMoney(money);
    MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");
    SoundManager::playSoundEffect(SoundEvent::CLAIM_MONEY);
    s_watchVideoCount++;
    Firebase::logGetMoneyWatchVideo("lobby", s_watchVideoCount, money - oldMoney);

    s_totalWatchVideoCount++;
    UserData::getInstance()->saveWatchVideoNumber();
    Firebase::logTotalWatchVideoNumber(s_totalWatchVideoCount);

    Node* node = getChildByTag((int) BoardID::GET_MONEY_TO_PLAY_BOARD);
    if (node != nullptr && node->getParent() != nullptr)
    {
        GetMoneyToPlayPopup* Popup = (GetMoneyToPlayPopup*) node;
        if (Popup->getParent() != nullptr)
        {
            Popup->setEnableViewAds(true);
            Popup->setTextOfViewAds(GameUtils::MoneyFormat(adsMoney, "+"));
        }
    }
}

void LobbyScene::onSaviorGoldBonus(const rapidjson::Document& data)
{
    double money    = data[KEY_MONEY].GetDouble();
    double oldMoney = UserData::getInstance()->getMoney();
    UserData::getInstance()->setMoney(money);
    MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");
    SoundManager::playSoundEffect(SoundEvent::CLAIM_MONEY);
    CallFunc* show = CallFunc::create([=] {
        if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
            AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, INTERSTITIAL_ADMOB_GET_MONEY_DEFAULT);
    });
    Sequence* seq  = Sequence::create(DelayTime::create(1.0), show, nullptr);
    runAction(seq);
    Firebase::logGetMoneyDefault("lobby", money - oldMoney);
}

void LobbyScene::onSaviorGoldBonusPopup(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == 1)
    {
        GetMoneyToPlayPopup* getMoneyToPlayPopup = BoardManager::createGetMoneyToPlayPopupBoard();
        getMoneyToPlayPopup->attach(this, kAboveForeground);
        getMoneyToPlayPopup->setTag((int) BoardID::GET_MONEY_TO_PLAY_BOARD);

        const rapidjson::Value& listNode = data[KEY_SAVIOR_LIST];
        for (int i = 0; i < listNode.Size(); i++)
        {
            const rapidjson::Value& node = listNode[i];
            int type = node[KEY_SAVIOR_TYPE].GetInt();

            int    status = node[KEY_SAVIOR_STATUS].GetInt();
            double money  = node[KEY_FREE_MONEY].GetDouble();
            switch (type)
            {
                case SAVIOR_OPTION_FREE_GOLD:
                {
                    if (status == 1)
                        getMoneyToPlayPopup->setEnableFreeGold(true);
                    else
                        getMoneyToPlayPopup->setEnableFreeGold(false);
                    getMoneyToPlayPopup->setTextOfGetGold(GameUtils::MoneyFormat(money, "+"));
                    break;
                }
                case SAVIOR_OPTION_VIDEO_ADS:
                {
                    bool   isVideoRewardEnabled = node[KEY_VIDEO_REWARDS_ENABLE].GetBool();
                    double availableAt          = JsonHelper::getDouble(node, KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, 0);
                    if (isVideoRewardEnabled)
                    {
                        getMoneyToPlayPopup->onVideoRewardEnable(isVideoRewardEnabled, availableAt);
                    }
                    else
                    {
                        getMoneyToPlayPopup->setEnableViewAds(status == 1);
                        getMoneyToPlayPopup->setTextOfViewAds(GameUtils::MoneyFormat(money, "+"));
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void LobbyScene::onLogInBonus(const rapidjson::Document& data)
{
    int    result         = data[KEY_RESULT].GetInt();
    double money          = data[KEY_MONEY].GetDouble();
    double bonusMoney     = data[KEY_BONUS_MONEY].GetDouble();
    double nextBonusMoney = data[KEY_NEXT_BONUS_MONEY].GetDouble();
    if (result != 0)
    {
        if (result == 1) //daily bonus
        {
            DailyBonusPopup* bonus = DailyBonusPopup::create();
            bonus->setContent(money, bonusMoney, nextBonusMoney, [=](double money, double bonusMoney) {
                int oldMoney = UserData::getInstance()->getMoney();
                UserData::getInstance()->setMoney(money);
                MyActionsManager::getInstance()->createActionMoneyChange(_textBMMoneyUser, 1.0f, 100, oldMoney, UserData::getInstance()->getMoney(), "", "");
                // Show admob
                if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
                    AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, INTERSTITIAL_ADMOB_GET_MONEY_BONUS);
                if (UserData::getInstance()->isEnableSuggestUpdate())
                    MessageSender::requestCheckSuggestUpdate();
            });
            bonus->show(this);
        }
        else if (result == 2) //welcome back
        {
            RewardPopup* notifyPopup = BoardManager::createRewardPopup();
            notifyPopup->attach(this, kAboveForeground);
            notifyPopup->setContent(GET_TEXT_BY_ID(TXT_WELCOME_BACK));
            notifyPopup->setRewardContent(GameUtils::MoneyFormat(bonusMoney, "+"));
            notifyPopup->setClaimRewardCallback([=]() {
                MessageSender::requestUserData();
                if (UserData::getInstance()->isEnableSuggestUpdate())
                    MessageSender::requestCheckSuggestUpdate();
            });

            Firebase::logGetMoneyWelcomeBack("lobby", JsonHelper::getDouble(data, KEY_WELCOME_BACK_DAY, 0), bonusMoney);
        }
        else if (result == 3) //startup
        {
            RewardPopup* notifyPopup = BoardManager::createRewardPopup();
            notifyPopup->attach(this, kAboveForeground);
            notifyPopup->setContent(fmt::format(GET_TEXT_BY_ID(TXT_WELCOME_STARTUP), GameUtils::getGameName().c_str()));
            notifyPopup->setRewardContent(GameUtils::MoneyFormat(bonusMoney, "+"));
            notifyPopup->setClaimRewardCallback([=]() {
                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
                    ChangeAvatarPopup* notifyPopup = BoardManager::createChangeAvatarPopup();
                    notifyPopup->initValue();
                    notifyPopup->attach(this->getParent(), kAboveForeground);
                }), nullptr));
                MessageSender::requestUserData();
            });
        }
    }
    else
    {
        if (!this->_ratingCondition.empty())
        {
            this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=] {
                GameUtils::showRatingAndReview(this->_ratingState, this->_ratingCondition);
                this->_ratingCondition = "";
                this->_ratingState     = 0;
            }), nullptr));
        }
        else
        {
            bool showNews = NewsPopup::showNewsPopup(this, false);
            if (!showNews && UserData::getInstance()->isEnableSuggestUpdate())
                MessageSender::requestCheckSuggestUpdate();
        }
    }
}

void LobbyScene::onHourlyBonusData(const rapidjson::Document& data)
{
    double money      = data[KEY_MONEY].GetDouble();
    double bonusMoney = data[KEY_FREE_MONEY].GetDouble();
    double countDown  = data[KEY_COUNT_DOWN_TIME].GetDouble();
    int    result     = data[KEY_RESULT].GetInt();

    this->_hourlyBonusCountDown = countDown;
    this->_hourlyBonusMoney     = bonusMoney;
    //
    if (result == 1)
    {
        double oldMoney = UserData::getInstance()->getMoney();
        RewardPopup* notifyPopup = BoardManager::createRewardPopup();
        notifyPopup->attach(this, kAboveForeground);
        notifyPopup->setContent(GET_TEXT_BY_ID(TXT_GET_BONUS_MONEY));
        notifyPopup->setRewardContent(GameUtils::MoneyFormat(money - oldMoney, "+"));
        notifyPopup->setClaimRewardCallback([=]() {
            MessageSender::requestUserData();
            Action* action1 = Sequence::create(CallFunc::create(CC_CALLBACK_0(LobbyScene::setHourlyBonusButtonState,
                                                                              this,
                                                                              2,
                                                                              this->_hourlyBonusCountDown - GameUtils::getCurrentTime(),
                                                                              this->_hourlyBonusMoney)), DelayTime::create(2.0f), (FiniteTimeAction*) createMoveHourlyBonusButton(1), nullptr);
            action1->setTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
            this->_buttonHourlyBonus->stopActionByTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
            this->_buttonHourlyBonus->runAction(action1);
            if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
                AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, "");
            if (UserData::getInstance()->isEnableLocalNotification())
                MessageSender::requestLocalNotificationData();
        });

        Action* action = Sequence::create((FiniteTimeAction*) createMoveHourlyBonusButton(0), nullptr);
        action->setTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
        this->_buttonHourlyBonus->stopActionByTag(TAG_ACTION_MOVE_BUTTON_HOURLY_BONUS);
        this->_buttonHourlyBonus->runAction(action);
    }
    else
    {
        if (this->_hourlyBonusCountDown - GameUtils::getCurrentTime() <= 0)
            setHourlyBonusButtonState(1, this->_hourlyBonusCountDown - GameUtils::getCurrentTime(), bonusMoney);
        else
            setHourlyBonusButtonState(2, this->_hourlyBonusCountDown - GameUtils::getCurrentTime(), bonusMoney);
    }
}

void LobbyScene::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonSetting)
    {
        LobbySettingBoard* lobbySettingBoard = BoardManager::createLobbySettingBoard();
        lobbySettingBoard->initValue(0);
        lobbySettingBoard->attach(this, kAboveForeground);
    }
    else if (button == _buttonRating)
    {
        Firebase::logOpenAppRating();
        CallToOtherLanguage::getInstance()->openAppInStore();
    }
    else if (button == _buttonFreeGold)
    {
        bool adsIsAvailable = AdsManager::getInstance()->isRewardedAdsAvailable();
        if (adsIsAvailable)
        {
            Firebase::logOpenWatchVideo("available");
            AdsManager::getInstance()->showRewardedAds(VideoAdsRequestSourceEnum::OLD_VIDEO_REWARD);
        }
        else
        {
            if (!CallToOtherLanguage::getInstance()->checkInternet())
            {
                Firebase::logOpenWatchVideo("no internet");
                CCLOG("No Network");
                CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_1));
            }
            else
            {
                AdsManager::getInstance()->fetchRewardedAds();
                Firebase::logOpenWatchVideo("not available");
                CCLOG("No Available");
                CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_2));
            }
        }
    }
    else if (button == _buttonHelp)
    {
        Firebase::logOpenTutorial();
        this->showTutorial();
    }
    else if (button == _buttonShop)
    {
        openShopTable();
    }
    else if (button == _buttonHourlyBonus)
    {
        MessageSender::requestHourlyBonus();
    }
    else if (button == _buttonPromoteGame1)
    {
        GameCenterData data = GameCenterPopup::getGameCenterData(button->getName());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (CallToOtherLanguage::getInstance()->isPackageInstalled(data._packageID) || data._link.empty())
            CallToOtherLanguage::getInstance()->openAnotherAppInStore(button->getName());
        else
            CallToOtherLanguage::getInstance()->openAnotherAppInStoreByLink(data._link);
#endif
        Firebase::logOpenApp(button->getName());
    }
    else if (button == _buttonPromoteGame2)
    {
        GameCenterData data = GameCenterPopup::getGameCenterData(button->getName());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (CallToOtherLanguage::getInstance()->isPackageInstalled(data._packageID) || data._link.empty())
            CallToOtherLanguage::getInstance()->openAnotherAppInStore(button->getName());
        else
            CallToOtherLanguage::getInstance()->openAnotherAppInStoreByLink(data._link);
#endif
        Firebase::logOpenApp(button->getName());
    }
    else if (button == _buttonLeaderboard)
    {
        openLeaderboard();
    }
    else if (button == _buttonMailbox)
    {
        MessageSender::requestMailbox();
    }
    else if (button == _buttonGameCenter)
    {
        openGameCenter();
    }
    else if (button == _buttonVideoBonusReward)
    {
        MessageSender::requestVideoRewardInfo();
    }
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void LobbyScene::onTouchLayoutPlaceListener(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
    Layout* layout = (Layout*) sender;
    switch (eventType)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            for (int i = 0; i < _listPlace.size(); i++)
                if (_listPlace[i]->checkTouchCity(layout))
                    _listPlace[i]->touchBegin();
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            for (int i = 0; i < _listPlace.size(); i++)
                if (_listPlace[i]->checkTouchCity(layout))
                    _listPlace[i]->touchEnd();
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            for (int i = 0; i < _listPlace.size(); i++)
                if (_listPlace[i]->checkTouchCity(layout))
                    _listPlace[i]->playActionTouchEnd();
            break;
        default:
            break;
    }
}

void LobbyScene::onLayoutClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
    Layout* layout = (Layout*) sender;
    if (layout == _layoutInfoUser || layout == _layoutAvatarUser)
    {
        layout->stopAllActions();
        if (cocos2d::ui::Widget::TouchEventType::BEGAN == eventType)
        {
            auto scaleTo = ScaleTo::create(0.1f, 0.95f);
            layout->runAction(EaseExponentialOut::create(scaleTo));
        }
        else if (cocos2d::ui::Widget::TouchEventType::ENDED == eventType)
        {
            auto scaleTo = ScaleTo::create(0.1f, 1.0f);
            layout->runAction(Sequence::create(EaseExponentialOut::create(scaleTo), CallFunc::create([=]() {
                LobbyInfoUserBoard* lobbyInfoUserBoard = BoardManager::createLobbyInfoUserBoard();
                lobbyInfoUserBoard->initValue(UserData::getInstance()->getUserName(), Avatar::create());
                lobbyInfoUserBoard->attach(this, kAboveForeground);
                SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
            }), nullptr));
        }
        else if (cocos2d::ui::Widget::TouchEventType::CANCELED == eventType)
        {
            auto scaleTo = ScaleTo::create(0.1f, 1.0f);
            layout->runAction(EaseExponentialOut::create(scaleTo));
        }
    }
}

void LobbyScene::setHourlyBonusButtonState(int state, double countdown, double money)
{
    this->stopActionByTag(TAG_ACTION_COUNTDOWN_HOURLY_BONUS);
    _buttonHourlyBonus->getChildByName("PanelParticle")->setVisible(false);
    if (state == 0) //wait time
    {
        _buttonHourlyBonus->setCascadeColorEnabled(false);
        _buttonHourlyBonus->setEnabled(false);

        _textHourlyBonusGetNow->setVisible(false);
        _textHourlyBonusMoney->setVisible(false);
        _textHourlyBonusTitle->setVisible(true);
        _imgHourlyBonusGet->setVisible(false);
        _textHourlyBonusCountDown->setVisible(true);


        _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(countdown));
        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));
    }
    else if (state == 1) //get now
    {
        _buttonHourlyBonus->getChildByName("PanelParticle")->setVisible(true);
        _buttonHourlyBonus->setCascadeColorEnabled(true);
        _buttonHourlyBonus->setEnabled(true);

        _textHourlyBonusGetNow->setVisible(true);
        _textHourlyBonusMoney->setVisible(true);
        _textHourlyBonusTitle->setVisible(false);
        _imgHourlyBonusGet->setVisible(true);
        _textHourlyBonusCountDown->setVisible(false);

        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));

        auto scaleBy = ScaleBy::create(0.2f, 1.1f);
        _buttonHourlyBonus->runAction(Sequence::create(EaseBackOut::create(scaleBy), scaleBy->reverse(), nullptr));
    }
    else if (state == 2) //wait time
    {
        _buttonHourlyBonus->setCascadeColorEnabled(true);
        _buttonHourlyBonus->setEnabled(false);

        _textHourlyBonusGetNow->setVisible(false);
        _textHourlyBonusMoney->setVisible(true);
        _textHourlyBonusTitle->setVisible(true);
        _imgHourlyBonusGet->setVisible(false);
        _textHourlyBonusCountDown->setVisible(true);

        _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(countdown));
        _textHourlyBonusMoney->setString(GameUtils::MoneyFormat(money, "+"));

        /*Delay action*/
        DelayTime    * delay         = DelayTime::create(1);
        CallFunc     * delayCallback = CallFunc::create([=]() {
            if (this->_hourlyBonusCountDown - GameUtils::getCurrentTime() >= 0)
                _textHourlyBonusCountDown->setString(GameUtils::getTimeTextFromNumber(this->_hourlyBonusCountDown - GameUtils::getCurrentTime()));
            else
                setHourlyBonusButtonState(1, 0, money);
        });
        RepeatForever* repeatAction  = RepeatForever::create(Sequence::create(delay, delayCallback, nullptr));
        Action       * action        = repeatAction;
        action->setTag(TAG_ACTION_COUNTDOWN_HOURLY_BONUS);
        this->runAction(action);
    }
}

Action* LobbyScene::createMoveHourlyBonusButton(int state)
{
    Point srcPos;
    Point dstPos;
    int   opacity = 0;
    if (state == 0)
    {
        srcPos = this->_posButtonHourlyBonus;
        dstPos = this->_posButtonHourlyBonus + Point(this->_buttonHourlyBonus->getContentSize().width + 100, 0);
        this->_buttonHourlyBonus->setPosition(srcPos);

        opacity = 0;
    }
    else if (state == 1)
    {
        dstPos  = this->_posButtonHourlyBonus;
        opacity = 255;
    }
    MoveTo       * moveAction      = MoveTo::create(0.5f, dstPos);
    EaseBackInOut* moveAccelAction = EaseBackInOut::create(moveAction);
    return Sequence::create(DelayTime::create(0.2f), Spawn::create(moveAccelAction, FadeTo::create(0.5f, opacity), nullptr), nullptr);
}

void LobbyScene::showTutorial()
{
    auto tutorialView = TutorialView::create();
    tutorialView->setContent({ RuleDetails::create(), RuleScore::create() });
    addChild(tutorialView, kAboveForeground);
}

void LobbyScene::showShopNotify(bool show)
{
    auto notifyShop = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonShop, "ImageNotify"));
    if (show)
    {
        notifyShop->setVisible(true);
        ScaleTo* scaleUp   = ScaleTo::create(1.0f, 0.8f);
        ScaleTo* scaleDown = ScaleTo::create(0.1f, 0.4f);
        notifyShop->runAction(Sequence::create(Repeat::create(Sequence::create(EaseElasticOut::create(scaleUp), scaleDown, nullptr), 2), ScaleTo::create(0.1f, 0.8f), nullptr));
    }
    else
    {
        notifyShop->setVisible(false);
    }
}

void LobbyScene::onNotify(void* sender, int id, const cocos2d::Value& data)
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
        case NOTIFY_USERDATA_CHANGE_MONEY:
        {
            if (_listDataStr != "")
            {
                const rapidjson::Document& dataDoc       = JsonHelper::parseStringToJson(_listDataStr);
                const rapidjson::Value   & listDataTable = JsonHelper::getValue(dataDoc, KEY_LIST_TABLES);
                auto notifyShop = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonShop, "ImageNotify"));
                showShopNotify(false);
                if (dataDoc.HasMember(KEY_LIST_TABLES) == true && notifyShop->isVisible() == false)
                {
                    for (unsigned int i = 0; i < listDataTable.Size(); i++)
                    {
                        const rapidjson::Value& dataTable = listDataTable[i];
                        if (JsonHelper::getInt(dataTable, KEY_TABLE_STATUS, 0) == 0 && UserData::getInstance()->getMoney() >= JsonHelper::getInt(dataTable, KEY_TABLE_PRICE, 0))
                        {
                            showShopNotify(true);
                            break;
                        }
                    }
                    for (unsigned int i = 0; i < listDataTable.Size(); i++)
                    {
                        LobbyTableNew* table = LobbyTableNew::getLobbyTableByID(dataDoc[KEY_LIST_TABLES][i][KEY_TABLE_ID].GetInt());
                        if (table)
                            table->updateDataFromUnlockCityOrTable(dataDoc[KEY_LIST_TABLES][i]);
                    }
                }
                for (int i = 0; i < _listPlace.size(); i++)
                {
                    if (UserData::getInstance()->getMoney() >= _listPlace[i]->getPrice())
                        _listPlace[i]->showBuyNotify(true);
                    else
                        _listPlace[i]->showBuyNotify(false);
                }
            }
            break;
        }
        case EVENT_PLAYGAME_SIGNIN:
        {
            break;
        }
        case EVENT_PLAYGAME_SIGNOUT:
        {
            UserDefault::getInstance()->setBoolForKey(USERDEFAULT_PLAYGAME_SIGNOUT, true);
            break;
        }
        case EVENT_INTERSTITIAL_HIDE:
        {
            ValueMap    arr          = data.asValueMap();
            std::string nameFunction = arr[AD_NAME_FUNCTION_KEY].asString();
            if (nameFunction == INTERSTITIAL_ADMOB_EXIT_GAME)
            {
                auto quitConfirm = BoardManager::createYesNoConfirmPopup();
                quitConfirm->setContent(GET_TEXT_BY_ID(TXT_EXIT_GAME_CONFIRM));
                quitConfirm->setButtonLabel(GET_TEXT_BY_ID(TXT_BUTTON_EXIT_GAME), GET_TEXT_BY_ID(TXT_YESNOCONFIRM_NO_BUTTON));
                quitConfirm->setCallback([=](YesNoConfirmResult result) {
                    if (result == YesNoConfirmResult::CONFIRM_YES)
                        Director::getInstance()->end();
                    else if (result == YesNoConfirmResult::CONFIRM_NO)
                        quitConfirm->close();
                });
                quitConfirm->attach(getParent(), kAboveForeground);
            }
            break;
        }
        case EVENT_SHOW_CUSTOM_APP_REVIEW:
        {
            cocos2d::ValueMap map       = data.asValueMap();
            int               state     = map[EVENT_KEY_SHOW_RATING_STATE].asInt();
            std::string       condition = map[EVENT_KEY_SHOW_RATING_CONDITION].asString();
            GameUtils::showRatingPopup(this, state, condition);
            break;
        }
        case EVENT_RATING_REVIEW_COMPLETED:
        {
            RatingPopupThank* ratingThankPopup = BoardManager::createRatingThankPopup();
            ratingThankPopup->setType(RatingPopupThank::Type::THANK_HIGH_STAR);
            ratingThankPopup->setCallbackOK(nullptr);
            ratingThankPopup->attach(this, kAboveForeground);
            break;
        }
        default:
            break;
    }
}

void LobbyScene::changeLanguage(Language language)
{
    _textVideoBonusHint->setString(GET_TEXT_BY_ID(TXT_VIDEO_BONUS_TITLE));
    _textHourlyBonusTitle->setString(GET_TEXT_BY_ID(TXT_HOURLY_BONUS_TITLE));
    _textHourlyBonusGetNow->setString(GET_TEXT_BY_ID(TXT_GET_NOW));
    _textGameCenter->setString(GET_TEXT_BY_ID(TXT_MORE_GAME));
    _textBMVideoBonusReward->setString(GET_TEXT_BY_ID(TXT_GET_REWARD));

    auto label = (Text*) _buttonPromoteGame1->getChildByName("Label");
    label->setString(GET_TEXT_BY_ID(TXT_DOWNLOAD));
    label = (Text*) _buttonPromoteGame2->getChildByName("Label");
    label->setString(GET_TEXT_BY_ID(TXT_DOWNLOAD));
}

void LobbyScene::onCheckRatingPopup(const rapidjson::Document& data)
{
    this->_ratingState     = 0;
    this->_ratingCondition = "";
    int result = JsonHelper::getInt(data, KEY_RESULT, 0);
    if (result > 0)
    {
        this->_ratingState     = JsonHelper::getInt(data, KEY_RATING_STATE, 0);
        this->_ratingCondition = JsonHelper::getString(data, KEY_RATING_CONDITION, "");
    }
}

void LobbyScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        case EventKeyboard::KeyCode::KEY_BACK:
            break;
        case EventKeyboard::KeyCode::KEY_A:
            break;
#endif
        default:
            break;
    }
}

void LobbyScene::openShopTable()
{
    if (_listDataStr != "")
    {
        DocumentType data = JsonHelper::parseStringToDocumentType(_listDataStr);
        const ValueType    & listDataTable       = JsonHelper::getValue(data, KEY_LIST_TABLES);
        LobbyShopTableBoard* lobbyShopTableBoard = BoardManager::createLobbyShopTableBoard();
        lobbyShopTableBoard->setLobbyScene(this);
        lobbyShopTableBoard->initValue(listDataTable);
        lobbyShopTableBoard->attach(this, kAboveForeground);
    }
}

void LobbyScene::onVideoRewardEnable(const rapidjson::Document& data)
{
    bool   result      = JsonHelper::getBool(data, KEY_RESULT, false);
    double availableAt = JsonHelper::getDouble(data, KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, 0);
    if (result)
    {
        _buttonFreeGold->setVisible(false);
        _buttonVideoBonusReward->setVisible(true);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();
        _imageVideoBonusReward->setScale(this->_imageVideoBonusRewardScaleX, this->_imageVideoBonusRewardScaleY);
        _imageVideoBonusRewardGlow0->setVisible(false);
        _imageVideoBonusRewardGlow1->setVisible(false);
        if (availableAt >= 0)
        {
            double countDown = std::max((double) 0, availableAt - GameUtils::getCurrentTime());
            this->runAction(Sequence::create(DelayTime::create(countDown), CallFunc::create([=] {
                ScaleTo      * scale1 = ScaleTo::create(0.4f, this->_imageVideoBonusRewardScaleX * 1.1f, this->_imageVideoBonusRewardScaleY * 0.9f);
                ScaleTo      * scale2 = ScaleTo::create(0.4f, this->_imageVideoBonusRewardScaleX * 0.9f, this->_imageVideoBonusRewardScaleY * 1.1f);
                Sequence     * seq    = Sequence::create(scale1, scale2, nullptr);
                RepeatForever* action = RepeatForever::create(seq);
                _imageVideoBonusReward->runAction(action);

                _imageVideoBonusRewardGlow0->setVisible(true);
                _imageVideoBonusRewardGlow1->setVisible(true);
                _imageVideoBonusRewardGlow0->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
                _imageVideoBonusRewardGlow1->runAction(RepeatForever::create(RotateBy::create(20.0f, 360)));
            }), nullptr));
        }
    }
    else
    {
        _buttonFreeGold->setVisible(true);
        _buttonVideoBonusReward->setVisible(false);
        _imageVideoBonusReward->stopAllActions();
        _imageVideoBonusRewardGlow0->stopAllActions();
        _imageVideoBonusRewardGlow1->stopAllActions();
    }

    Node* node = getChildByTag((int) BoardID::GET_MONEY_TO_PLAY_BOARD);
    if (node != nullptr && node->getParent() != nullptr)
    {
        GetMoneyToPlayPopup* popup = dynamic_cast<GetMoneyToPlayPopup*>(node);
        if (popup != nullptr)
            popup->onVideoRewardEnable(result, availableAt);
    }
}

void LobbyScene::onVideoRewardInfo(const rapidjson::Document& data)
{
    VideoRewardsPopup* popup = BoardManager::createVideoRewardsPopup();
    popup->attach(this, kAboveForeground);
    popup->setData(data, true);
}

void LobbyScene::onSuggestUpdate(const rapidjson::Document& data)
{
    if (UserData::getInstance()->isEnableSuggestUpdate())
    {
        int currentVersionCode          = s_versionCode;
        int suggestVersion              = JsonHelper::getInt(data, KEY_SUGGEST_UPDATE_VERSION, 0);
        int currentSuggestUpdateVersion = UserData::getInstance()->getSuggestUpdateVersionCode();
        if (currentSuggestUpdateVersion == -1)
        {
            currentSuggestUpdateVersion = currentVersionCode;
            UserData::getInstance()->setSuggestUpdateVersionCode(currentSuggestUpdateVersion);
        }

        if (suggestVersion > currentVersionCode && suggestVersion > currentSuggestUpdateVersion)
        {
            UserData::getInstance()->setSuggestUpdateVersionCode(suggestVersion);
            std::string contentData = JsonHelper::getString(data, KEY_SUGGEST_UPDATE_CONTENT, "");

            std::string content = "";
            if (contentData != "")
            {
                const rapidjson::Document& contentJson = JsonHelper::parseStringToJson(contentData);
                content = GET_TEXT_BY_JSON(contentJson);
            }

            SuggestUpdatePopup* suggestUpdatePopup = BoardManager::createSuggestUpdatePopup();
            suggestUpdatePopup->initValue(content);
            suggestUpdatePopup->attach(this, kAboveForeground);
        }
    }
}

void LobbyScene::onMailbox(const rapidjson::Document& data)
{
    std::string mailboxStr = JsonHelper::convertDocumentToString(data);
    const rapidjson::Value& mailboxList = JsonHelper::getValue(data, KEY_MAILBOX_LIST);
    MailboxPopup          * mailbox     = (MailboxPopup*) getChildByTag((int) BoardID::MAILBOX_POPUP);
    if (mailbox == nullptr)
    {
        mailbox = BoardManager::createMailboxPopup();
        mailbox->attach(this, kAboveForeground);
    }
    mailbox->initValue(mailboxList, this);
    _listMailStr = JsonHelper::convertValueToString(mailboxList);
}

void LobbyScene::onClaimMailboxReward(const rapidjson::Document& data)
{
    std::string type = JsonHelper::getString(data, KEY_MAILBOX_TYPE, "");
    std::string id   = JsonHelper::getString(data, KEY_MAILBOX_ID, "");

    double bonusMoney = JsonHelper::getDouble(data, KEY_BONUS_MONEY, 0);
    if (type == MAILBOX_TYPE_REWARD && bonusMoney > 0)
    {
        RewardPopup* notifyPopup = BoardManager::createRewardPopup();
        notifyPopup->attach(this, kAboveForeground);
        notifyPopup->setContent(GET_TEXT_BY_ID(TXT_GET_BONUS_MONEY));
        notifyPopup->setRewardContent(GameUtils::MoneyFormat(bonusMoney, "+"));
        notifyPopup->setClaimRewardCallback([=]() {
            MessageSender::requestUserData();
            MailboxPopup* mailbox = (MailboxPopup*) getChildByTag((int) BoardID::MAILBOX_POPUP);
            if (mailbox != nullptr)
            {
                std::vector<std::pair<std::string, std::string>> items;
                items.push_back(make_pair(type, id));
                mailbox->removeMailboxItems(items);
            }
        });
        Firebase::logClaimMailboxReward(id, bonusMoney);
    }
    _listMailStr = JsonHelper::convertValueToString(JsonHelper::getValue(data, KEY_MAILBOX_LIST, rapidjson::Value(kNullType)));
}

void LobbyScene::onGetLocalNotificationData(const rapidjson::Document& data)
{
    int result = JsonHelper::getInt(data, KEY_RESULT, 0);
    if (result == RESULT_SUCCESS)
    {
        const rapidjson::Value& listNotificationData = JsonHelper::getValue(data, KEY_LOCAL_NOTIFICATION_NOTIFICATIONS);
        UserData::getInstance()->parseLocalNotificationData(listNotificationData);
    }
}

void LobbyScene::openGameCenter()
{
    if (UserData::getInstance()->isEnableGameCenterPopup())
    {
        GameCenterPopup* gameCenterPopup = BoardManager::createGameCenterPopup();
        gameCenterPopup->attach(this, kAboveForeground);

        // Turn off notification
        auto notifyGameCenter = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonGameCenter, "ImageNotify"));
        notifyGameCenter->setVisible(GameCenterPopup::isNew());
    }
    else
    {
        CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_GAME_CENTER_IS_MAINTAINING));
    }
}

void LobbyScene::openLeaderboard()
{
    LeaderboardPopup* leaderboardPopup = BoardManager::createLeaderboardPopup();
    leaderboardPopup->attach(this, kAboveForeground);
}

void LobbyScene::checkNewMail()
{
    showMailNotify(LobbyScene::hasNewMail(_listMailStr));
}

void LobbyScene::showMailNotify(bool show)
{
    auto notifyShop = static_cast<ImageView*>(Helper::seekWidgetByName(_buttonMailbox, "ImageNotify"));
    if (show)
    {
        notifyShop->setVisible(true);
        ScaleTo* scaleUp   = ScaleTo::create(1.0f, 0.8f);
        ScaleTo* scaleDown = ScaleTo::create(0.1f, 0.4f);
        notifyShop->runAction(Sequence::create(Repeat::create(Sequence::create(EaseElasticOut::create(scaleUp), scaleDown, nullptr), 2), ScaleTo::create(0.1f, 0.8f), nullptr));
    }
    else
    {
        notifyShop->setVisible(false);
    }
}

bool LobbyScene::hasNewMail(const std::string& jsonData)
{
    if (jsonData != "")
    {
        const rapidjson::Document& mailboxList = JsonHelper::parseStringToJson(jsonData);
        if (mailboxList.IsArray() && mailboxList.Size() > 0)
        {
            bool                     newMail = false;
            std::vector<std::string> listMailIds;
            for (int                 i       = 0; i < mailboxList.Size(); i++)
            {
                const rapidjson::Value& mail = mailboxList[i];
                std::string type = JsonHelper::getString(mail, KEY_MAILBOX_TYPE, "");
                std::string id   = JsonHelper::getString(mail, KEY_MAILBOX_ID, "");
                listMailIds.push_back(type + "_" + id);
                if (type == MAILBOX_TYPE_REWARD)
                    newMail = true;
            }
            if (!newMail)
            {
                std::vector<std::string> listMailRead = UserData::getInstance()->getMailRead();
                newMail                               = false;
                for (int i = 0; i < listMailIds.size(); i++)
                {
                    if (std::find(listMailRead.begin(), listMailRead.end(), listMailIds[i]) == listMailRead.end())
                    {
                        newMail = true;
                        break;
                    }
                }
            }
            if (newMail)
                return true;
        }
    }
    return false;
}

void LobbyScene::checkAndEnableEventDecorLayer()
{
    bool isEnabled = UserData::getInstance()->isEventDecorEnabled();
    _layoutEventDecor->setVisible(isEnabled);
    _layoutRoot->stopActionByTag(TAG_ACTION_DECOR_LAYOUT_TIMELINE);
    if (isEnabled)
    {
        cocostudio::timeline::ActionTimeline* pActionTimeline = cocos2d::CSLoader::createTimeline("ui/LobbyScene/LobbyScene.csb");
        if (pActionTimeline != nullptr)
        {
            pActionTimeline->setTag(TAG_ACTION_DECOR_LAYOUT_TIMELINE);
            pActionTimeline->setTarget(_layoutRoot);
            _layoutRoot->runAction(pActionTimeline);
            pActionTimeline->gotoFrameAndPlay(0, true);
        }
    }
}
