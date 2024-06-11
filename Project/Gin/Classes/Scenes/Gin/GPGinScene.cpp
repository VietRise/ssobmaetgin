#include "GPGinScene.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "Helper/GameUtils.h"
#include "Manager/NotificationManager.h"
#include "Manager/BoardManager.h"
#include "Manager/TextManager.h"
#include "GinSlotInfo.h"
#include "Helper/CardIndexConverter.h"
#include "GPGinEffect.h"
#include "Network/MessageHandler.h"
#include "Native/CallToOtherLanguage.h"
#include "FirebaseGG/Firebase.h"
#include "Network/MessageSender.h"
#include "Helper/JsonHelper.h"
#include "Component/LobbyPlace.h"
#include "Scenes/LobbyScene.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/AdsManager.h"
#include "Component/TutorialView.h"
#include "Component/RuleScore.h"
#include "Component/RuleDetails.h"
#include "Component/ResultScoreBoxNode.h"

#if USE_DEBUG_VIEW == 1

#include "Scenes/DebugLayer.h"

#endif

#if USE_CHEAT_CARD == 1

#include "Scenes/CheatCardLayer.h"

#endif

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace std::placeholders;

#define STARTGAME_FRISTTIME_DELAY_TIME 2.0
#define STARTGAME_DELAY_TIME 0.5f
#define MATCH_COUNT_TO_CHANGE_MUSIC 3
#define STATUS_POSITION_OFFSET cocos2d::Point(0, 65)

bool GPGinScene::_forceContinue = false;

GPGinScene::GPGinScene(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue)
{
    this->_screenSize               = Director::getInstance()->getWinSize();
    this->_canQuit                  = true;
    this->_slotManager              = GPGinSlotManager::getInstance();
    this->_interfaceLayer           = nullptr;
    this->_countToPlaySoundDealCard = 0;
    this->_cityID                   = cityType;
    this->_tableID                  = tableType;
    this->_betMoney                 = betMoney;
    this->_rejoin                   = false;
    this->_conditionRatingName      = "";
    this->_inRound                  = false;
    this->_turnActionOrderIdx       = -1;
    this->_targetScore              = targetScore;
    this->_round                    = 1;
    this->_rule                     = 0;
    this->_multiplyValue            = maxMultiplyValue;
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listBetData.push_back(BetData());
}

GPGinScene::~GPGinScene()
{
}

cocos2d::Scene* GPGinScene::createScene(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue)
{
    auto scene = Scene::create();
    auto layer = GPGinScene::create(cityType, tableType, betMoney, targetScore, maxMultiplyValue);
    layer->initRoom();
    scene->addChild(layer);
    return scene;
}

GPGinScene* GPGinScene::create(CityType cityType, TableType tableType, double betMoney, int targetScore, float maxMultiplyValue)
{
    GPGinScene* gpGinLayer = new GPGinScene(cityType, tableType, betMoney, targetScore, maxMultiplyValue);
    if (gpGinLayer != nullptr && gpGinLayer->init())
    {
        gpGinLayer->autorelease();
        return gpGinLayer;
    }
    CC_SAFE_DELETE(gpGinLayer);
    return nullptr;
}

bool GPGinScene::init()
{
    if (!InterfaceScene::init())
        return false;
    return true;
}

void GPGinScene::onEnter()
{
    InterfaceScene::onEnter();

    registerHandleMessage(MessageID::MESSAGE_BOT_JOIN_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_START_MATCH_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_HAND_CARD_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_BOT_HAND_CARD_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_BEGIN_TURN_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_END_MATCH_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_END_TURN_RESPONSE);

    registerHandleMessage(MessageID::MESSAGE_CHEAT_CARD_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_QUIT_IN_GAME_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_BONUS_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_JOIN_GAME_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_USER_DATA_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_TICKET_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PROFILE_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE);
    registerHandleMessage(MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE);

    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_CHANGE_LANGUAGE, NOTIFYCALLBACK_SELECTOR(this, GPGinScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_INTERSTITIAL_HIDE, NOTIFYCALLBACK_SELECTOR(this, GPGinScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_SHOW_CUSTOM_APP_REVIEW, NOTIFYCALLBACK_SELECTOR(this, GPGinScene::onNotify));
    NotificationManager::getInstance()->registerNotificationWithData(this, EVENT_RATING_REVIEW_COMPLETED, NOTIFYCALLBACK_SELECTOR(this, GPGinScene::onNotify));
    MessageSender::requestVideoAdsMoney();
}

void GPGinScene::onExit()
{
    InterfaceScene::onExit();
    MessageHandler::getInstance()->unregisterHandleMessage(this);
    NotificationManager::getInstance()->unregisterNotification(this);
}

void GPGinScene::initScreen()
{
    _layoutRootScale     = 1.0f;
    _layoutRootCutomSize = _screenSize;
    if (s_resolution_ratio <= RESOLUTION_1_4)
    {
        _layoutRootCutomSize = _layoutRootCutomSize / RESOLUTION_1_4_SCALE;
        _layoutRootScale     = RESOLUTION_1_4_SCALE;
    }
    else if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        _layoutRootCutomSize = _layoutRootCutomSize / RESOLUTION_1_6_SCALE;
        _layoutRootScale     = RESOLUTION_1_6_SCALE;
    }

    setKeypadEnabled(true);
    this->_interfaceLayer = GPGinInterfaceLayer::create();
    this->_interfaceLayer->setGPScene(this);
    this->_interfaceLayer->initLayout(_layoutRootCutomSize, _layoutRootScale);
    this->_interfaceLayer->setVisible(true);

    this->_controlCardLayer = GPGinControlCardLayer::create();
    this->_controlCardLayer->setGameGin(this);
    this->_controlCardLayer->initLayout(_layoutRootCutomSize, _layoutRootScale);
    this->_controlCardLayer->setVisible(true);

    this->_interfaceLayer->addUserHandNode(this->_controlCardLayer->getListUserHandNode());

    auto onDealCard  = std::bind(&GPGinScene::onDealCardCallback, this);
    auto posDealCard = std::bind(&GPGinScene::posDealCardCallback, this);

    this->_dealCardLayer = GPGinDealCardLayer::createLayerForGame(this->_layoutRootCutomSize, _layoutRootScale, Point(0, 0), this->_interfaceLayer->getDealPos(), 0, this->_interfaceLayer->getDrawCardPos(), 0);
    this->_dealCardLayer->setGameGin(this);
    this->_dealCardLayer->setDealCardCallback(nullptr, onDealCard, posDealCard);
    this->_dealCardLayer->setVisible(false);

    _effectLayer = GPGinEffect::create();
    _effectLayer->initLayout(_layoutRootCutomSize, _layoutRootScale, this, this->_controlCardLayer->getLayoutRoot());

    addChild(this->_interfaceLayer, kMiddleground);
    addChild(this->_dealCardLayer, kMiddleground);
    addChild(this->_controlCardLayer, kMiddleground);
    addChild(this->_effectLayer, kMiddleground);

    _effectLayer->setPositionOfTextBonus(this->_interfaceLayer->getPositionTextBonus());

    _controlCardLayer->alignChildrenPostion(_interfaceLayer->getDrawCardPos(), _interfaceLayer->getTakeCardPos(), _interfaceLayer->getButtonShowThrowCards());
    this->_dealCardLayer->setCascadeOpacityEnabled(true);

    scheduleUpdate();
    initGame();

    this->_interfaceLayer->setRoomInfo(_cityID, _tableID, _betMoney, _round, _multiplyValue);
    this->_interfaceLayer->setRoomName(_roomName);

#if (USE_CHEAT_CARD == 1)
    CheatCardLayer::destroyInstance();
    CheatCardLayer::getInstance()->attach(this, 1);
#endif

#if (USE_DEBUG_VIEW == 1)
    DebugLayer::destroyInstance();
    DebugLayer::getInstance()->attach(this);
#endif
    this->changeLanguage(TextManager::getInstance()->getLanguageType());
    waitToStart(STARTGAME_FRISTTIME_DELAY_TIME);
}

GPGinInterfaceLayer* GPGinScene::getInterfaceLayer()
{
    return _interfaceLayer;
}

GPGinControlCardLayer* GPGinScene::getControlCardLayer()
{
    return _controlCardLayer;
}

GPGinEffect* GPGinScene::getGinEffect()
{
    return _effectLayer;
}

CityType GPGinScene::getCityType()
{
    return _cityID;
}

const float& GPGinScene::getLuckyPercent()
{
    return _luckyPercent;
}

void GPGinScene::reset()
{
    _dealCardLayer->reset();
    _interfaceLayer->reset();
    _controlCardLayer->reset();
    _effectLayer->reset();
    _countListDealCard = 0;
    _listDealCards.clear();
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
        _listDealCards.push_back(std::vector<int>());
    _turnActionOrderIdx = -1;
}

void GPGinScene::onLoadResourcesSyncDone()
{
}

void GPGinScene::onLoadResourcesAsyncDone()
{
    this->initScreen();
}

SceneID GPGinScene::getSceneID()
{
    return SceneID::INGAME_GIN_SCENE;
}

void GPGinScene::changeLanguage(Language language)
{
    _interfaceLayer->changeLanguage(language);
    _controlCardLayer->changeLanguage(language);
}

void GPGinScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_1:
        {
            // Deal card
            this->resetDealCards();
            std::vector<int> listSlotOrderIdx = this->_slotManager->getListReadySlotOrderIdx();
            this->_dealCardLayer->setVisible(true);
            this->_dealCardLayer->setListSlotPos(this->_interfaceLayer->getListDealCardPosition(listSlotOrderIdx), 0);
            std::vector<std::vector<GinCardProperty>> listGinCardPropertyAllPlayer = _controlCardLayer->listPositionCardsOnHandAccordingNumberCardsOfAllPlayers(_listDealCards);
            this->_dealCardLayer->setPosDealCardLayer(listGinCardPropertyAllPlayer, GPGinSlotManager::getInstance()->getSlotOrderIdx(_firstIndexDealCard));
            this->_dealCardLayer->playDealCardAnimation();
            break;
        }
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            if (_interfaceLayer != nullptr)
                _interfaceLayer->onQuitKeyBackClick();
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}

void GPGinScene::onDealCardCallback()
{
    if (this->_countToPlaySoundDealCard % 2 == 0)
        SoundManager::playSoundEffect(SoundEvent::DEAL_CARD);
    this->_countToPlaySoundDealCard++;
    _interfaceLayer->setNumDrawCard(_interfaceLayer->getNumDrawCard() - 1, false, true);
}

void GPGinScene::posDealCardCallback()
{
    _dealCardLayer->setVisible(false);
    _controlCardLayer->setVisible(true);
    _controlCardLayer->initListHandCardsAllUser(_listDealCards);
    _controlCardLayer->finishDealCard(_firstIndexDealCard, _upcardValue, /*_controlCardLayer->convertToNodeSpace(*/_interfaceLayer->getLastFakeDrawCardWorldPosition()/*)*/);

    if (this->_cityID == CityType::OKLAHOMA_GIN)
    {
        _interfaceLayer->finishDealCard(0);
    }
    else
    {
        _interfaceLayer->finishDealCard(_knockScoreValue);
    }

    this->_interfaceLayer->runShowScoreTableAnimation();

//    MessageSender::requestDealCardFinish();
//
//    if (this->_cityID == CityType::OKLAHOMA_GIN && _isSpadesDouble == true)
//    {
//        this->_interfaceLayer->playShowExtraRoomInformation(_upcardValue, _isSpadesDouble);
//    }

    this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
        MessageSender::requestDealCardFinish();

        _interfaceLayer->finishDealCard(_knockScoreValue);
        if (this->_cityID == CityType::OKLAHOMA_GIN )
        {
//            if(_isSpadesDouble)
                this->_interfaceLayer->playShowExtraRoomInformation(_upcardValue, _isSpadesDouble);
        }
    }), nullptr));
}

void GPGinScene::initGame()
{
    this->reset();
    prepareTableDeal();

    GinSlotInfo mainSlot;
    mainSlot.copyFrom(UserData::getInstance());
    mainSlot.setReady(true);
    mainSlot.setSlotIdx(MAIN_USER_SLOT_INDEX);
    mainSlot.setMain(true);

    this->_slotManager->clearAllSlot();
    this->_slotManager->addSlot(mainSlot);
    this->_interfaceLayer->setMainSlot(this->_slotManager->getSlotInfo(mainSlot.getSlotIdx()), _controlCardLayer->getLayoutRoot());

    if (UserData::getInstance()->getSoundBackground())
    {
        SoundManager::getInstance()->stopBackgroundMusicAuto();
        SoundManager::getInstance()->playBackgroundMusicAuto();
    }
    else
    {
        GameUtils::checkToRestoreSoundBackground();
    }
    _adsExit = RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_EXIT_GAME_ENABLE, false);
    this->resetShowInterstitialAdmob();

    _interfaceLayer->playShowJackpot();
}

void GPGinScene::showCannotQuitNotify()
{
    removePopup((int) BoardID::NOTIFY_POPUP);
    NotifyPopup* notifyPopup = BoardManager::createNotifyPopup();
    notifyPopup->initValue(GET_TEXT_BY_ID(TXT_CAN_NOT_QUIT_AND_NOTIFY), nullptr);
    notifyPopup->attach(this, kAboveForeground);
}

void GPGinScene::onQuit()
{
    _rejoin = false;
    if (this->_canQuit)
    {
        MessageSender::requestQuitInGame();
        reset();
    }
    else
    {
        showCannotQuitNotify();
    }
}

void GPGinScene::onContinue2()
{
    waitToStart(STARTGAME_DELAY_TIME);
    _interfaceLayer->showLayoutContinue(false);
    _interfaceLayer->showLayoutRewards(false);
    _interfaceLayer->showButtonResult(false);
    prepareTableDeal();
}

void GPGinScene::onContinue(double bet, int targetScore)
{
    if (_canQuit)
    {
        if (bet != _betMoney || targetScore != _targetScore)
        {
            this->_rejoin = true;
            MessageSender::requestQuitInGame();
            MessageSender::requestFlightNow((int) _cityID, (int) _tableID, bet, targetScore);
            Firebase::logContinueToPlay(bet, bet / UserData::getInstance()->getMoney());

            _interfaceLayer->showLayoutContinue(false);
            _interfaceLayer->showLayoutRewards(false);
            _interfaceLayer->showButtonResult(false);
            prepareTableDeal();
        }
        else
        {
            onContinue2();
        }
    }
}

void GPGinScene::onChooseBet()
{
    GPGinScene::_forceContinue = false;
    MessageSender::requestOpenTicket((int) _cityID, (int) _tableID);
}

void GPGinScene::initRoom()
{
    switch (_cityID)
    {
        case CityType::CLASSIC_GIN:
            _slotManager->setNumSlot(2);
            _roomName = GET_TEXT_BY_ID(TXT_ROOM_1);
            break;
        case CityType::STRAIGHT_GIN:
            _slotManager->setNumSlot(2);
            _roomName = GET_TEXT_BY_ID(TXT_ROOM_2);
            break;
        case CityType::OKLAHOMA_GIN:
            _slotManager->setNumSlot(2);
            _roomName = GET_TEXT_BY_ID(TXT_ROOM_3);
            break;
        default:
            break;
    }
}

void GPGinScene::showTutorial(bool show)
{
    if (show)
    {
        auto tutorialView = TutorialView::create();
        tutorialView->setName("TutorialView");
        tutorialView->setContent({ RuleDetails::create()/*, RuleScore::create() */});
        addChild(tutorialView, kAboveForeground);
    }
    else
    {
        this->removeChildByName("TutorialView");
    }
}

void GPGinScene::handleQuitInGame(const rapidjson::Document& data)
{
    _exited = false;
    if (!this->_rejoin)
    {
        auto backLobby = [=]() {
            MessageHandler::getInstance()->unregisterHandleMessage(this);
            this->stopAllActions();
            LobbyScene::_backFromGameScene = true;
            prepareQuit();
            if (_adsExit)
                AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), true, INTERSTITIAL_ADMOB_EXIT_INGAME);
            else
            {
                _exited = true;
                SceneManager::getInstance()->openScene(SceneID::LOBBY_SCENE, cocos2d::Value());
            }
        };
        backLobby();
    }
}

void GPGinScene::handleUserProfile(const rapidjson::Document& data)
{
    const rapidjson::Value& listPlace = JsonHelper::getValue(data, KEY_LIST_PLACES);
    _interfaceLayer->showNotifyBack(false);
    if (s_startMatchCount > 5)
    {
        for (unsigned int i = 0; i < listPlace.Size(); i++)
        {
            const rapidjson::Value& place = listPlace[i];
            LobbyPlace::State state = (LobbyPlace::State) JsonHelper::getInt(place, KEY_PLACE_STATUS, 0);
            double            price = JsonHelper::getInt(place, KEY_PLACE_PRICE, 0);
            if (state == LobbyPlace::State::LOCK_CAN_UNLOCK && UserData::getInstance()->getMoney() >= price)
            {
                _interfaceLayer->showNotifyBack(true);
                break;
            }
        }
        const rapidjson::Value& listDataTable = JsonHelper::getValue(data, KEY_LIST_TABLES);
        for (unsigned int     i          = 0; i < listDataTable.Size(); i++)
        {
            const rapidjson::Value& dataTable = listDataTable[i];
            if (JsonHelper::getInt(dataTable, KEY_TABLE_STATUS, 0) == 0 && UserData::getInstance()->getMoney() >= JsonHelper::getInt(dataTable, KEY_TABLE_PRICE, 0))
            {
                _interfaceLayer->showNotifyBack(true);
                break;
            }
        }
        const rapidjson::Value& listMail = JsonHelper::getValue(data, KEY_MAILBOX_LIST);
        bool newMail = LobbyScene::hasNewMail(JsonHelper::convertValueToString(listMail));
        if (newMail)
            _interfaceLayer->showNotifyBack(newMail);
    }
}

void GPGinScene::handleCheckRatingPopup(const rapidjson::Document& data)
{
    int result = JsonHelper::getInt(data, KEY_RESULT, 0);
    if (result > 0)
    {
        _stateRating         = JsonHelper::getInt(data, KEY_RATING_STATE, 0);
        _conditionRatingName = JsonHelper::getString(data, KEY_RATING_CONDITION, "");
    }
    else
    {
        _stateRating         = 0;
        _conditionRatingName = "";
    }
}

void GPGinScene::handleSaviorGoldBonusPopup(const rapidjson::Document& data)
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

            int    status    = node[KEY_SAVIOR_STATUS].GetInt();
            double money     = node[KEY_FREE_MONEY].GetDouble();
            double countDown = 0;
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

void GPGinScene::handleGoldBonus(const rapidjson::Document& data)
{
    int    result   = data[KEY_RESULT].GetInt();
    double money    = data[KEY_MONEY].GetDouble();
    double adsMoney = data[KEY_ADS_VIEW_MONEY].GetDouble();
    double oldMoney = UserData::getInstance()->getMoney();
    _slotManager->getSlotInfo().at(MAIN_USER_SLOT_INDEX)->setMoney(money);
    _interfaceLayer->updateMoney(MAIN_USER_SLOT_INDEX);
    UserData::getInstance()->setMoney(money);
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
    s_watchVideoCount++;
    Firebase::logGetMoneyWatchVideo("gameplay", s_watchVideoCount, money - oldMoney);
    s_totalWatchVideoCount++;
    UserData::getInstance()->saveWatchVideoNumber();
    Firebase::logTotalWatchVideoNumber(s_totalWatchVideoCount);
    _interfaceLayer->onVideoAdsMoney(data);
}

void GPGinScene::handleSaviorGoldBonus(const rapidjson::Document& data)
{
    int    result   = data[KEY_RESULT].GetInt();
    double money    = data[KEY_MONEY].GetDouble();
    double oldMoney = UserData::getInstance()->getMoney();
    _slotManager->getSlotInfo().at(MAIN_USER_SLOT_INDEX)->setMoney(money);
    _interfaceLayer->updateMoney(MAIN_USER_SLOT_INDEX);
    UserData::getInstance()->setMoney(money);
    runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=] {
        if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_GET_REWARDS_ENABLE, true))
            AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), false, INTERSTITIAL_ADMOB_GET_MONEY_DEFAULT);
    }), nullptr));
    GPGinScene::_forceContinue = true;
    MessageSender::requestOpenTicket((int) _cityID, (int) _tableID);
    Firebase::logGetMoneyDefault("gameplay", money - oldMoney);
}

void GPGinScene::handleRejoinGame(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    if (result == RESULT_SUCCESS)
    {
        this->_cityID        = (CityType) JsonHelper::getInt(data, KEY_PLACE_ID, 1);
        this->_tableID       = (TableType) JsonHelper::getInt(data, KEY_TABLE_ID, 1);
        this->_betMoney      = JsonHelper::getDouble(data, KEY_BET, 1);
        this->_multiplyValue = JsonHelper::getDouble(data, KEY_MAX_MULTIPLY_VALUE, 1);
        this->_targetScore   = JsonHelper::getInt(data, KEY_TARGET_SCORE, -1);

        setBetMoney(this->_betMoney);
        this->_interfaceLayer->setMaxMultiplyNumber(this->_multiplyValue);
        //        this->_interfaceLayer->updateLayoutTargetScore(this->_targetScore, _listBetData);
        waitToStart(1.0f);
    }
}

void GPGinScene::handleUserData(const rapidjson::Document& data)
{
    double      money  = data[KEY_MONEY].GetDouble();
    std::string name   = data[KEY_NAME].GetString();
    std::string avatar = data[KEY_AVATAR].GetString();
    _slotManager->getSlotInfo().at(MAIN_USER_SLOT_INDEX)->setMoney(money);
    _interfaceLayer->updateMoney(MAIN_USER_SLOT_INDEX);
    UserData::getInstance()->setMoney(money);
}

void GPGinScene::handleTicker(const rapidjson::Document& data)
{
    int result = data[KEY_RESULT].GetInt();
    int city   = 0;
    if (result == RESULT_SUCCESS)
    {
        city                      = data[KEY_PLACE_ID].GetInt();
        double minBet             = data[KEY_MIN_BET].GetDouble();
        double maxBet             = data[KEY_MAX_BET].GetDouble();
        int    defaultTargetScore = data[KEY_DEFAULT_TARGET_SCORE].GetInt();

        ContinueToPlayConfirmPopup* continueConfirm = BoardManager::createContinueToPlayConfirmPopup();
        continueConfirm->initValue((CityType) city, GPGinScene::_forceContinue, minBet + (maxBet - minBet) * 0.5f, defaultTargetScore);
        continueConfirm->setTag((int) BoardID::CONTINUE_TO_PLAY_CONFIRM_POPUP);
        continueConfirm->setQuitCallback(CONTINUE_TO_PLAY_QUIT_CALLBACK_SELECTOR(GPGinScene::onQuit));
        continueConfirm->setContinueCallback(CONTINUE_TO_PLAY_CONTINUE_CALLBACK_SELECTOR(GPGinScene::onContinue));
        continueConfirm->attach(this, kAboveForeground);
    }
    else
    {
        onQuit();
    }
}

void GPGinScene::resetDealCards()
{
    removePopup((int) BoardID::THROW_CARD_DETAIL_POPUP);
    this->_dealCardLayer->reset();
    this->_dealCardLayer->setVisible(true);
    this->_interfaceLayer->setNumDrawCard(MAXIMUM_NUMBER_CARD, false, true);
    this->_interfaceLayer->setVisibleDrawCard(true);
    this->_interfaceLayer->setVisibleLayoutDrawCard(true);
    this->_interfaceLayer->setVisibleLayoutTakeCard(true);
    this->_interfaceLayer->setAllUserTurnOff();
}

void GPGinScene::handleDealCards()
{
#if(USE_CHEAT_CARD == 1)
    CheatCardLayer::getInstance()->requestCheatCard(-1);
#endif
    this->_canQuit = false;
    _interfaceLayer->showLayoutContinue(false);
    _interfaceLayer->showLayoutRewards(false);
    this->_dealCardLayer->setVisible(true);
    this->_dealCardLayer->setListSlotPos(this->_interfaceLayer->getListDealCardPosition(this->_slotManager->getListReadySlotOrderIdx()), 0);
    this->_dealCardLayer->setPosDealCardLayer(_controlCardLayer->listPositionCardsOnHandAccordingNumberCardsOfAllPlayers(_listDealCards), GPGinSlotManager::getInstance()->getSlotOrderIdx(_firstIndexDealCard));
    this->_dealCardLayer->playDealCardAnimation();
    s_startMatchCount++;

    Firebase::logStartMatch(cocos2d::StringUtils::format("%d", _cityID), cocos2d::StringUtils::format("%d", _tableID), UserData::getInstance()->getMoney(), this->_betMoney, this->_betMoney / UserData::getInstance()->getMoney(), 0, s_startMatchCount);
    removePopup((int) BoardID::VIDEO_REWARDS_POPUP);
    bool isOnline = CallToOtherLanguage::getInstance()->checkInternet();
    Firebase::logNetwork(isOnline);
    if (!isOnline)
        CallToOtherLanguage::getInstance()->showAlert(GET_TEXT_BY_ID(TXT_NOTIFY_VIDEO_ADS_NOT_AVAILABLE_1));
}

void GPGinScene::handlePlayerHandCard(const rapidjson::Document& data)
{
    _firstIndexDealCard = JsonHelper::getInt(data, KEY_FIRST_TURN_INDEX, 0);
    _upcardValue        = JsonHelper::getInt(data, KEY_UP_CARD_VALUE, 0);
    _knockScoreValue    = JsonHelper::getInt(data, KEY_KNOCK_POINTS, 0);
    int      orderIndex = MAIN_USER_SLOT_INDEX;
    for (int i          = 0; i < data[KEY_GIN_HAND_CARDS].Size(); i++)
        _listDealCards.at(orderIndex).push_back(data[KEY_GIN_HAND_CARDS][i].GetInt());
    _countListDealCard++;
    if (_countListDealCard == _slotManager->getNumberSlot())
    {
        this->resetDealCards();
        this->handleDealCards();
    }
    _controlCardLayer->setCurrentKnockScore(_knockScoreValue);

    _isSpadesDouble = JsonHelper::getBool(data, KEY_IS_SPADES_DOUBLE, false);
}

void GPGinScene::handleBotHandCard(const rapidjson::Document& data)
{
    int      slotIndex  = JsonHelper::getInt(data, KEY_BOT_ID, 0);
    int      orderIndex = _slotManager->getSlotOrderIdx(slotIndex);
    for (int i          = 0; i < data[KEY_GIN_HAND_CARDS].Size(); i++)
        _listDealCards.at(orderIndex).push_back(data[KEY_GIN_HAND_CARDS][i].GetInt());
    _countListDealCard++;
    if (_countListDealCard == _slotManager->getNumberSlot())
    {
        this->resetDealCards();
        this->handleDealCards();
    }
}

void GPGinScene::handleBeginTurn(const rapidjson::Document& data)
{
    int  slotIdx = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, 0);
    bool isUser  = JsonHelper::getBool(data, KEY_GIN_IS_USER, false);
    if (isUser)
        SoundManager::playSoundEffect(SoundEvent::TL_TURN);
    _interfaceLayer->setUserOnTurn(GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx));
    MessageSender::requestBeginTurnFinish(slotIdx);
}

void GPGinScene::handleEndTurn(const rapidjson::Document& data)
{
    _interfaceLayer->setAllUserTurnOff();
    _controlCardLayer->setUserOnTurn(MAIN_USER_SLOT_INDEX, false);
}

void GPGinScene::handleTurn(GinMatchStatus status, const rapidjson::Document& data)
{
    int playerIndex = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, -1);
    int orderIdx    = GPGinSlotManager::getInstance()->getSlotOrderIdx(playerIndex);
    if (status == GinMatchStatus::GIN_MATCH_STATUS_PASS_CARD)
    {
        _interfaceLayer->updateStatusUser(status, orderIdx);
        if (orderIdx == MAIN_USER_SLOT_INDEX)
            _controlCardLayer->hintPassCard(true, JsonHelper::getBool(data, KEY_GIN_HINT_TAKE, false), JsonHelper::getBool(data, KEY_GIN_CAN_TAKE, false));
    }
    else if (status == GinMatchStatus::GIN_MATCH_STATUS_DRAW_CARD)
    {
        _interfaceLayer->updateStatusUser(status, orderIdx);
        if (orderIdx == MAIN_USER_SLOT_INDEX)
            _controlCardLayer->hintGetCard(true,
                                           JsonHelper::getInt(data, KEY_GIN_PREVIOUS_PLAYER_INDEX, -1),
                                           JsonHelper::getInt(data, KEY_GIN_PREVIOUS_PLAYER_CARD, -1),
                                           JsonHelper::getBool(data, KEY_GIN_HINT_TAKE, false),
                                           JsonHelper::getBool(data, KEY_GIN_CAN_TAKE, false));
    }
    else if (status == GinMatchStatus::GIN_MATCH_STATUS_THROW_CARD)
    {
        _interfaceLayer->updateStatusUser(status, orderIdx);
        if (orderIdx == MAIN_USER_SLOT_INDEX)
        {
            _controlCardLayer->setKnockDeadwood(JsonHelper::getInt(data, KEY_KNOCK_POINTS, -1));
            _controlCardLayer->hintDumpCard(true, (ActionType) JsonHelper::getInt(data, KEY_GIN_ACTION_TYPE, (int) ActionType::K_NONE));
        }
    }

    if (_turnActionOrderIdx != orderIdx)
    {
        int previousTurnActionOrderIdx = _turnActionOrderIdx;
        _turnActionOrderIdx = orderIdx;
        _interfaceLayer->setUserOnTurn(orderIdx);
        _controlCardLayer->setUserOnTurn(_turnActionOrderIdx, true);
        if (previousTurnActionOrderIdx >= 0 && previousTurnActionOrderIdx < MAXIMUM_NUMBER_SLOT_GAME)
            _controlCardLayer->setUserOnTurn(previousTurnActionOrderIdx, false);
    }
}

void GPGinScene::handlePlayerTurnStepPassingDemandServer(const rapidjson::Document& data)
{
    this->handleTurn(GinMatchStatus::GIN_MATCH_STATUS_PASS_CARD, data);
}

void GPGinScene::handlePlayerTurnStepPassing(const rapidjson::Document& data)
{
    bool result      = JsonHelper::getBool(data, KEY_GIN_ACTION_RESULT, false);
    int  playerIndex = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, -1);
    int  orderId     = GPGinSlotManager::getInstance()->getSlotOrderIdx(playerIndex);
    if (result)
    {
        if (orderId == MAIN_USER_SLOT_INDEX)
        {
            _controlCardLayer->hintPassCard(false, false, false);
            int cardValue = JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, -1);
            if (cardValue >= 0)
            {
                _controlCardLayer->takeCard(orderId, cardValue);
                this->hideAllPassEffect();
            }
            else
            {
                this->showStatus(playerIndex, FightStatus::PASS, false);
                this->_mapStatusPassShow[playerIndex] = true;
//                this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
//                    this->_interfaceLayer->hideStatus(playerIndex);
//                }), nullptr));
                this->hideAllPassEffectExcept(playerIndex);
            }
        }
        else
        {
            int cardValue = JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, -1);
            if (cardValue >= 0)
            {
                _controlCardLayer->takeCard(orderId, cardValue);
                this->hideAllPassEffect();
            }
            else
            {
                this->showStatus(playerIndex, FightStatus::PASS, false);
                this->_mapStatusPassShow[playerIndex] = true;
//                this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=] {
//                    this->_interfaceLayer->hideStatus(playerIndex);
//                }), nullptr));
                this->hideAllPassEffectExcept(playerIndex);
            }
        }
        MessageSender::requestPassDone(playerIndex);
    }
    else
    {
        if (orderId == MAIN_USER_SLOT_INDEX)
            _controlCardLayer->hintPassCard(true, JsonHelper::getBool(data, KEY_GIN_HINT_TAKE, false), JsonHelper::getBool(data, KEY_GIN_CAN_TAKE, false));
    }
}

void GPGinScene::handlePlayerTurnStepGetCardDemandServer(const rapidjson::Document& data)
{
    this->handleTurn(GinMatchStatus::GIN_MATCH_STATUS_DRAW_CARD, data);
}

void GPGinScene::handlePlayerTurnStepGetCard(const rapidjson::Document& data)
{
    bool result      = JsonHelper::getBool(data, KEY_GIN_ACTION_RESULT, false);
    int  playerIndex = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, -1);
    if (result)
    {
        this->hideAllPassEffect();

        PlayerCardData listPlayerData[MAXIMUM_NUMBER_SLOT_GAME];
        const rapidjson::Value& listPlayerDataJson = JsonHelper::getValue(data, KEY_GIN_LIST_PLAYER_DATA);
        if (listPlayerDataJson.IsArray())
        {
            for (rapidjson::Value::ConstValueIterator iterPlayer = listPlayerDataJson.Begin(); iterPlayer != listPlayerDataJson.End(); iterPlayer++)
            {
                const rapidjson::Value& playerData = *iterPlayer;
                int oderIndexData = GPGinSlotManager::getInstance()->getSlotOrderIdx(JsonHelper::getInt(playerData, KEY_GIN_PLAYER_INDEX, -1));
                PlayerCardData& playerDatas = listPlayerData[oderIndexData];
                if (oderIndexData >= 0 && oderIndexData < MAXIMUM_NUMBER_SLOT_GAME)
                {
                    playerDatas._isUser                      = JsonHelper::getBool(playerData, KEY_GIN_IS_USER, false);
                    playerDatas._money                       = JsonHelper::getDouble(playerData, KEY_MONEY, 0);
                    const rapidjson::Value& handCardDataJson = JsonHelper::getValue(playerData, KEY_GIN_HAND_CARDS);
                    playerDatas._handCards.clear();
                    for (rapidjson::Value::ConstValueIterator iterCard = handCardDataJson.Begin(); iterCard != handCardDataJson.End(); iterCard++)
                        playerDatas._handCards.push_back((*iterCard).GetInt());
                    const rapidjson::Value& throwCardDataJson = JsonHelper::getValue(playerData, KEY_GIN_THROW_CARDS);
                    playerDatas._throwCards.clear();
                    for (rapidjson::Value::ConstValueIterator iterCard = throwCardDataJson.Begin(); iterCard != throwCardDataJson.End(); iterCard++)
                        playerDatas._throwCards.push_back((*iterCard).GetInt());
                    const rapidjson::Value& listCombinationDataJson = JsonHelper::getValue(playerData, KEY_GIN_LIST_COMBINATION);
                    playerDatas._listCombinationPhom.clear();
                    for (int i = 0; i < listCombinationDataJson.Size(); i++)
                    {
                        std::vector<int> combinationCard;
                        for (int         j = 0; j < listCombinationDataJson[i].Size(); j++)
                            combinationCard.push_back(listCombinationDataJson[i][j].GetInt());
                        playerDatas._listCombinationPhom.push_back(combinationCard);
                    }
                }
            }
        }
        int                   cardValue            = JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, -1);
        int                   playerTakenIndex     = JsonHelper::getInt(data, KEY_GIN_TAKE_PLAYER_INDEX, -1);
        if (playerTakenIndex < 0)
        {
            _controlCardLayer->drawCard(GPGinSlotManager::getInstance()->getSlotOrderIdx(playerIndex), cardValue);
            _interfaceLayer->setNumDrawCard(_interfaceLayer->getNumDrawCard() - 1, true);
        }
        else
        {
            _controlCardLayer->takeCard(GPGinSlotManager::getInstance()->getSlotOrderIdx(playerIndex), cardValue);
        }
        this->runAction(Sequence::create(DelayTime::create(TIME_CARD_TAKE), CallFunc::create([=]() {
            MessageSender::requestGetCardDone(playerIndex);
            this->updateThrowCardsPopup();
        }), nullptr));
    }
    else
    {
        if (GPGinSlotManager::getInstance()->getSlotOrderIdx(playerIndex) == MAIN_USER_SLOT_INDEX)
            _controlCardLayer->hintGetCard(true,
                                           JsonHelper::getInt(data, KEY_GIN_TAKE_PLAYER_INDEX, -1),
                                           JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, -1),
                                           JsonHelper::getBool(data, KEY_GIN_HINT_TAKE, false),
                                           JsonHelper::getBool(data, KEY_GIN_CAN_TAKE, false));
    }
}

void GPGinScene::handlePlayerThrowCardDemandServer(const rapidjson::Document& data)
{
    this->handleTurn(GinMatchStatus::GIN_MATCH_STATUS_THROW_CARD, data);
}

void GPGinScene::handlePlayerThrowCard(const rapidjson::Document& data)
{
    SoundManager::playSoundEffect(SoundEvent::TL_THROW_CARD);
    bool result   = JsonHelper::getBool(data, KEY_GIN_ACTION_RESULT, false);
    int  slotIdx  = JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, 0);
    int  orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(slotIdx);
    int actionType = JsonHelper::getInt(data, KEY_GIN_ACTION_TYPE, -1);
    if (result)
    {
        int cardValue = JsonHelper::getInt(data, KEY_GIN_CARD_VALUE, -1);
        if (cardValue >= 0)
        {
            bool finalCard = false;
            if(actionType != -1)
            {
                finalCard = true;
            }
//            finalCard = true;

            float delayTime = TIME_CARD_THROW;
            if(finalCard == true)
            {
                delayTime = GinCard::getSpinEffectDuration() + TIME_FINAL_CARD_DELAY + 0.2f;
            }
            _controlCardLayer->throwCard(orderIdx, cardValue, actionType);
            this->runAction(Sequence::create(DelayTime::create(delayTime), CallFunc::create([=]() {
                MessageSender::requestThrowCardDone(slotIdx, getListMeldOnHandMainUserBySoftGroup());
            }), nullptr));
        }
        else
        {
            MessageSender::requestThrowCardDone(slotIdx, getListMeldOnHandMainUserBySoftGroup());
        }
    }
    else
    {
        if (orderIdx == MAIN_USER_SLOT_INDEX)
        {
            _controlCardLayer->showNotifyWrong(GET_TEXT_BY_ID(TXT_CAN_NOT_THROW));
            _controlCardLayer->hintDumpCard(true, (ActionType) JsonHelper::getInt(data, KEY_GIN_ACTION_TYPE, (int) ActionType::K_NONE));
            _controlCardLayer->resetDataByDrapDropCards(false);
            _controlCardLayer->forceUndrapAllCards();
        }
    }
}

void GPGinScene::handleEndMatch(const rapidjson::Document& data)
{
    double           _claimedMoney = UserData::getInstance()->getMoney();
    PlayerResultData winInfo;
    winInfo._playerIndex = -1;
    PlayerResultData              userInfo;
    std::vector<PlayerResultData> listLoseInfo;
    std::vector<PlayerResultData> listPlayerResultData;

    const rapidjson::Value& jackpot = JsonHelper::getValue(data, KEY_JACKPOT);
    this->_jackpotMoney = JsonHelper::getDouble(jackpot, KEY_JACKPOT_MONEY, 0);
    this->_jackpotRound = JsonHelper::getInt(jackpot, KEY_JACKPOT_ROUND, 0);
    this->_interfaceLayer->setJackpotValue(this->_jackpotMoney);
    this->_jackpotLock  = JsonHelper::getBool(jackpot, KEY_JACKPOT_LOCK, false);

//    this->_interfaceLayer->setJackpotValue(this->_jackpotMoney);
//    if (this->_jackpotLock == false)
//    {
//        this->_interfaceLayer->runJackpotBackgroundAnimation(true);
//    }
//    else
//    {
//        this->_interfaceLayer->runJackpotBackgroundAnimation(false);
//    }

    PlayerResultData userGiveCardInfo;
    userGiveCardInfo._playerIndex = -1;
    bool isWinningBet = false;
    const rapidjson::Value& listPlayerData = JsonHelper::getValue(data, KEY_GIN_LIST_PLAYER_DATA);
    for (int i = 0; i < listPlayerData.Size(); i++)
    {
        const rapidjson::Value& playerData = listPlayerData[i];
        PlayerResultData playerResultData;
        playerResultData._playerIndex  = JsonHelper::getInt(playerData, KEY_GIN_PLAYER_INDEX, 0);
        playerResultData._userName     = this->_slotManager->getSlotInfo(playerResultData._playerIndex)->getUserName();
        playerResultData._avatarLink   = this->_slotManager->getSlotInfo(playerResultData._playerIndex)->getAvatarLink();
        playerResultData._isUser       = JsonHelper::getInt(playerData, KEY_GIN_IS_USER, false);
        playerResultData._money        = JsonHelper::getDouble(playerData, KEY_MONEY, 0);
        playerResultData._level        = JsonHelper::getInt(playerData, KEY_LEVEL, 0);
        playerResultData._levelExp     = JsonHelper::getInt(playerData, KEY_LEVELEXP, 0);
        playerResultData._levelExpNext = JsonHelper::getInt(playerData, KEY_NEXTLEVELEXP, 0);
        playerResultData._rewardMoney  = JsonHelper::getDouble(playerData, KEY_GIN_REWARD_MONEY, 0);
        playerResultData._winOrder     = JsonHelper::getInt(playerData, KEY_GIN_WIN_ORDER, 0);
        playerResultData._type         = JsonHelper::getInt(playerData, KEY_GIN_RESULT_TYPE, 0);

        const rapidjson::Value& listCombinationJson = JsonHelper::getValue(playerData, KEY_GIN_LIST_COMBINATION);
        for (int ii = 0; ii < listCombinationJson.Size(); ii++)
        {
            const rapidjson::Value& combinationJson = listCombinationJson[ii];
            std::vector<int> combinationPhom;
            for (int         j = 0; j < combinationJson.Size(); j++)
                combinationPhom.push_back(combinationJson[j].GetInt());
            playerResultData._listPhom.push_back(combinationPhom);
        }
        const rapidjson::Value& listHandCardJson = JsonHelper::getValue(playerData, KEY_GIN_HAND_CARDS);
        for (int j = 0; j < listHandCardJson.Size(); j++)
            playerResultData._remainCards.push_back(listHandCardJson[j].GetInt());
        const rapidjson::Value& listGiveCardData = JsonHelper::getValue(playerData, KEY_GIN_GIVE_CARD_DATA);
        for (int k = 0; k < listGiveCardData.Size(); k++)
        {
            PointGiveServerDefine pointGiveCard;
            const rapidjson::Value& givecardData = listGiveCardData[k];
            const rapidjson::Value& cardsData    = JsonHelper::getValue(givecardData, KEY_GIN_GIVE_CARD_VALUES);
            for (int j = 0; j < cardsData.Size(); j++)
                pointGiveCard.card.push_back(cardsData[j].GetInt());
            pointGiveCard.slot      = JsonHelper::getInt(givecardData, KEY_GIN_GIVE_CARD_PLAYER_INDEX, 0);
            pointGiveCard.phomOrder = JsonHelper::getInt(givecardData, KEY_GIN_GIVE_CARD_PLAYER_COMBINATION_INDEX, 0);
            playerResultData._listGiveCards.push_back(pointGiveCard);
        }
        const rapidjson::Value& resultDetail = JsonHelper::getValue(playerData, KEY_GIN_RESULT_DETAIL);
        playerResultData._resultDetail._handPoints        = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_HAND_POINTS, 0);
        playerResultData._resultDetail._targetPoints      = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_TARGET_POINTS, 0);
        playerResultData._resultDetail._normalWinMoney    = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_NORMAL_WIN, 0);
        playerResultData._resultDetail._bonusWinMoney     = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_BONUS_WIN, 0);
        playerResultData._resultDetail._jackpotWinMoney   = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_JACKPOT_WIN, 0);
        playerResultData._resultDetail._scoreGin          = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_BONUS_POINTS_GIN, 0);
        playerResultData._resultDetail._scoreBigGin       = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_BONUS_POINTS_BIG_GIN, 0);
        playerResultData._resultDetail._scoreUnderCut     = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_BONUS_POINTS_UNDERCUT, 0);
        playerResultData._resultDetail._winPoints         = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_WIN_POINTS, 0);
        playerResultData._resultDetail._scoreSpadesDouble = JsonHelper::getDouble(resultDetail, KEY_GIN_RESULT_DETAIL_BONUS_POINTS_SPADES_BONUS, 0);
        BetData& betData = this->getBetDataRef(playerResultData._playerIndex);
        betData._score           = playerResultData._resultDetail._targetPoints;
        playerResultData._isUser = (playerResultData._playerIndex == MAIN_USER_SLOT_INDEX);
        if (playerResultData._winOrder == 0)
            winInfo          = playerResultData;
        else
            listLoseInfo.push_back(playerResultData);
        if (playerResultData._rewardMoney > 0)
            isWinningBet     = true;
        if (GPGinSlotManager::getInstance()->getSlotOrderIdx(playerResultData._playerIndex) == MAIN_USER_SLOT_INDEX)
        {
            UserData::getInstance()->setMoney(playerResultData._money);
            UserData::getInstance()->setLevel(playerResultData._level);
            UserData::getInstance()->setLevelExp(playerResultData._levelExp);
            UserData::getInstance()->setNextLevelExp(playerResultData._levelExpNext);
            _claimedMoney = UserData::getInstance()->getMoney() - _claimedMoney;
            userInfo      = playerResultData;
        }
        if (listGiveCardData.Size() > 0)
            userGiveCardInfo = playerResultData;
        listPlayerResultData.push_back(playerResultData);
    }

    float showCardPhaseAnimDuration = _controlCardLayer->calculateMatchEndShowCardDuration(winInfo, listLoseInfo);
    bool  isDraw                    = ((GinResultType) listLoseInfo[0]._type == GinResultType::GIN_RESULT_TYPE_DRAW);

    // Show Effect Win, Lose, Gin, Burned
    cocos2d::Vector<FiniteTimeAction*> listAction;
    listAction.pushBack(DelayTime::create(0.4f));
    listAction.pushBack(CallFunc::create([=]() {
        _interfaceLayer->setAllUserTurnOff();
        _interfaceLayer->updateStatusUser(GinMatchStatus::GIN_MATCH_STATUS_FINISH, -1);
        _interfaceLayer->setVisibleDeadwoodBox(false, 0, 0, MAIN_USER_SLOT_INDEX, Vec2::ZERO);
        _interfaceLayer->setLayoutDrawAndTakeVisible(false);
        _controlCardLayer->preMatchEnd();

        GinResultType     resultType = (GinResultType) winInfo._type;
        GinMatchEndStatus status     = GinMatchEndStatus::GIN_MATCH_END_STATUS_NONE;

        if (isDraw == true)
        {
            status = GinMatchEndStatus::GIN_MATCH_END_STATUS_DRAW;
            this->showMatchEndStatus(status, Vec2::ZERO);
        }
        else if (resultType == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
        {
            status = GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN;
            this->showMatchEndStatus(status, Vec2::ZERO);
            this->_controlCardLayer->shakeTableWithDelay(0.4f);
        }
    }));
    listAction.pushBack(DelayTime::create(0.8f));

    listAction.pushBack(CallFunc::create([=, &showCardPhaseAnimDuration]()
                                         {
                                             SoundManager::playSoundEffect(SoundEvent::TL_COMPLETE);
                                             _controlCardLayer->matchEnd(winInfo, listLoseInfo, nullptr);

                                         }));
    listAction.pushBack(DelayTime::create(showCardPhaseAnimDuration));
    if(isDraw == false)
    {
        // Give card
//        listAction.pushBack(DelayTime::create(userGiveCardInfo._playerIndex >= 0 ? showCardPhaseAnimDuration : 0));
        listAction.pushBack(CallFunc::create([=]() {
            if (userGiveCardInfo._playerIndex >= 0)
            {
                int      orderIdx = GPGinSlotManager::getInstance()->getSlotOrderIdx(userGiveCardInfo._playerIndex);
                for (int i        = 0; i < userGiveCardInfo._listGiveCards.size(); i++)
                {
                    _controlCardLayer->giveCard(orderIdx, userGiveCardInfo._listGiveCards[i], i);
                }
            }
        }));
        // Update target score
        listAction.pushBack(DelayTime::create(/*1.4f +*/ (userGiveCardInfo._listGiveCards.size()) * 1.8f));
        listAction.pushBack(CallFunc::create([=]() {

            PlayerResultData playerResultData = listLoseInfo[0];
            int              winHandDeadwood  = winInfo._resultDetail._handPoints;
            int              loseHandDeadwood = playerResultData._resultDetail._handPoints;

            int           score      = (winHandDeadwood > loseHandDeadwood) ? (winHandDeadwood - loseHandDeadwood) : (loseHandDeadwood - winHandDeadwood);
            GinResultType resultType = (GinResultType) winInfo._type;
            if (resultType == GinResultType::GIN_RESULT_TYPE_WIN_KNOCK || resultType == GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT)
            {
                score += 0;
            }
            else if (resultType == GinResultType::GIN_RESULT_TYPE_WIN_GIN)
            {
                score += 0;
            }
            else if (resultType == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
            {
                score += 0;
            }
            //        if(score != _interfaceLayer->getResultScoreBox()->getScore())
            {
                Point dstPos = _interfaceLayer->getResultScoreBox()->getParent()->convertToWorldSpace(_interfaceLayer->getResultScoreBox()->getPosition());
                _interfaceLayer->playMoveAndHideScoreBoxAnimation(MAIN_USER_SLOT_INDEX, dstPos, 1.0f);
                _interfaceLayer->playMoveAndHideScoreBoxAnimation(BOT_SLOT_INDEX, dstPos, 1.0f);
                _interfaceLayer->getResultScoreBox()->playShowGapAnimation(nullptr, score, 1.5f);
            }
        }));
        listAction.pushBack(DelayTime::create(1.5f));
        if (winInfo._type == GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT)
        {
            listAction.pushBack(CallFunc::create([=]() {
                this->showMatchEndStatus(GinMatchEndStatus::GIN_MATCH_END_STATUS_UNDERCUT, STATUS_POSITION_OFFSET);
                this->_controlCardLayer->shakeTableWithDelay(0.4f);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
            listAction.pushBack(CallFunc::create([=] {
                int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut;
                this->_interfaceLayer->getResultScoreBox()->playUndercutAnimation(nullptr, totalScore);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
        }
        else if (winInfo._type == GinResultType::GIN_RESULT_TYPE_WIN_GIN)
        {
            listAction.pushBack(CallFunc::create([=]() {
                this->showMatchEndStatus(GinMatchEndStatus::GIN_MATCH_END_STATUS_GIN_25, STATUS_POSITION_OFFSET);
                this->_controlCardLayer->shakeTableWithDelay(0.4f);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
            listAction.pushBack(CallFunc::create([=] {
                int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut;
                this->_interfaceLayer->getResultScoreBox()->playUndercutAnimation(nullptr, totalScore);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
        }
        else if (winInfo._type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
        {
            listAction.pushBack(CallFunc::create([=]() {
                this->showMatchEndStatus(GinMatchEndStatus::GIN_MATCH_END_STATUS_BIGGIN_50, STATUS_POSITION_OFFSET);
                this->_controlCardLayer->shakeTableWithDelay(0.4f);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
            listAction.pushBack(CallFunc::create([=] {
                int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut;
                this->_interfaceLayer->getResultScoreBox()->playUndercutAnimation(nullptr, totalScore);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
        }
        else
        {
            int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut;
            this->_interfaceLayer->getResultScoreBox()->playScoreGapUpdateAnimation(nullptr, totalScore);
        }

        if (_isSpadesDouble == true)
        {
            listAction.pushBack(CallFunc::create([=]() {
                this->showMatchEndStatus(GinMatchEndStatus::GIN_MATCH_END_STATUS_SPADES_DOUBLE, STATUS_POSITION_OFFSET);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
            listAction.pushBack(CallFunc::create([=] {
                int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut + winInfo._resultDetail._scoreSpadesDouble;
                this->_interfaceLayer->getResultScoreBox()->playUndercutAnimation(nullptr, totalScore);
            }));
            listAction.pushBack(DelayTime::create(2.0f));
        }

        listAction.pushBack(DelayTime::create(0.5f));
        listAction.pushBack(CallFunc::create([=]() {
            this->_interfaceLayer->setScoreBoxColor(1, winInfo._playerIndex);

            if (winInfo._playerIndex >= 0)
                _interfaceLayer->showEffectRank(GPGinSlotManager::getInstance()->getSlotOrderIdx(winInfo._playerIndex),
                                                (GinResultType) winInfo._type, 1);
            for (int i = 0; i < listLoseInfo.size(); i++)
                _interfaceLayer->showEffectRank(
                        GPGinSlotManager::getInstance()->getSlotOrderIdx(listLoseInfo.at(i)._playerIndex),
                        (GinResultType) listLoseInfo.at(i)._type, listLoseInfo.at(i)._winOrder + 1);

            int totalScore = winInfo._resultDetail._winPoints + winInfo._resultDetail._scoreGin + winInfo._resultDetail._scoreBigGin + winInfo._resultDetail._scoreUnderCut + winInfo._resultDetail._scoreSpadesDouble;
            this->_interfaceLayer->getResultScoreBox()->playShowFinalAnimation(nullptr, winInfo._avatarLink, totalScore, winInfo._resultDetail._targetPoints, this->_targetScore);
        }));

        listAction.pushBack(DelayTime::create(1.0f));
        // Update rank
        listAction.pushBack(CallFunc::create([=]() {
//            if (isWinningBet == false)
//            {
//                this->_interfaceLayer->runShowScoreTableAnimation();
//            }
//            if (winInfo._playerIndex >= 0)
//                _interfaceLayer->showEffectRank(GPGinSlotManager::getInstance()->getSlotOrderIdx(winInfo._playerIndex), (GinResultType) winInfo._type, 1);
//            for (int i = 0; i < listLoseInfo.size(); i++)
//                _interfaceLayer->showEffectRank(GPGinSlotManager::getInstance()->getSlotOrderIdx(listLoseInfo.at(i)._playerIndex), (GinResultType) listLoseInfo.at(i)._type, listLoseInfo.at(i)._winOrder + 1);

//            this->_interfaceLayer->getResultScoreBox()->playHideAnimation(nullptr, 2.0f);
            Point pos = Point::ZERO;
            if(winInfo._playerIndex == MAIN_USER_SLOT_INDEX)
            {
                pos = this->_interfaceLayer->getResultScoreBox()->getParent()->convertToNodeSpace(this->_interfaceLayer->getScoreTable()->getUserScoreGlobalPos());
            }
            else
            {
                pos = this->_interfaceLayer->getResultScoreBox()->getParent()->convertToNodeSpace(this->_interfaceLayer->getScoreTable()->getBotScoreGlobalPos());
            }
            pos.x -= 33;
            this->_interfaceLayer->getResultScoreBox()->playMoveHideAnimation(nullptr, pos, 0.0f);

            if (this->_jackpotLock == true)
            {
                this->_interfaceLayer->runJackpotBackgroundAnimation(false);
            }
        }));
        listAction.pushBack(DelayTime::create(1.0f));
        listAction.pushBack(CallFunc::create([=]() {
            _interfaceLayer->updateLayoutTargetScore(_targetScore, _listBetData);

            if (this->_cityID == CityType::OKLAHOMA_GIN && _isSpadesDouble == true)
            {
                this->_interfaceLayer->playHideExtraRoomInformation();
            }
        }));
    }
    else
    {
        listAction.pushBack(CallFunc::create([=]()
                                             {
                                                 this->_interfaceLayer->getResultScoreBox()->playHideAnimation(nullptr, 2.0f);
                                                 _interfaceLayer->updateLayoutTargetScore(_targetScore, _listBetData);
                                             }
        ));
        listAction.pushBack(DelayTime::create(2.2f));
        listAction.pushBack(CallFunc::create([=]() {
            if (this->_cityID == CityType::OKLAHOMA_GIN && _isSpadesDouble == true)
            {
                this->_interfaceLayer->playHideExtraRoomInformation();
            }
        }));
    }
    // Update money if end game
    if (isWinningBet)
    {
        listAction.pushBack(DelayTime::create(0.8f));
        listAction.pushBack(CallFunc::create([=]{
            this->_interfaceLayer->runHideScoreTableAnimation();
        }));
        listAction.pushBack(DelayTime::create(1.0f));
        listAction.pushBack(CallFunc::create([=] {
            PlayerResultData userData;
            PlayerResultData botData;

            if (winInfo._isUser)
            {
                userData = winInfo;
                botData  = listLoseInfo[0];
            }
            else
            {
                userData = listLoseInfo[0];
                botData  = winInfo;
            }

            if (winInfo._playerIndex >= 0)
            {
                GPGinSlotManager::getInstance()->getSlotInfo(winInfo._playerIndex)->setMoney(winInfo._money);
            }
            for (const auto& loseInfo : listLoseInfo)
            {
                GPGinSlotManager::getInstance()->getSlotInfo(loseInfo._playerIndex)->setMoney(loseInfo._money);
            }

            this->_controlCardLayer->setShowCardsVisible(false);
            this->_interfaceLayer->setVisibleUIOnEndGame();

            this->_interfaceLayer->getEndGameResultNode()->reset();
            this->_interfaceLayer->getEndGameResultNode()->setVisible(true);
            this->_interfaceLayer
                ->getEndGameResultNode()
                ->playEndGameAnimation(userData._avatarLink,
                                       botData._avatarLink,
                                       userData._resultDetail._targetPoints,
                                       botData._resultDetail._targetPoints,
                                       userData._rewardMoney,
                                       botData._rewardMoney,
                                       winInfo._resultDetail._normalWinMoney,
                                       winInfo._resultDetail._bonusWinMoney,
                                       winInfo._resultDetail._jackpotWinMoney,
                                       winInfo._isUser,
                                       _betMoney,
                                       _rule,
                                       _cityID,
                                       _targetScore);
//            this->_interfaceLayer->runHideScoreTableAnimation();
        }));
        listAction.pushBack(DelayTime::create(this->_interfaceLayer
                                                  ->getEndGameResultNode()
                                                  ->getEstimateEndGameAnimationTime(abs(winInfo._resultDetail._targetPoints - listLoseInfo[0]._resultDetail._targetPoints),
                                                                                    winInfo._resultDetail._bonusWinMoney / _betMoney + 1,
                                                                                    (winInfo._resultDetail._jackpotWinMoney > 0)) + 0.0f));
        listAction.pushBack(CallFunc::create([=] {
            this->_interfaceLayer->getEndGameResultNode()->setVisible(false);
        }));
        listAction.pushBack(DelayTime::create(1.0f));
        listAction.pushBack(CallFunc::create([=]() {
//            if (winInfo._playerIndex >= 0)
//            {
//                GPGinSlotManager::getInstance()->getSlotInfo(winInfo._playerIndex)->setMoney(winInfo._money);
//                _interfaceLayer->showEffectScoreWinLose(true, winInfo._rewardMoney, true, GPGinSlotManager::getInstance()->getSlotOrderIdx(winInfo._playerIndex));
//                SoundManager::playSoundEffect(SoundEvent::TL_SCORE);
//            }
//            for (const auto& loseInfo : listLoseInfo)
//            {
//                GPGinSlotManager::getInstance()->getSlotInfo(loseInfo._playerIndex)->setMoney(loseInfo._money);
//                _interfaceLayer->showEffectScoreWinLose(false, loseInfo._rewardMoney, true, GPGinSlotManager::getInstance()->getSlotOrderIdx(loseInfo._playerIndex));
//            }

            if (winInfo._resultDetail._jackpotWinMoney > 0)
            {
                this->_interfaceLayer->playShowJackpot();
                this->_interfaceLayer->setJackpotValue(0);
            }
        }));
//        listAction.pushBack(DelayTime::create(4.0f));
        listAction.pushBack(CallFunc::create([=]() {
            GinSlotInfo* slotInfo = GPGinSlotManager::getInstance()->getSlotInfo(MAIN_USER_SLOT_INDEX);
            slotInfo->setLevel(UserData::getInstance()->getLevel());
            slotInfo->setLevelCurrentExp(UserData::getInstance()->getLevelExp());
            slotInfo->setLevelTotalExp(UserData::getInstance()->getNextLevelExp());
            _interfaceLayer->updateLevel();
        }));
        listAction.pushBack(DelayTime::create(1.0));
    }
    else
    {
        listAction.pushBack(DelayTime::create(2.2f));
    }
    // Finish
//    listAction.pushBack(DelayTime::create(2.6));
    listAction.pushBack(CallFunc::create([=]() {
        removePopup((int) BoardID::NOTIFY_POPUP);
        removePopup((int) BoardID::LOBBY_SETTING_BOARD);
        removePopup((int) BoardID::GAME_CENTER_POPUP);
        removePopup((int) BoardID::THROW_CARD_DETAIL_POPUP);
        this->onMatchCompleted();

        if (isWinningBet == true)
        {
            this->_interfaceLayer->runShowScoreTableAnimation();
        }
        else
        {
            this->_interfaceLayer->setVisibleScoreBox(true, MAIN_USER_SLOT_INDEX);
            this->_interfaceLayer->setVisibleScoreBox(true, BOT_SLOT_INDEX);
        }
    }));
    this->runAction(Sequence::create(listAction));
    if (_claimedMoney > 0)
    {
        s_winNumber++;
        Firebase::logWinResult(_claimedMoney / _betMoney,
                               GameUtils::getResultTypeString((GinResultType) userInfo._type, true, false),
                               _claimedMoney,
                               UserData::getInstance()->getMoney(),
                               _betMoney,
                               cocos2d::StringUtils::format("%d", _cityID),
                               s_startMatchCount,
                               userInfo._resultDetail._handPoints,
                               0,
                               0);
    }
    else if (_claimedMoney < 0)
    {
        s_loseNumber++;
        Firebase::logLoseResult(_claimedMoney / _betMoney,
                                GameUtils::getResultTypeString((GinResultType) userInfo._type, false, !(winInfo._type == (int) GinResultType::GIN_RESULT_TYPE_SCORE)),
                                _claimedMoney,
                                UserData::getInstance()->getMoney(),
                                _betMoney,
                                cocos2d::StringUtils::format("%d", _cityID),
                                s_startMatchCount,
                                userInfo._resultDetail._handPoints,
                                0,
                                0);
    }
    else
    {
        CCLOG("Error");
    }
    if (UserData::getInstance()->isEnableShowRatingPopup())
        MessageSender::requestCheckShowRatingPopup(VALUE_SHOW_RATING_AT_GAMEPLAY);
    if (UserData::getInstance()->isEnableLocalNotification())
        MessageSender::requestLocalNotificationData();
    UserData::getInstance()->saveWinOrLoseNumber();
    Firebase::logUserProperties(s_versionName, GameUtils::getMoneyRangeName(UserData::getInstance()->getMoney()), s_winNumber, s_loseNumber, s_winHitpotNumber, UserData::getInstance()->getLevel());
    CallToOtherLanguage::getInstance()->submitLeaderboardMoney(UserData::getInstance()->getMoney());
    CallToOtherLanguage::getInstance()->submitLeaderboardLevel(UserData::getInstance()->getLevel());
    if (CallToOtherLanguage::getInstance()->checkInternet())
        AdsManager::getInstance()->loadInterstitialAds();
}

void GPGinScene::handleCheatCardsResponse(const rapidjson::Document& data)
{
#if(USE_CHEAT_CARD == 1)
    CheatCardLayer::getInstance()->onCheatCardResponse(data);
#endif
}

void GPGinScene::onMatchCompleted()
{
    this->_canQuit = true;
    if ((checkShowInterstitialAdmob()) && _conditionRatingName.empty())
        AdsManager::getInstance()->showInterferingAds(UserData::getInstance()->isEnableInterferingAds(), true, INTERSTITIAL_ADMOB_CLOSE_ENDGAME);
    else
    {
        //        this->_controlCardLayer->setShowCardsVisible(false);
        //        this->_interfaceLayer->setScoreBoxesVisible(false);

        if (_interfaceLayer->getQuitState())
        {
            MessageSender::requestQuitInGame();
            reset();
        }
        else if (!checkGetMoney())
        {
            prepareNewMatch();
        }
    }
}

void GPGinScene::removeSlot(int slotIdx)
{
    if (_slotManager->isSlotValid(slotIdx))
    {
        this->_slotManager->removeSlot(slotIdx);
        this->_interfaceLayer->removeSlot(_slotManager->getSlotOrderIdx(slotIdx));
    }
}

void GPGinScene::handleGinCommandMatchInfoResponse(const rapidjson::Document& resp)
{
    const rapidjson::Value& listBetData = JsonHelper::getValue(resp, KEY_GIN_LIST_PLAYER_DATA);
    for (int i = 0; i < listBetData.Size(); i++)
    {
        const rapidjson::Value& betData      = listBetData[i];
        auto                  & betDataValue = _listBetData[i];
        betDataValue._slotIdx = JsonHelper::getInt(betData, KEY_GIN_PLAYER_INDEX, -1);
        betDataValue._isUser  = JsonHelper::getBool(betData, KEY_GIN_IS_USER, false);
        betDataValue._score   = JsonHelper::getInt(betData, KEY_GIN_SCORE, 0);
        betDataValue._bet     = JsonHelper::getDouble(betData, KEY_BET, 0);

        //        long money = JsonHelper::getDouble(betData, KEY_MONEY, 0);
        //        this->_slotManager->getSlotInfo(i)->setMoney(money);
        //        this->_interfaceLayer->updateMoney(i);
    }
    _round       = JsonHelper::getInt(resp, KEY_GIN_ROUND, 0);
    _targetScore = JsonHelper::getInt(resp, KEY_TARGET_SCORE, 0);
    _interfaceLayer->setRoundNumber(_round);

    const rapidjson::Value& jackpot = JsonHelper::getValue(resp, KEY_JACKPOT);
    this->_jackpotMoney = JsonHelper::getDouble(jackpot, KEY_JACKPOT_MONEY, 0);
    this->_jackpotRound = JsonHelper::getInt(jackpot, KEY_JACKPOT_ROUND, 0);
    this->_jackpotLock  = JsonHelper::getBool(jackpot, KEY_JACKPOT_LOCK, false);
    this->_interfaceLayer->setJackpotValue(this->_jackpotMoney);

    if (this->_jackpotLock == false)
    {
        this->_interfaceLayer->runJackpotBackgroundAnimation(true);
    }
    else
    {
        this->_interfaceLayer->runJackpotBackgroundAnimation(false);
    }

    if (_round == 1)
        _interfaceLayer->resetLayoutTargetScore(_targetScore, _listBetData);
    else
        _interfaceLayer->updateLayoutTargetScore(_targetScore, _listBetData);
}

const std::vector<cocos2d::Point>& GPGinScene::getThrowPos()
{
    return _interfaceLayer->getThrowPos();
}

const std::vector<cocos2d::Point>& GPGinScene::getDealPos()
{
    return _interfaceLayer->getDealPos();
}

const std::vector<cocos2d::Point>& GPGinScene::getShowCardPos()
{
    return _interfaceLayer->getShowCardPos();
}

const std::vector<cocos2d::Point>& GPGinScene::getShowStatusPos()
{
    return _interfaceLayer->getShowStatusPos();
}

const std::vector<cocos2d::Point>& GPGinScene::getHandCardPos()
{
    return _interfaceLayer->getHandCardPos();
}

cocos2d::Point GPGinScene::getSlotPos(int order)
{
    return _interfaceLayer->getSlotPos(order);
}

cocos2d::Vec2 GPGinScene::getRealPositionOfDrawCardAtBase()
{
    return _interfaceLayer->getRealCardOnDrawCardPosition();
}

int GPGinScene::getUpCardValue()
{
    return _upcardValue;
}

void GPGinScene::setVisibleScoreBox(bool show, int value, int score, int slotIdx, const cocos2d::Vec2& position, GinResultType type)
{
    _interfaceLayer->setVisibleScoreBox(show, value, score, slotIdx, position, type);
}

void GPGinScene::updateScoreBox(int value, int score, int slotidx, const cocos2d::Vec2& position, GinResultType type)
{
    _interfaceLayer->updateScoreBox(value, score, slotidx, position, type);
}

int GPGinScene::getScoreBoxValue(int slotIdx)
{
    return _interfaceLayer->getScoreBoxValue(slotIdx);
}

void GPGinScene::updateDeadwoodBox(int score, int knockScore, int slotIdx)
{
    _interfaceLayer->updateDeadwoodBox(score, knockScore, slotIdx);
}

cocos2d::Size GPGinScene::getLayoutCardSize(int orderIdx)
{
    return _interfaceLayer->getLayoutCardSize(orderIdx);
}

void GPGinScene::registerHandleMessage(MessageID messageId)
{
    MessageHandler::getInstance()->registerHandleMessage(this, messageId, MESSAGEHANDLE_SELECTOR(GPGinScene::onServerResponse));
}

void GPGinScene::onServerResponse(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE:
            onGetLocalNotificationData(data);
            break;
        case MessageID::MESSAGE_PROFILE_RESPONSE:
            handleUserProfile(data);
            break;
        case MessageID::MESSAGE_QUIT_IN_GAME_RESPONSE:
            handleQuitInGame(data);
            break;
        case MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE:
            handleSaviorGoldBonusPopup(data);
            break;
        case MessageID::MESSAGE_BONUS_RESPONSE:
            handleGoldBonus(data);
            break;
        case MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE:
            handleSaviorGoldBonus(data);
            break;
        case MessageID::MESSAGE_JOIN_GAME_RESPONSE:
            handleRejoinGame(data);
            break;
        case MessageID::MESSAGE_USER_DATA_RESPONSE:
            handleUserData(data);
            break;
        case MessageID::MESSAGE_TICKET_RESPONSE:
            handleTicker(data);
            break;
        case MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE:
            handleCheckRatingPopup(data);
            break;
        case MessageID::MESSAGE_BOT_JOIN_RESPONSE:
            handleBotJoin(data);
            break;
        case MessageID::MESSAGE_START_MATCH_RESPONSE:
            handleGinCommandMatchInfoResponse(data);
            break;
        case MessageID::MESSAGE_PLAYER_HAND_CARD_RESPONSE:
            handlePlayerHandCard(data);
            break;
        case MessageID::MESSAGE_BOT_HAND_CARD_RESPONSE:
            handleBotHandCard(data);
            break;
        case MessageID::MESSAGE_PLAYER_BEGIN_TURN_RESPONSE:
            handleBeginTurn(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_BEGIN_RESPONSE:
            handlePlayerTurnStepPassingDemandServer(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_PASS_ACTION_RESPONSE:
            handlePlayerTurnStepPassing(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_BEGIN_RESPONSE:
            handlePlayerTurnStepGetCardDemandServer(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_GET_CARD_ACTION_RESPONSE:
            handlePlayerTurnStepGetCard(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_BEGIN_RESPONSE:
            handlePlayerThrowCardDemandServer(data);
            break;
        case MessageID::MESSAGE_PLAYER_TURN_STEP_THROW_CARD_ACTION_RESPONSE:
            handlePlayerThrowCard(data);
            break;
        case MessageID::MESSAGE_END_MATCH_RESPONSE:
            handleEndMatch(data);
            break;
        case MessageID::MESSAGE_PLAYER_END_TURN_RESPONSE:
            handleEndTurn(data);
            break;
        case MessageID::MESSAGE_CHEAT_CARD_RESPONSE:
            handleCheatCardsResponse(data);
            break;
        case MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE:
            handleVideoRewardEnable(data);
            break;
        case MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE:
            handleVideoRewardInfo(data);
            break;
        case MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE:
            handleVideoAdsMoney(data);
            break;
        case MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE:
            handleHourlyBonusData(data);
            break;
        case MessageID::MESSAGE_PLAYER_CUSTOM_MELDS_RESPONSE:
            handleManualMeldData(data);
            break;
        default:
            break;
    }
}

void GPGinScene::onNotify(void* sender, int id, const cocos2d::Value& data)
{
    switch (id)
    {
        case EVENT_CHANGE_LANGUAGE:
        {
            ValueMap arr  = data.asValueMap();
            Language lang = (Language) arr[LANGUAGE_TYPE_KEY].asInt();
            this->changeLanguage(lang);
            break;
        }
        case EVENT_INTERSTITIAL_HIDE:
        {
            ValueMap    arr          = data.asValueMap();
            std::string nameFunction = arr[AD_NAME_FUNCTION_KEY].asString();
            bool        adClosed     = arr[AD_CLOSED_KEY].asBool();
            if (nameFunction == INTERSTITIAL_ADMOB_CLOSE_ENDGAME)
            {
                if (adClosed)
                {
                    Firebase::logShowFullAds();
                    this->resetShowInterstitialAdmob();
                }

                if (!checkGetMoney())
                {
                    //                    this->_controlCardLayer->setShowCardsVisible(false);
                    //                    this->_interfaceLayer->setScoreBoxesVisible(false);
                    prepareNewMatch();
                }
            }
            else if (nameFunction == INTERSTITIAL_ADMOB_EXIT_INGAME)
            {
                if (!_exited)
                {
                    _exited = true;
                    SceneManager::getInstance()->openScene(SceneID::LOBBY_SCENE, cocos2d::Value());
                }
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

bool GPGinScene::checkGetMoney()
{
    double money = UserData::getInstance()->getMoney();
    if (money >= USER_MINIMUM_MONEY)
        return false;
    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=] {
        MessageSender::requestShowGoldBonusPopup();
    }), nullptr));
    return true;
}

void GPGinScene::prepareNewMatch()
{
    if (!_conditionRatingName.empty())
    {
        GameUtils::showRatingAndReview(_stateRating, _conditionRatingName);
        _stateRating         = 0;
        _conditionRatingName = "";
    }
    MessageSender::requestVideoRewardEnable();
    _interfaceLayer->showButtonResult(false);
#if USE_AUTO_PLAY_CARD == 0
    _interfaceLayer->showLayoutContinue(true);
#else
    onContinue2();
#endif
    MessageSender::requestUserProfile("ingame", s_versionCode);
    if (s_startMatchCount % MATCH_COUNT_TO_CHANGE_MUSIC == 0 && UserData::getInstance()->getSoundBackground())
    {
        if (UserData::getInstance()->getSoundBackground())
        {
            SoundManager::getInstance()->stopBackgroundMusicAuto();
            SoundManager::getInstance()->playBackgroundMusicAuto();
        }
        else
        {
            GameUtils::checkToRestoreSoundBackground();
        }
    }
}

void GPGinScene::setBetMoney(double money)
{
    _betMoney = money;
    this->_interfaceLayer->setBetMoney(this->_betMoney);
}

void GPGinScene::waitToStart(float time)
{
    this->_canQuit = true;
    runAction(Sequence::create(DelayTime::create(time), CallFunc::create([=] {
        if (_interfaceLayer->getQuitState())
        {
            MessageSender::requestQuitInGame();
            reset();
        }
        else if (!checkGetMoney())
        {
            startGame();
        }
    }), nullptr));
}

void GPGinScene::startGame()
{
    this->_canQuit = false;
    this->reset();
    auto start = [=]() {
        MessageSender::requestStartGame();
    };
    start();
}

void GPGinScene::handleBotJoin(const rapidjson::Document& data)
{
    const rapidjson::Value& listBot = JsonHelper::getValue(data, KEY_BOT_LIST);
    for (int i = 0; i < listBot.Size(); i++)
    {
        int    slotIdx = JsonHelper::getInt(listBot[i], KEY_BOT_INDEX, -2);
        string name    = JsonHelper::getString(listBot[i], KEY_BOT_NAME, "");
        string avatar  = JsonHelper::getString(listBot[i], KEY_BOT_AVATAR, "");
        double money   = JsonHelper::getDouble(listBot[i], KEY_BOT_MONEY, 0);

        GinSlotInfo slotInfo;
        slotInfo.setSlotIdx(slotIdx + 1);
        slotInfo.setUserName(name);
        slotInfo.setAvatarLink(avatar);
        slotInfo.setMoney(money);
        slotInfo.setReady(true);
        slotInfo.setDone(false);

        this->_slotManager->addSlot(slotInfo);
        this->_interfaceLayer->addSlot(this->_slotManager->getSlotInfo().at(slotInfo.getSlotIdx()));
    }
}

void GPGinScene::handleVideoRewardInfo(const rapidjson::Document& data)
{
    VideoRewardsPopup* popup = BoardManager::createVideoRewardsPopup();
    popup->attach(this, kAboveForeground);
    popup->setData(data, true);
}

bool GPGinScene::checkShowInterstitialAdmob()
{
    s_countShowInterstitial++;
    if (_adsExit)
    {
        if (s_countShowInterstitial >= s_showInterstitial && UserData::getInstance()->getMoney() > 0 && _interfaceLayer && !_interfaceLayer->getQuitState())
            return true;
    }
    else
    {
        if (!_showAdsForce && RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_ADS_SHOW_FORCE_ENABLE, false) && RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_ADS_SHOW_FORCE_APPLY_LEVEL, 0) <= UserData::getInstance()->getLevel() &&
            GameUtils::randomDoubleInRange(0, 100) >= RemoteConfig::getDouble(REMOTECONFIG_FUNCTION_ADS_SHOW_FORCE_RATE, 100) && UserData::getInstance()->getMoney() > 0)
        {
            _showAdsForce = true;
            return true;
        }
        if (s_countShowInterstitial >= s_showInterstitial && UserData::getInstance()->getMoney() > 0)
            return true;
    }
    return false;
}

void GPGinScene::resetShowInterstitialAdmob()
{
    _showAdsForce           = false;
    s_countShowInterstitial = 0;
    s_showInterstitial      = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_ADS_SHOW_INGAME_FREQUENCY, 2);
}

void GPGinScene::handleVideoRewardEnable(const rapidjson::Document& data)
{
    Node* node = getChildByTag((int) BoardID::GET_MONEY_TO_PLAY_BOARD);
    bool hasGetMoneyToPLayPopup = (node != nullptr && node->getParent() != nullptr && dynamic_cast<GetMoneyToPlayPopup*>(node) != nullptr);
    if (!hasGetMoneyToPLayPopup)
    {
        _interfaceLayer->updateLayoutRewards(data);
    }
    else
    {
        GetMoneyToPlayPopup* popup = dynamic_cast<GetMoneyToPlayPopup*>(node);
        bool   result      = JsonHelper::getBool(data, KEY_RESULT, false);
        double availableAt = JsonHelper::getDouble(data, KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, 0);
        popup->onVideoRewardEnable(result, availableAt);
    }
}

void GPGinScene::handleVideoAdsMoney(const rapidjson::Document& data)
{
    _interfaceLayer->onVideoAdsMoney(data);
}

void GPGinScene::handleHourlyBonusData(const rapidjson::Document& data)
{
    _interfaceLayer->onHourlyBonusData(data);
}

void GPGinScene::handleManualMeldData(const rapidjson::Document& data)
{
    _controlCardLayer->handleManualMeldResponse(GPGinSlotManager::getInstance()->getSlotOrderIdx(JsonHelper::getInt(data, KEY_GIN_PLAYER_INDEX, 0)),
                                                JsonHelper::getInt(data, KEY_GIN_ACTION_TYPE, (int) ActionType::K_NONE),
                                                JsonHelper::getInt(data, KEY_KNOCK_POINTS, -1));
}

bool GPGinScene::inRound()
{
    return _inRound;
}

void GPGinScene::showRoundPointPopup(const std::vector<PlayerResultData>& listPlayerResultData/* = std::vector<PlayerResultData>()*/)
{
    RoundPointPopup* gameResultPopup = BoardManager::createRoundPointPopup();
    gameResultPopup->setRoomInfo(_roomName, _betMoney);
    if (listPlayerResultData.size() > 0)
    {
        RoundPointPopup::_listUserDataCached = listPlayerResultData;
        gameResultPopup->loadData(listPlayerResultData);
        gameResultPopup->setOnExitCallback([=]() {
            this->onMatchCompleted();
        });
    }
    else
    {
        gameResultPopup->loadData(RoundPointPopup::_listUserDataCached);
        gameResultPopup->setOnExitCallback(nullptr);
    }
    this->addChild(gameResultPopup, kAboveForeground);
}

void GPGinScene::showStatus(int orderIdx, FightStatus status, bool isChallenge/* = true*/)
{
    _interfaceLayer->showStatus(orderIdx, status);

    _interfaceLayer->showTextDropArea(false, orderIdx);
    if (orderIdx == MAIN_USER_SLOT_INDEX && isChallenge)
        Firebase::logDoAction(GameUtils::getFightStatusString(status), _controlCardLayer->getCurrentScore());
}

Sprite* GPGinScene::getLastDrawCard()
{
    return _interfaceLayer->getLastDrawCard();
}

void GPGinScene::showGin(int orderIdx, cocos2d::Vec2 position)
{
    _effectLayer->showEffectRankGinOnTable(position, orderIdx);
    _interfaceLayer->showTextDropArea(false, orderIdx);
}

std::vector<std::vector<int>> GPGinScene::getListMeldOnHandMainUser(bool isChallenged)
{
    std::vector<std::vector<int>> listMeld;
    if (_interfaceLayer->getNumDrawCard() == 0 || isChallenged)
        listMeld = _controlCardLayer->getListMeldOnHandMainUser();
    return listMeld;
}

std::vector<std::vector<int>> GPGinScene::getListMeldOnHandMainUserBySoftGroup()
{
    std::vector<std::vector<int>> listMeld;
    //    if (_interfaceLayer->getNumDrawCard() == 0)
    listMeld = _controlCardLayer->getListMeldOnHandMainUserBySoftGroup();
    return listMeld;
}

void GPGinScene::prepareTableDeal()
{
    this->reset();
    this->_dealCardLayer->reset();
    this->_dealCardLayer->setVisible(true);
    this->_interfaceLayer->setNumDrawCard(MAXIMUM_NUMBER_CARD, false, true);
    this->_interfaceLayer->setVisibleLayoutDrawCard(true);
    this->_interfaceLayer->setAllUserTurnOff();
}

const BetData& GPGinScene::getBetData(int slotIdx)
{
    for (const BetData& betData : _listBetData)
    {
        if (betData._slotIdx == slotIdx)
            return betData;
    }
    static BetData data;
    return data;
}

BetData& GPGinScene::getBetDataRef(int slotIdx)
{
    for (BetData& betData : _listBetData)
        if (betData._slotIdx == slotIdx)
            return betData;
    static BetData data;
    return data;
}

double GPGinScene::getTotalBet()
{
    double totalBet = 0;
    for (const BetData& betData : _listBetData)
        totalBet += betData._bet;
    return totalBet;
}

void GPGinScene::onGetLocalNotificationData(const rapidjson::Document& data)
{
    int result = JsonHelper::getInt(data, KEY_RESULT, 0);
    if (result == RESULT_SUCCESS)
    {
        const rapidjson::Value& listNotificationData = JsonHelper::getValue(data, KEY_LOCAL_NOTIFICATION_NOTIFICATIONS);
        UserData::getInstance()->parseLocalNotificationData(listNotificationData);
    }
}

void GPGinScene::hideLayerForQuit()
{
    auto      children = this->getChildren();
    for (auto child : children)
    {
        child->stopAllActions();
        child->setVisible(false);
    }
    _interfaceLayer->setVisible(true);
    _interfaceLayer->hideLayerForQuit();
}

void GPGinScene::prepareQuit()
{
    hideLayerForQuit();
}

void GPGinScene::updateThrowCardsPopup()
{
    Node* node = getChildByTag((int) BoardID::THROW_CARD_DETAIL_POPUP);
    if (node != nullptr && this->_controlCardLayer)
    {
        ThrowCardDetailPopup* popup = dynamic_cast<ThrowCardDetailPopup*>(node);
        if (popup)
            popup->loadData(this->_controlCardLayer->getListThrowCardsValue());
    }
}

void GPGinScene::showMatchEndStatus(GinMatchEndStatus status, cocos2d::Point positionOffset)
{
    _interfaceLayer->showMatchEndStatus(status, positionOffset);
}

int GPGinScene::getRule()
{
    return this->_rule;
}

int GPGinScene::getTargetScore()
{
    return this->_targetScore;
}

void GPGinScene::hideAllPassEffect()
{
    for(auto data : this->_mapStatusPassShow)
    {
        if(data.second == true)
        {
            this->_interfaceLayer->hideStatus(data.first);
            this->_mapStatusPassShow[data.first] = false;
        }
    }
}

void GPGinScene::hideAllPassEffectExcept(int currentIdx)
{
    for(auto data : this->_mapStatusPassShow)
    {
        if(data.first != currentIdx && data.second == true)
        {
            this->_interfaceLayer->hideStatus(data.first);
            this->_mapStatusPassShow[data.first] = false;
        }
    }
}
