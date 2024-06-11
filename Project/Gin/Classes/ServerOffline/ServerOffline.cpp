#include "ServerOffline.h"
#include "cocos2d.h"
#include "ServerOfflineDefines.h"
#include "InGameServerOfflineDefines.h"
#include "Network/MessageHandler.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/SvrUserProfile.h"
#include "AdapterClientServer/ServerManager.h"
#include "Helper/GameUtils.h"
#include "ServerOffline/SvrHelper.h"
#include "Define/GameDefine.h"
#include "base/base64.h"
#include "FirebaseGG/RemoteConfig.h"
#include "ServerOffline/Feature/SvrLocalNotification.h"
#include "ServerOffline/Feature/SvrTimeBonus.h"
#include "ServerOffline/Feature/SvrHourlyBonus.h"
#include "ServerOffline/Feature/SvrMailbox.h"
#include "ServerOffline/Feature/SvrRating.h"
#include "ServerOffline/Feature/SvrLevel.h"
#include "ServerOffline/Feature/SvrVideoAds.h"
#include "ServerOffline/Feature/SvrLog.h"
#include "ServerOffline/Feature/SvrShop.h"
#include "ServerOffline/Rules/RuleCommon.h"

#define MONEY_DEVIDE 1
#define MONEY_LIMIT_DEVIDE 0

ServerOffline* ServerOffline::_serverOfflineInstance = nullptr;

bool  ServerOffline::_getBonusGold                  = false;
int   ServerOffline::_consecutiveLose               = 0;
int   ServerOffline::_consecutiveWin                = 0;
float ServerOffline::_winBetMultiple                = 0;
int   ServerOffline::_matchCountPerOneTimeEnterRoom = 0;
int   ServerOffline::_winScoreGapMultiply           = 0;
int   ServerOffline::_winJackpot                    = 0;
int   ServerOffline::_gameCountPerOneTimeEnterRoom  = 0;
int   ServerOffline::_winGameRoundCount             = 0;
int   ServerOffline::_winGame                       = 0;

ServerOffline::ServerOffline()
{
    SvrHourlyBonus::initHourlyBonus();
    SvrLevel::initLevelExp();
    SvrUserProfile::getBetData();
    SvrUserProfile::updateRawData();
    SvrUserProfile::getTargetScoreData();
    SvrUserProfile::initMoneyRangeName();
}

ServerOffline::~ServerOffline()
{
    MessageHandler::getInstance()->unregisterHandleMessage(this);
    if (_currentGameManager != nullptr)
    {
        _currentGameManager->endGame();
        _currentGameManager = nullptr;
    }
}

bool ServerOffline::init()
{
    if (!cocos2d::Node::init())
        return false;

    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_PROFILE_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_TICKET_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_JOIN_GAME_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_UNLOCK_CITY_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_UNLOCK_TABLE_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_USER_DATA_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_NAME_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHANGE_USER_AVATAR_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_BONUS_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SAVIOR_BONUS_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SAVIOR_POPUP_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_ADS_MONEY_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOG_IN_OFFLINE_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOAD_SAVED_GAME_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_GET_HOURLY_BONUS_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_QUIT_IN_GAME_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_RATING_POPUP_SUBMIT_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_INFO_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_CLAIM_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_VIDEO_REWARD_SKIP_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_SUGGEST_UPDATE_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_MAILBOX_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    MessageHandler::getInstance()->registerHandleMessage(this, MessageID::MESSAGE_LOCAL_NOTIFICATION_REQUEST, MESSAGEHANDLE_SELECTOR(ServerOffline::onMessage));
    _scheduler->scheduleUpdate(this, cocos2d::Scheduler::PRIORITY_NON_SYSTEM_MIN, false);
    _currentGameManager = nullptr;
    return true;
}

void ServerOffline::resetUserData()
{
    if (cocos2d::UserDefault::getInstance()->getBoolForKey(FIRST_TIME_IN_GAME, false) == false)
    {
        SvrShop::initCities();
        SvrShop::initTables();
        SvrUserProfile::initUserMoney();
        SvrShop::initUserCities();
        SvrShop::initUserTables();
        SvrUserProfile::initUserName();
        SvrUserProfile::initUserAvatar();
        SvrVideoAds::initWatchVideos();
        SvrTimeBonus::initVideoRewardDatas();
        SvrRating::initStateRating();
        cocos2d::UserDefault::getInstance()->setBoolForKey(FIRST_TIME_IN_GAME, true);
    }
}

void ServerOffline::onRequestBuyNewCity(int id)
{
    std::pair<int, std::pair<std::vector<SvrCity>, std::vector<SvrTable>>> result = SvrShop::buyCity(id);

    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNLOCK_CITY_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, result.first, doc.GetAllocator());
    doc.AddMember(KEY_PLACE_ID, id, doc.GetAllocator());
    if (result.first == RESULT_BUY_SUCCESS)
    {
        std::vector<SvrTable> listTable = SvrShop::getTables();
        rapidjson::Value      arrTable(rapidjson::kArrayType);
        for (int              i         = 0; i < listTable.size(); i++)
        {
            rapidjson::Value table;
            table.SetObject();
            table.AddMember(KEY_TABLE_ID, listTable[i].id, doc.GetAllocator());
            table.AddMember(KEY_TABLE_NAME, rapidjson::Value().SetString(listTable[i].name.c_str(), doc.GetAllocator()), doc.GetAllocator());
            table.AddMember(KEY_TABLE_PRICE, listTable[i].price, doc.GetAllocator());
            table.AddMember(KEY_TABLE_BONUS, listTable[i].bonus, doc.GetAllocator());

            int status = listTable[i].status;
            if (status == STATUS_ALREADY_BUY)
                table.AddMember(KEY_TABLE_STATUS, status, doc.GetAllocator());
            else if (!SvrShop::isMeetTableCondition(listTable[i].id))
                table.AddMember(KEY_TABLE_STATUS, STATUS_LOCK, doc.GetAllocator());
            arrTable.PushBack(table, doc.GetAllocator());
        }
        doc.AddMember(KEY_LIST_TABLES, arrTable, doc.GetAllocator());

        std::vector<SvrCity> listCity = result.second.first;
        rapidjson::Value     arrCity(rapidjson::kArrayType);
        for (int             i        = 0; i < listCity.size(); i++)
            arrCity.PushBack(listCity[i].id, doc.GetAllocator());
        doc.AddMember(KEY_LIST_PLACES, arrCity, doc.GetAllocator());
        SvrLog::logEarnSpendMoney();
    }
    doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestBuyNewTable(int id)
{
    std::pair<int, std::vector<SvrTable>> result = SvrShop::buyTable(id);
    rapidjson::Document                   doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNLOCK_TABLE_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, result.first, doc.GetAllocator());
    doc.AddMember(KEY_TABLE_ID, id, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
    if (result.first == RESULT_BUY_SUCCESS)
    {
        std::vector<SvrTable> listTable = SvrShop::getTables();
        rapidjson::Value      arrTable(rapidjson::kArrayType);
        for (int              i         = 0; i < listTable.size(); i++)
        {
            rapidjson::Value table;
            table.SetObject();
            table.AddMember(KEY_TABLE_ID, listTable[i].id, doc.GetAllocator());
            table.AddMember(KEY_TABLE_NAME, rapidjson::Value().SetString(listTable[i].name.c_str(), doc.GetAllocator()), doc.GetAllocator());
            table.AddMember(KEY_TABLE_PRICE, listTable[i].price, doc.GetAllocator());
            table.AddMember(KEY_TABLE_BONUS, listTable[i].bonus, doc.GetAllocator());
            int status = listTable[i].status;
            if (status == STATUS_ALREADY_BUY)
                table.AddMember(KEY_TABLE_STATUS, status, doc.GetAllocator());
            else if (!SvrShop::isMeetTableCondition(listTable[i].id))
                table.AddMember(KEY_TABLE_STATUS, STATUS_LOCK, doc.GetAllocator());
            arrTable.PushBack(table, doc.GetAllocator());
        }
        doc.AddMember(KEY_LIST_TABLES, arrTable, doc.GetAllocator());
        SvrLog::logEarnSpendMoney();
    }
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestTicket(int cityId, int tableId)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_TICKET_RESPONSE, doc.GetAllocator());
    double money    = SvrUserProfile::getMoney();
    double maxMoney = round(money / 2);
    if (maxMoney >= MONEY_LIMIT_DEVIDE)
        maxMoney /= MONEY_DEVIDE;
    maxMoney        = maxMoney < 1 ? 1 : (long long) (maxMoney);
    double   minMoney           = 1;
    int      defaultTargetScore = 0;
    if (SvrShop::isBuyCity(cityId) && SvrShop::isBuyTable(tableId) && maxMoney > 0 && minMoney > 0 && maxMoney >= minMoney)
    {
        SvrCity city     = SvrShop::getCityByID(cityId);
        auto    gameRule = (RuleConstant::GameRule) cityId;
        if ((gameRule != RuleConstant::GameRule::OKLAHOMA_GIN) || (gameRule == RuleConstant::GameRule::OKLAHOMA_GIN && money >= city.requiredMoney))
        {
            doc.AddMember(KEY_JACKPOT, city.jackpotRatioBet, doc.GetAllocator());
            doc.AddMember(KEY_RESULT, RESULT_SUCCESS, doc.GetAllocator());
        }
        else if (money < city.requiredMoney)
        {
            doc.AddMember(KEY_JACKPOT, city.requiredMoney, doc.GetAllocator());
            doc.AddMember(KEY_RESULT, SVR_RESULT_FAIL_JACKPOT_MINIMUM_MONEY, doc.GetAllocator());
        }
        else
            doc.AddMember(KEY_RESULT, RESULT_FAIL, doc.GetAllocator());
        defaultTargetScore = s_targetScoreDataRange[0][cityId][0]._targetScore;
    }
    else
        doc.AddMember(KEY_RESULT, RESULT_FAIL, doc.GetAllocator());
    SvrCity  cityT              = SvrShop::getCityByID(cityId);
    SvrTable tableT             = SvrShop::getTableByID(tableId);
    doc.AddMember(KEY_PLACE_ID, cityT.id, doc.GetAllocator());
    doc.AddMember(KEY_MIN_BET, minMoney, doc.GetAllocator());
    doc.AddMember(KEY_MAX_BET, maxMoney, doc.GetAllocator());
    doc.AddMember(KEY_DEFAULT_TARGET_SCORE, defaultTargetScore, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestJoinCity(const rapidjson::Document& data)
{
    int                 cityId      = JsonHelper::getInt(data, KEY_PLACE_ID, -1);
    int                 tableId     = JsonHelper::getInt(data, KEY_TABLE_ID, -1);
    double              bet         = JsonHelper::getDouble(data, KEY_BET, -1);
    int                 targetScore = JsonHelper::getInt(data, KEY_TARGET_SCORE, -1);
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_JOIN_GAME_RESPONSE, doc.GetAllocator());
    bet = (long long) (bet);
    double money    = SvrUserProfile::getMoney();
    double maxMoney = money;
    if (maxMoney >= MONEY_LIMIT_DEVIDE)
        maxMoney /= MONEY_DEVIDE;
    maxMoney        = maxMoney < 1 ? 1 : (long long) (maxMoney);
    double minMoney = 1;
    if (SvrShop::isBuyCity(cityId) && SvrShop::isBuyTable(tableId) && bet <= SvrUserProfile::getMoney() && maxMoney > 0 && minMoney > 0 && maxMoney >= minMoney && maxMoney >= bet && minMoney <= bet)
    {
        SvrCity  city  = SvrShop::getCityByID(cityId);
        SvrTable table = SvrShop::getTableByID(tableId);

        doc.AddMember(KEY_PLACE_ID, cityId, doc.GetAllocator());
        doc.AddMember(KEY_TABLE_ID, tableId, doc.GetAllocator());
        doc.AddMember(KEY_BET, bet, doc.GetAllocator());
        doc.AddMember(KEY_TARGET_SCORE, targetScore, doc.GetAllocator());
        auto gameRule = (RuleConstant::GameRule) cityId;
        if ((gameRule != RuleConstant::GameRule::OKLAHOMA_GIN) || (gameRule == RuleConstant::GameRule::OKLAHOMA_GIN && money >= city.requiredMoney))
        {
            if (_currentGameManager != nullptr)
            {
                _currentGameManager->endGame();
                _currentGameManager = nullptr;
            }
            _currentGameManager          = GinGameMatchManager::createInstance();
            GinGameMatchManager::_server = this;
            GameMatchBundle bundle;
            bundle.cityId          = cityId;
            bundle.tableId         = tableId;
            bundle.price           = bet;
            bundle.numPlayer       = 1;
            bundle.numBot          = city.numBot;
            bundle.bonus           = SvrHelper::getMatchBonus(city.bonus, table.bonus);
            bundle.jackpotRatioBet = _currentGameManager->getJackpotMultiplyValue(cityId, targetScore);
            bundle.targetScore     = targetScore;
            _currentGameManager->joinGame(bundle);
            doc.AddMember(KEY_MAX_MULTIPLY_VALUE, _currentGameManager->getMaxMultiplyValue(), doc.GetAllocator());
            doc.AddMember(KEY_JACKPOT, bundle.jackpotRatioBet, doc.GetAllocator());
            doc.AddMember(KEY_RESULT, RESULT_SUCCESS, doc.GetAllocator());
        }
        else if (money < city.requiredMoney)
        {
            doc.AddMember(KEY_JACKPOT, city.requiredMoney, doc.GetAllocator());
            doc.AddMember(KEY_RESULT, SVR_RESULT_FAIL_JACKPOT_MINIMUM_MONEY, doc.GetAllocator());
        }
    }
    else
    {
        doc.AddMember(KEY_RESULT, RESULT_FAIL, doc.GetAllocator());
    }
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestUserProfileData(const rapidjson::Document& data)
{
    int versionCode = JsonHelper::getInt(data, KEY_VERSION_CODE, 0);
    SvrUserProfile::setVersionCode(versionCode);
    sendHourlyBonusData();

    double                money     = SvrUserProfile::getMoney();
    std::string           name      = SvrUserProfile::getName();
    std::string           path      = SvrUserProfile::getAvatar();
    std::vector<SvrCity>  listCity  = SvrShop::getCities();
    std::vector<SvrTable> listTable = SvrShop::getTables();
    int                   level     = SvrLevel::getLevel();
    int                   levelExp  = SvrLevel::getLevelExp();
    int                   nextExp   = SvrLevel::getLevelExp(level + 1).second;
    rapidjson::Document   doc;
    doc.SetObject();

    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_PROFILE_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, money, doc.GetAllocator());
    doc.AddMember(KEY_NAME, rapidjson::Value().SetString(name.c_str(), doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember(KEY_LEVEL, level, doc.GetAllocator());
    doc.AddMember(KEY_LEVELEXP, levelExp, doc.GetAllocator());
    doc.AddMember(KEY_NEXTLEVELEXP, nextExp, doc.GetAllocator());
    doc.AddMember(KEY_INTERFERING_ADS, SvrUserProfile::getInterferingAdsEnable(), doc.GetAllocator());
    doc.AddMember(KEY_AVATAR, rapidjson::Value().SetString(path.c_str(), doc.GetAllocator()), doc.GetAllocator());
    rapidjson::Value arrCity(rapidjson::kArrayType);
    for (const auto& svrCity : listCity)
    {
        rapidjson::Value city;
        city.SetObject();
        city.AddMember(KEY_PLACE_ID, svrCity.id, doc.GetAllocator());
        city.AddMember(KEY_PLACE_STATUS, svrCity.status, doc.GetAllocator());
        city.AddMember(KEY_PLACE_NAME, rapidjson::Value().SetString(svrCity.name.c_str(), doc.GetAllocator()), doc.GetAllocator());
        city.AddMember(KEY_PLACE_PRICE, svrCity.price, doc.GetAllocator());
        rapidjson::Value cityArr(rapidjson::kArrayType);
        for (const auto& require : svrCity.listRequireCity)
            cityArr.PushBack(require, doc.GetAllocator());
        city.AddMember(KEY_PLACE_CONDITION, cityArr, doc.GetAllocator());
        city.AddMember(KEY_PLACE_JACKPOT_RATIO_BET, svrCity.jackpotRatioBet, doc.GetAllocator());
        city.AddMember(KEY_PLACE_REQUIRED_MONEY, svrCity.requiredMoney, doc.GetAllocator());
        arrCity.PushBack(city, doc.GetAllocator());
    }
    doc.AddMember(KEY_LIST_PLACES, arrCity, doc.GetAllocator());
    rapidjson::Value arrTable(rapidjson::kArrayType);
    for (const auto& svrTable : listTable)
    {
        rapidjson::Value table;
        table.SetObject();
        table.AddMember(KEY_TABLE_ID, svrTable.id, doc.GetAllocator());
        table.AddMember(KEY_TABLE_NAME, rapidjson::Value().SetString(svrTable.name.c_str(), doc.GetAllocator()), doc.GetAllocator());
        table.AddMember(KEY_TABLE_PRICE, svrTable.price, doc.GetAllocator());
        table.AddMember(KEY_TABLE_BONUS, svrTable.bonus, doc.GetAllocator());
        int status = svrTable.status;
        if (status == STATUS_ALREADY_BUY)
            table.AddMember(KEY_TABLE_STATUS, status, doc.GetAllocator());
        else if (!SvrShop::isMeetTableCondition(svrTable.id))
            table.AddMember(KEY_TABLE_STATUS, STATUS_LOCK, doc.GetAllocator());
        rapidjson::Value cityArr(rapidjson::kArrayType);
        for (const auto& require : svrTable.listRequireCity)
            cityArr.PushBack(require, doc.GetAllocator());
        table.AddMember(KEY_PLACE_CONDITION, cityArr, doc.GetAllocator());
        arrTable.PushBack(table, doc.GetAllocator());
    }
    doc.AddMember(KEY_LIST_TABLES, arrTable, doc.GetAllocator());
    std::string where = JsonHelper::getString(data, KEY_WHERE, "");
    if (where == "lobby")
        SvrMailbox::_listMailbox = SvrMailbox::getMailboxList();
    rapidjson::Value arrMailbox(rapidjson::kArrayType);
    for (const auto& mailItem : SvrMailbox::_listMailbox)
    {
        rapidjson::Value mail;
        mail.SetObject();
        mail.AddMember(KEY_MAILBOX_ID, rapidjson::Value().SetString(mailItem._id.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_TYPE, rapidjson::Value().SetString(mailItem._type.c_str(), doc.GetAllocator()), doc.GetAllocator());
        arrMailbox.PushBack(mail, doc.GetAllocator());
    }
    doc.AddMember(KEY_MAILBOX_LIST, arrMailbox, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
    SvrLog::logEarnSpendMoney();
}

void ServerOffline::onRequestUserData()
{
    double              money    = SvrUserProfile::getMoney();
    std::string         name     = SvrUserProfile::getName();
    std::string         path     = SvrUserProfile::getAvatar();
    int                 level    = SvrLevel::getLevel();
    int                 levelExp = SvrLevel::getLevelExp();
    int                 nextExp  = SvrLevel::getLevelExp(level + 1).second;
    rapidjson::Document doc;
    doc.SetObject();

    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_USER_DATA_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, money, doc.GetAllocator());
    doc.AddMember(KEY_NAME, rapidjson::Value().SetString(name.c_str(), doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember(KEY_AVATAR, rapidjson::Value().SetString(path.c_str(), doc.GetAllocator()), doc.GetAllocator());
    doc.AddMember(KEY_LEVEL, level, doc.GetAllocator());
    doc.AddMember(KEY_LEVELEXP, levelExp, doc.GetAllocator());
    doc.AddMember(KEY_NEXTLEVELEXP, nextExp, doc.GetAllocator());
    doc.AddMember(KEY_INTERFERING_ADS, SvrUserProfile::getInterferingAdsEnable(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
    SvrLog::logEarnSpendMoney();
}

void ServerOffline::onRequestChangeName(const std::string& name)
{
    SvrUserProfile::setName(name);
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHANGE_USER_NAME_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, 1, doc.GetAllocator());
    doc.AddMember(KEY_NAME, rapidjson::Value().SetString(name.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestChangeAvatar(const std::string& path)
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHANGE_USER_AVATAR_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, 1, doc.GetAllocator());
    doc.AddMember(KEY_AVATAR, rapidjson::Value().SetString(path.c_str(), doc.GetAllocator()), doc.GetAllocator());
    SvrUserProfile::setAvatar(path);
    ServerManager::getInstance()->sendMessage(doc);
}

ServerOffline* ServerOffline::getInstance()
{
    if (_serverOfflineInstance == nullptr)
    {
        _serverOfflineInstance = ServerOffline::create();
        _serverOfflineInstance->retain();
    }
    return _serverOfflineInstance;
}

void ServerOffline::onRequestBonus()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_BONUS_RESPONSE, doc.GetAllocator());
    int result = RESULT_FAIL;
    if (SvrVideoAds::isReachVideoWatchPerDay() == false)
    {
        result            = RESULT_SUCCESS;
        SvrVideoAds::updateWatchVideoCount();
        double moneyBonus = SvrVideoAds::getWatchVideoBonus();
        SvrUserProfile::setMoney("VideoBonus", moneyBonus);
        ServerOffline::_getBonusGold = true;
    }
    doc.AddMember(KEY_RESULT, result, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
    doc.AddMember(KEY_ADS_VIEW_MONEY, SvrVideoAds::getNextWatchVideoBonus(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestSaviorBonus()
{
    SvrUserProfile::setMoney("SaviorBonus", USER_DEFAULT_SAVIOR_BONUS);
    ServerOffline::_getBonusGold = true;
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SAVIOR_BONUS_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, RESULT_SUCCESS, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestQuitMatch()
{
    if (_currentGameManager != nullptr)
    {
        _currentGameManager->endGame();
        _currentGameManager = nullptr;
    }
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_QUIT_IN_GAME_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, 1, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onMessage(MessageID messageId, const rapidjson::Document& data)
{
    switch (messageId)
    {
        case MessageID::MESSAGE_PROFILE_REQUEST:
        {
            onRequestUserProfileData(data);
            break;
        }
        case MessageID::MESSAGE_UNLOCK_CITY_REQUEST:
        {
            int pId = JsonHelper::getInt(data, KEY_PLACE_ID, -1);
            onRequestBuyNewCity(pId);
            break;
        }
        case MessageID::MESSAGE_UNLOCK_TABLE_REQUEST:
        {
            int pId = JsonHelper::getInt(data, KEY_TABLE_ID, -1);
            onRequestBuyNewTable(pId);
            break;
        }
        case MessageID::MESSAGE_TICKET_REQUEST:
        {
            int pcId = JsonHelper::getInt(data, KEY_PLACE_ID, -1);
            int ptId = JsonHelper::getInt(data, KEY_TABLE_ID, -1);
            onRequestTicket(pcId, ptId);
            break;
        }
        case MessageID::MESSAGE_JOIN_GAME_REQUEST:
        {
            onRequestJoinCity(data);
            break;
        }
        case MessageID::MESSAGE_USER_DATA_REQUEST:
        {
            onRequestUserData();
            break;
        }
        case MessageID::MESSAGE_CHANGE_USER_NAME_REQUEST:
        {
            std::string name = JsonHelper::getString(data, KEY_NAME, USER_DEFAULT_NAME);
            onRequestChangeName(name);
            break;
        }
        case MessageID::MESSAGE_CHANGE_USER_AVATAR_REQUEST:
        {
            std::string path = JsonHelper::getString(data, KEY_AVATAR, USER_DEFAULT_AVATAR);
            onRequestChangeAvatar(path);
            break;
        }
        case MessageID::MESSAGE_BONUS_REQUEST:
        {
            onRequestBonus();
            break;
        }
        case MessageID::MESSAGE_SAVIOR_BONUS_REQUEST:
        {
            onRequestSaviorBonus();
            break;
        }
        case MessageID::MESSAGE_SAVIOR_POPUP_REQUEST:
        {
            onRequestSaviorPopup();
            break;
        }
        case MessageID::MESSAGE_QUIT_IN_GAME_REQUEST:
        {
            onRequestQuitMatch();
            break;
        }
        case MessageID::MESSAGE_VIDEO_ADS_MONEY_REQUEST:
        {
            onVideoAdsMoney();
            break;
        }
        case MessageID::MESSAGE_LOG_IN_OFFLINE_REQUEST:
        {
            onLogInBonusMoney();
            break;
        }
        case MessageID::MESSAGE_LOAD_SAVED_GAME_REQUEST:
        {
            onLoadSavedMatch();
            break;
        }
        case MessageID::MESSAGE_GET_HOURLY_BONUS_REQUEST:
        {
            onRequestGetHourlyBonus();
            break;
        }
        case MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_REQUEST:
        {
            onRequestCheckShowRatingPopup(JsonHelper::getInt(data, KEY_RATING_OPEN_AT, 0));
            break;
        }
        case MessageID::MESSAGE_RATING_POPUP_SUBMIT_REQUEST:
        {
            onRequestRatingSumbit(JsonHelper::getInt(data, KEY_RATING_STAR, 0));
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_REQUEST:
        {
            onRequestIsVideoRewardFunctionEnable();
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_INFO_REQUEST:
        {
            onRequestVideoRewardInfo();
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_CLAIM_REQUEST:
        {
            bool isSkipWaitingPhase = JsonHelper::getBool(data, KEY_VIDEO_REWARD_IS_SKIP_WAITING_PHASE, false);
            onRequestVideoRewardClaim(isSkipWaitingPhase);
            break;
        }
        case MessageID::MESSAGE_VIDEO_REWARD_SKIP_REQUEST:
        {
            onRequestVideoRewardSkip();
            break;
        }
        case MessageID::MESSAGE_SUGGEST_UPDATE_REQUEST:
        {
            onSuggestUpdate();
            break;
        }
        case MessageID::MESSAGE_MAILBOX_REQUEST:
        {
            onMailbox();
            break;
        }
        case MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_REQUEST:
        {
            std::string type = JsonHelper::getString(data, KEY_MAILBOX_TYPE, "");
            std::string id   = JsonHelper::getString(data, KEY_MAILBOX_ID, "");
            onClaimRewardMailbox(type, id);
            break;
        }
        case MessageID::MESSAGE_LOCAL_NOTIFICATION_REQUEST:
        {
            onGetLocalNotificationData();
            break;
        }
        default:break;
    }
}

void ServerOffline::onRequestSaviorPopup()
{
    if (SvrUserProfile::getMoney() < USER_DEFAULT_MINIMUM_MONEY)
    {
        rapidjson::Document doc;
        doc.SetObject();
        doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SAVIOR_POPUP_RESPONSE, doc.GetAllocator());
        doc.AddMember(KEY_RESULT, 1, doc.GetAllocator());

        int freeGoldResult = true;
        int videoResult    = true;
        int inviteResult   = true;

        rapidjson::Value listNode(rapidjson::kArrayType);

        rapidjson::Value nodeFreeGold;
        nodeFreeGold.SetObject();
        nodeFreeGold.AddMember(KEY_SAVIOR_STATUS, freeGoldResult, doc.GetAllocator());
        nodeFreeGold.AddMember(KEY_FREE_MONEY, USER_DEFAULT_SAVIOR_BONUS, doc.GetAllocator());
        nodeFreeGold.AddMember(KEY_SAVIOR_TYPE, SAVIOR_OPTION_FREE_GOLD, doc.GetAllocator());
        listNode.PushBack(nodeFreeGold, doc.GetAllocator());

        rapidjson::Value nodeVideoAds;
        nodeVideoAds.SetObject();
        nodeVideoAds.AddMember(KEY_SAVIOR_STATUS, videoResult, doc.GetAllocator());
        nodeVideoAds.AddMember(KEY_FREE_MONEY, SvrVideoAds::getNextWatchVideoBonus(), doc.GetAllocator());
        nodeVideoAds.AddMember(KEY_SAVIOR_TYPE, SAVIOR_OPTION_VIDEO_ADS, doc.GetAllocator());
        nodeVideoAds.AddMember(KEY_VIDEO_REWARDS_ENABLE, SvrTimeBonus::isVideoRewardEnabled(), doc.GetAllocator());
        nodeVideoAds.AddMember(KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, SvrTimeBonus::getCurrentVideoRewardAvailableAt(), doc.GetAllocator());
        listNode.PushBack(nodeVideoAds, doc.GetAllocator());

        doc.AddMember(KEY_SAVIOR_LIST, listNode, doc.GetAllocator());
        ServerManager::getInstance()->sendMessage(doc);
    }
}

void ServerOffline::onVideoAdsMoney()
{
    SvrVideoAds::resetWatchVideoBonus();
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_ADS_MONEY_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_ADS_VIEW_MONEY, SvrVideoAds::getNextWatchVideoBonus(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onLogInBonusMoney()
{
    sendHourlyBonusData();
    std::vector<double> result = SvrUserProfile::getLogInBonus();
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOG_IN_OFFLINE_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, (int) result[0], doc.GetAllocator());
    doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
    if (result[0] != LOGIN_BONUS_RESULT_FAIL)
    {
        doc.AddMember(KEY_BONUS_MONEY, result[1], doc.GetAllocator());
        doc.AddMember(KEY_NEXT_BONUS_MONEY, result[2], doc.GetAllocator());
        doc.AddMember(KEY_WELCOME_BACK_DAY, result[3], doc.GetAllocator());
    }
    else
    {
        doc.AddMember(KEY_BONUS_MONEY, 0, doc.GetAllocator());
        doc.AddMember(KEY_NEXT_BONUS_MONEY, 0, doc.GetAllocator());
        doc.AddMember(KEY_WELCOME_BACK_DAY, 0, doc.GetAllocator());
    }
    ServerManager::getInstance()->sendMessage(doc);
    onRequestSaviorPopup();
}

void ServerOffline::onLoadSavedMatch()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOAD_SAVED_MATCH_RESPONSE, doc.GetAllocator());
    if (cocos2d::UserDefault::getInstance()->getBoolForKey(KEY_HAS_SAVED_MATCH, false))
    {
        rapidjson::Document docSave = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_SAVED_MATCH_DATA));
        double              pBet    = JsonHelper::getDouble(docSave, KEY_BET, 0);
        if (pBet > 0)
            SvrUserProfile::setMoney("ForceQuitMatch", -4 * pBet);
        cocos2d::UserDefault::getInstance()->setBoolForKey(KEY_HAS_SAVED_MATCH, false);
    }
    doc.AddMember(KEY_RESULT, RESULT_SUCCESS, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(_serverOfflineInstance);
}

void ServerOffline::sendHourlyBonusData(int result)
{
    double              countdown   = SvrHourlyBonus::getCurrentHourlyBonusCountDown();
    double              hourlyMoney = SvrHourlyBonus::getCurrentHourlyBonus();
    double              money       = SvrUserProfile::getMoney();
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_GET_HOURLY_BONUS_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_MONEY, money, doc.GetAllocator());
    doc.AddMember(KEY_COUNT_DOWN_TIME, countdown, doc.GetAllocator());
    doc.AddMember(KEY_FREE_MONEY, hourlyMoney, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, result, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestGetHourlyBonus()
{
    int result = SvrHourlyBonus::applyClaimHourlyReward();
    sendHourlyBonusData(result);
}

void ServerOffline::onRequestCheckShowRatingPopup(int where)
{
    int         result    = RESULT_FAIL;
    std::string condition = "";
#if USE_CHEAT_SHOW_RATING
    SvrUserProfile::setStateRating(RATING_STATE_WILL_SHOW_FIRST_TIME);
#endif
    int stateRatingPopup = SvrRating::getStateRating();
    if (SvrRating::_listRatingCondition.size() == 0)
        SvrRating::getDataConditionRatingPopup();

    // Check condition
    if (stateRatingPopup != SvrRating::_ratingStateNeverShow)
    {
        condition = SvrRating::checkConditionToShowRatingPopup(where);
        if (condition != "")
        {
            SvrRating::setLastTimeShowRatingPopup(GameUtils::getCurrentTime());
            SvrRating::setCountShowRatingPopup(SvrRating::getCountShowRatingPopup() + 1);
            result = RESULT_SUCCESS;
        }
    }
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_CHECK_SHOW_RATING_POPUP_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, result, doc.GetAllocator());
    doc.AddMember(KEY_RATING_STATE, SvrRating::getCountShowRatingPopup(), doc.GetAllocator());
    doc.AddMember(KEY_RATING_CONDITION, rapidjson::Value(rapidjson::kStringType).SetString(condition.c_str(), doc.GetAllocator()), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestRatingSumbit(int star)
{
    int result = RESULT_SUCCESS;
    if (star >= 1 && star <= 5)
        SvrRating::setStateRating(SvrRating::_ratingStateNeverShow);
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_RATING_POPUP_SUBMIT_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, result, doc.GetAllocator());
    doc.AddMember(KEY_RATING_STAR, star, doc.GetAllocator());
    doc.AddMember(KEY_RATING_STATE, SvrRating::getStateRating(), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}


void ServerOffline::onRequestIsVideoRewardFunctionEnable()
{
    rapidjson::Document doc;
    doc.SetObject();
    bool   isEnable           = SvrTimeBonus::isVideoRewardEnabled();
    double currentVideoReward = (isEnable == true) ? SvrTimeBonus::getCurrentVideoRewardAvailableAt() : -1;
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_FUNCTION_ENABLE_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, isEnable, doc.GetAllocator());
    doc.AddMember(KEY_VIDEO_REWARD_REWARD_AVAILABLE_AT, currentVideoReward, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestVideoRewardInfo()
{
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_INFO_RESPONSE, doc.GetAllocator());
    std::string videoRewardData     = "";
    std::string videoRewardUserData = "";
    SvrTimeBonus::getVideoRewardData(videoRewardData, videoRewardUserData, true);
    if (videoRewardData != "" && videoRewardUserData != "")
    {
        const rapidjson::Document& videoRewardDataDoc     = JsonHelper::parseStringToJson(videoRewardData);
        const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);

        doc.AddMember(KEY_VIDEO_REWARD_REWARD_LV, videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_LEVEL].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_MAX_ROUND, videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_ROUND].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_CURRENT_ROUND, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND].GetInt(), doc.GetAllocator());

        doc.AddMember(KEY_VIDEO_REWARD_ENABLE, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].GetBool(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_CURRENT_STEP, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_COUNT_DOWN, (double) std::max(videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble(), (double) 0), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_REWARD_RESET_AT, (double) SvrTimeBonus::getVideoRewardResetTime(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_SKIPPABLE, (videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt() >= videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_SKIPPABLE_STEP].GetInt()), doc.GetAllocator());
        rapidjson::Value rewardArray = rapidjson::Value(rapidjson::kArrayType);
        for (int         i           = 0; i < videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS].Size(); i++)
            rewardArray.PushBack(videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS][i].GetDouble(), doc.GetAllocator());
        int              lotterySize = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS].Size();
        rewardArray.PushBack(videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS][lotterySize - 1][JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_MONEY].GetDouble(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_REWARD_DATA, rewardArray, doc.GetAllocator());
    }
    else
    {
        doc.AddMember(JSON_VIDEO_REWARDS_USER_DATA_ENABLE, false, doc.GetAllocator());
    }
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestVideoRewardSkip()
{
    std::string videoRewardUserData = SvrTimeBonus::skipVideoRewardStep();
    const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_SKIP_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_VIDEO_REWARD_CURRENT_STEP, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt(), doc.GetAllocator());
    doc.AddMember(KEY_VIDEO_REWARD_COUNT_DOWN, (double) std::max(videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble(), (double) 0), doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onRequestVideoRewardClaim(bool isSkipWaitingPhase)
{
    bool        isLottery           = false;
    double      bonus               = SvrTimeBonus::claimVideoRewardBonus(isLottery);
    std::string videoRewardData     = "";
    std::string videoRewardUserData = "";
    SvrTimeBonus::getVideoRewardData(videoRewardData, videoRewardUserData, false);
    rapidjson::Document doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_VIDEO_REWARD_CLAIM_RESPONSE, doc.GetAllocator());
    if (videoRewardData != "" && videoRewardUserData != "" && bonus > 0)
    {
        SvrUserProfile::setMoney("ClaimVideoReward", bonus);
        doc.AddMember(KEY_VIDEO_REWARD_IS_SKIP_WAITING_PHASE, isSkipWaitingPhase, doc.GetAllocator());
        const rapidjson::Document& videoRewardDataDoc     = JsonHelper::parseStringToJson(videoRewardData);
        const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);
        doc.AddMember(KEY_RESULT, true, doc.GetAllocator());
        doc.AddMember(KEY_BONUS_MONEY, bonus, doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_IS_LOTTERY, isLottery, doc.GetAllocator());
        doc.AddMember(KEY_MONEY, SvrUserProfile::getMoney(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_REWARD_LV, videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_LEVEL].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_MAX_ROUND, videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_ROUND].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_CURRENT_ROUND, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_ENABLE, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].GetBool(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_CURRENT_STEP, videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_COUNT_DOWN, (double) std::max(videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble(), (double) 0), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_REWARD_RESET_AT, (double) SvrTimeBonus::getVideoRewardResetTime(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_SKIPPABLE, (videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt() >= videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_SKIPPABLE_STEP].GetInt()), doc.GetAllocator());
        rapidjson::Value rewardArray = rapidjson::Value(rapidjson::kArrayType);
        for (int         i           = 0; i < videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS].Size(); i++)
            rewardArray.PushBack(videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS][i].GetDouble(), doc.GetAllocator());
        int              lotterySize = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS].Size();
        rewardArray.PushBack(videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS][lotterySize - 1][JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_MONEY].GetDouble(), doc.GetAllocator());
        doc.AddMember(KEY_VIDEO_REWARD_REWARD_DATA, rewardArray, doc.GetAllocator());
    }
    else
    {
        doc.AddMember(KEY_RESULT, false, doc.GetAllocator());
    }
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onSuggestUpdate()
{
    std::string data = RemoteConfig::getString(REMOTECONFIG_FUNCTION_SUGGEST_UPDATE_DATA, "");
    if (data != "")
    {
        const rapidjson::Document& document = JsonHelper::parseStringToJson(data);
        if (!document.IsNull() && document.IsObject())
        {
            long long validDate = JsonHelper::getLong(document, "valid_date", 0);
            if (GameUtils::getCurrentTime() >= validDate)
            {
                int version = JsonHelper::getInt(document, "version", 0);
                const rapidjson::Value& content = JsonHelper::getValue(document, "content");
                rapidjson::Document doc;
                doc.SetObject();
                doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_SUGGEST_UPDATE_RESPONSE, doc.GetAllocator());
                doc.AddMember(KEY_SUGGEST_UPDATE_VERSION, version, doc.GetAllocator());
                if (!content.IsNull())
                    doc.AddMember(KEY_SUGGEST_UPDATE_CONTENT, rapidjson::Value().SetString(JsonHelper::convertValueToString(content).c_str(), doc.GetAllocator()), doc.GetAllocator());
                ServerManager::getInstance()->sendMessage(doc);
            }
        }
    }
}

void ServerOffline::onMailbox()
{
    SvrMailbox::_listMailbox.clear();
    SvrMailbox::_listMailbox = SvrMailbox::getMailboxList();
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Value arrMailbox(rapidjson::kArrayType);
    for (int         i = 0; i < SvrMailbox::_listMailbox.size(); i++)
    {
        const MailboxData& mailItem = SvrMailbox::_listMailbox[i];
        rapidjson::Value mail;
        mail.SetObject();
        mail.AddMember(KEY_MAILBOX_ID, rapidjson::Value().SetString(mailItem._id.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_TYPE, rapidjson::Value().SetString(mailItem._type.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_TITLE, rapidjson::Value().SetString(mailItem._title.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_CONTENT, rapidjson::Value().SetString(mailItem._content.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_FULL_CONTENT, rapidjson::Value().SetString(mailItem._fullContent.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_DATA, rapidjson::Value().SetString(mailItem._data.c_str(), doc.GetAllocator()), doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_START_AT, mailItem._startAt, doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_END_AT, mailItem._endAt, doc.GetAllocator());
        mail.AddMember(KEY_MAILBOX_REWARDS, rapidjson::Value().SetString(mailItem._reward.c_str(), doc.GetAllocator()), doc.GetAllocator());
        arrMailbox.PushBack(mail, doc.GetAllocator());
    }
    doc.AddMember(KEY_MAILBOX_LIST, arrMailbox, doc.GetAllocator());
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_MAILBOX_RESPONSE, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}

void ServerOffline::onClaimRewardMailbox(const std::string& type, const std::string& id)
{
    MailboxData reward = SvrMailbox::getMailbox(type, id);
    if (SvrMailbox::isValidMailboxReward(reward))
    {
        SvrMailbox::removeOldMailboxChecked();
        if (reward._id != "" && reward._type != "")
        {
            const rapidjson::Document& rewardJson = JsonHelper::parseStringToJson(reward._reward);
            if (SvrMailbox::claimMailboxRewards(rewardJson))
            {
                SvrMailbox::setMailboxChecked(reward);
                if (reward._isLocal)
                    SvrMailbox::removeMailLocal(reward);
                SvrMailbox::_listMailbox.clear();
                SvrMailbox::_listMailbox = SvrMailbox::getMailboxList();

                rapidjson::Document doc;
                doc.SetObject();
                doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_MAILBOX_CLAIM_REWARD_RESPONSE, doc.GetAllocator());
                doc.AddMember(KEY_MAILBOX_ID, rapidjson::Value().SetString(reward._id.c_str(), doc.GetAllocator()), doc.GetAllocator());
                doc.AddMember(KEY_MAILBOX_TYPE, rapidjson::Value().SetString(reward._type.c_str(), doc.GetAllocator()), doc.GetAllocator());
                doc.AddMember(KEY_BONUS_MONEY, SvrMailbox::getMailboxRewardValue(reward, "money"), doc.GetAllocator());

                rapidjson::Value arrMailbox(rapidjson::kArrayType);
                for (int         i = 0; i < SvrMailbox::_listMailbox.size(); i++)
                {
                    const MailboxData& mailItem = SvrMailbox::_listMailbox[i];
                    rapidjson::Value mail;
                    mail.SetObject();
                    mail.AddMember(KEY_MAILBOX_ID, rapidjson::Value().SetString(mailItem._id.c_str(), doc.GetAllocator()), doc.GetAllocator());
                    mail.AddMember(KEY_MAILBOX_TYPE, rapidjson::Value().SetString(mailItem._type.c_str(), doc.GetAllocator()), doc.GetAllocator());

                    arrMailbox.PushBack(mail, doc.GetAllocator());
                }
                doc.AddMember(KEY_MAILBOX_LIST, arrMailbox, doc.GetAllocator());
                ServerManager::getInstance()->sendMessage(doc);
            }
        }
    }
}

void ServerOffline::onGetLocalNotificationData()
{
    std::vector<LocalNotification> listNotification = SvrLocalNotification::getLocalNotificationData();
    rapidjson::Document            doc;
    doc.SetObject();
    doc.AddMember(KEY_MESSAGE_ID, (int) MessageID::MESSAGE_LOCAL_NOTIFICATION_RESPONSE, doc.GetAllocator());
    doc.AddMember(KEY_RESULT, RESULT_SUCCESS, doc.GetAllocator());

    rapidjson::Value arrNotification(rapidjson::kArrayType);
    for (int         i = 0; i < listNotification.size(); i++)
    {
        const LocalNotification& notification = listNotification[i];
        rapidjson::Value notificationJson;
        notificationJson.SetObject();
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_ID, notification.id, doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_NAME, rapidjson::Value().SetString(notification.name.c_str(), doc.GetAllocator()), doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_TYPE, rapidjson::Value().SetString(notification.type.c_str(), doc.GetAllocator()), doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_TITLE, rapidjson::Value().SetString(notification.title.c_str(), doc.GetAllocator()), doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_MESSAGE, rapidjson::Value().SetString(notification.message.c_str(), doc.GetAllocator()), doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_IMAGE, rapidjson::Value().SetString(notification.image.c_str(), doc.GetAllocator()), doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_DELAY_TIME, (double) notification.delayTime, doc.GetAllocator());
        notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_PERIOD_TIME, (double) notification.periodTime, doc.GetAllocator());
        arrNotification.PushBack(notificationJson, doc.GetAllocator());
    }
    doc.AddMember(KEY_LOCAL_NOTIFICATION_NOTIFICATIONS, arrNotification, doc.GetAllocator());
    ServerManager::getInstance()->sendMessage(doc);
}
