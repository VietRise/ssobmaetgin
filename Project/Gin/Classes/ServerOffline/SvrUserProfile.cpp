#include "SvrUserProfile.h"
#include "Helper/JsonHelper.h"
#include "ServerOfflineDefines.h"
#include "InGameServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "ServerOffline.h"
#include "Define/ResourcesDefine.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Bot/BotConstant.h"
#include "Feature/SvrWelcomeBackBonus.h"
#include "Feature/SvrLevel.h"
#include "Feature/SvrDailyBonus.h"
#include "Feature/SvrLog.h"
#include "Feature/SvrShop.h"
#include "Define/GameDefine.h"

USING_NS_CC;
using namespace std;

double SvrUserProfile::getMoney()
{
    return GameUtils::getDoubleForKeyUserDefault(KEY_DATA_MONEY);
}

void SvrUserProfile::setMoney(const string& name, double bonusAmount)
{
    double current  = (double) getMoney();
    double current2 = current;
    current += bonusAmount;
    current         = current > USER_DEFAULT_MAXIMUM_MONEY ? USER_DEFAULT_MAXIMUM_MONEY : (current < 0 ? 0 : current);
    GameUtils::setDoubleForKeyUserDefault(KEY_DATA_MONEY, current);
    double diffAmount = current - current2;
    if (diffAmount > 0)
        SvrLog::addEarnMoney(name, diffAmount);
    else if (diffAmount < 0)
        SvrLog::addSpendMoney(name, diffAmount);
}

void SvrUserProfile::initUserMoney()
{
    GameUtils::setDoubleForKeyUserDefault(KEY_DATA_MONEY, USER_DEFAULT_MONEY);
}

string SvrUserProfile::getName()
{
    return UserDefault::getInstance()->getStringForKey(KEY_DATA_NAME, USER_DEFAULT_NAME);
}

string SvrUserProfile::getAvatar()
{
    return UserDefault::getInstance()->getStringForKey(KEY_DATA_AVATAR, USER_DEFAULT_AVATAR);
}

void SvrUserProfile::initUserName()
{
    UserDefault::getInstance()->getStringForKey(KEY_DATA_NAME, USER_DEFAULT_NAME);
}

void SvrUserProfile::initUserAvatar()
{
    UserDefault::getInstance()->getStringForKey(KEY_DATA_AVATAR, USER_DEFAULT_AVATAR);
}

void SvrUserProfile::setName(const string& name)
{
    UserDefault::getInstance()->setStringForKey(KEY_DATA_NAME, name);
}

void SvrUserProfile::setAvatar(const string& path)
{
    UserDefault::getInstance()->setStringForKey(KEY_DATA_AVATAR, path);
}

void SvrUserProfile::updateRawData()
{
    SvrShop::updateRawCityData();
    SvrShop::updateRawTableData();
}

void SvrUserProfile::updateLastLogIn(double newLastLogIn)
{
    GameUtils::setDoubleForKeyUserDefault(KEY_USER_LAST_LOGIN, newLastLogIn);
}

vector<double> SvrUserProfile::getLogInBonus()
{
    vector<double> result;
    double              currentTimeDouble = GameUtils::getCurrentTime();
    double              lastLogInDouble   = GameUtils::getDoubleForKeyUserDefault(KEY_USER_LAST_LOGIN);
    double              dateDiff          = GameUtils::dayDiff(currentTimeDouble, lastLogInDouble);
    if (dateDiff > DAILY_LOGIN_DAY_DIFF)
        SvrDailyBonus::resetLogInBonusCounter();
    auto welcomeBackBonus = SvrWelcomeBackBonus::getWelcomeBackBonus(SvrLevel::getLevel(), dateDiff);
    if (lastLogInDouble != 0 && welcomeBackBonus.second > 0)
    {
        double moneyBonus = welcomeBackBonus.second;
        result.push_back(LOGIN_BONUS_RESULT_WELCOMEBACK);
        result.push_back(moneyBonus);
        result.push_back(0);
        result.push_back(welcomeBackBonus.first);
        setMoney("WelcomeBackBonus", moneyBonus);
    }
    else if (dateDiff >= DAILY_LOGIN_DAY_DIFF && lastLogInDouble != 0 /*|| lastLogInDouble == 0*/)
    {
        double bonusMoney = SvrDailyBonus::getLogInBonusMoney();
        SvrDailyBonus::toNextLogInBonusCounter();
        double nextBonusMoney = SvrDailyBonus::getLogInBonusMoney();
        result.push_back(LOGIN_BONUS_RESULT_DAILY);
        result.push_back(bonusMoney);
        result.push_back(nextBonusMoney);
        result.push_back(0);
        setMoney("DailyBonus", bonusMoney);
    }
    else if (isStartupLogin())
    {
        double money = USER_DEFAULT_WELLCOME_STARTUP;
        result.push_back(LOGIN_BONUS_RESULT_WELCOME_STARTUP);
        result.push_back(money);
        result.push_back(0);
        result.push_back(0);
        setMoney("StartupBonus", money);
    }
    else
    {
        result.push_back(LOGIN_BONUS_RESULT_FAIL);
    }
    updateLastLogIn(currentTimeDouble);
    return result;
}

void SvrUserProfile::addMatchCount(int city, int count)
{
    addMatchCount((CityType) city, count);
}

void SvrUserProfile::addMatchCount(const CityType& city, int count)
{
    switch (city)
    {
        case CityType::CLASSIC_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_MATCH_COUNT_CLASSIC_GIN, getMatchCount(city) + count);
            break;
        case CityType::STRAIGHT_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_MATCH_COUNT_STRAIGHT_GIN, getMatchCount(city) + count);
            break;
        case CityType::OKLAHOMA_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_MATCH_COUNT_OKLAHOMA_GIN, getMatchCount(city) + count);
            break;
    }
}

int SvrUserProfile::getMatchCount(int city)
{
    return getMatchCount((CityType) city);
}

int SvrUserProfile::getMatchCount(const CityType& city)
{
    switch (city)
    {
        case CityType::CLASSIC_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_MATCH_COUNT_CLASSIC_GIN);
        case CityType::STRAIGHT_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_MATCH_COUNT_STRAIGHT_GIN);
        case CityType::OKLAHOMA_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_MATCH_COUNT_OKLAHOMA_GIN);
        default:return 0;
    }
}

int SvrUserProfile::getTotalMatchCount()
{
    return getMatchCount(CityType::CLASSIC_GIN) + getMatchCount(CityType::STRAIGHT_GIN) + getMatchCount(CityType::OKLAHOMA_GIN);
}

void SvrUserProfile::addGameCount(int city, int count)
{
    addGameCount((CityType) city, count);
}

void SvrUserProfile::addGameCount(const CityType& city, int count)
{
    switch (city)
    {
        case CityType::CLASSIC_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_GAME_COUNT_CLASSIC_GIN, getGameCount(city) + count);
            break;
        case CityType::STRAIGHT_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_GAME_COUNT_STRAIGHT_GIN, getGameCount(city) + count);
            break;
        case CityType::OKLAHOMA_GIN:GameUtils::setDoubleForKeyUserDefault(KEY_GAME_COUNT_OKLAHOMA_GIN, getGameCount(city) + count);
            break;
    }
}

int SvrUserProfile::getGameCount(int city)
{
    return getGameCount((CityType) city);
}

int SvrUserProfile::getGameCount(const CityType& city)
{
    switch (city)
    {
        case CityType::CLASSIC_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_GAME_COUNT_CLASSIC_GIN);
        case CityType::STRAIGHT_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_GAME_COUNT_STRAIGHT_GIN);
        case CityType::OKLAHOMA_GIN:return GameUtils::getDoubleForKeyUserDefault(KEY_GAME_COUNT_OKLAHOMA_GIN);
        default:return 0;
    }
}

int SvrUserProfile::getTotalGameCount()
{
    return getGameCount(CityType::CLASSIC_GIN) + getGameCount(CityType::STRAIGHT_GIN) + getGameCount(CityType::OKLAHOMA_GIN);
}

void SvrUserProfile::setPlayingMatchData(const string& data)
{
    GameUtils::setStringForKeyUserDefault(KEY_SAVED_MATCH_DATA, data);
}

bool SvrUserProfile::isNewUser()
{
    return (SvrUserProfile::getTotalMatchCount() <= MATCH_COUNT_NEW_USER);
}

bool SvrUserProfile::isStartupLogin()
{
    double lastLogInDouble = GameUtils::getDoubleForKeyUserDefault(KEY_USER_LAST_LOGIN, -1);
    if (lastLogInDouble == -1)
        return true;
    return false;
}

void SvrUserProfile::setCountWinMatchUser(int count)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_WIN_MATCH_COUNT, count);
}

int SvrUserProfile::getCountWinMatchUser()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_WIN_MATCH_COUNT, 0);
}

void SvrUserProfile::setCountLoseMatchUser(int count)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_LOSE_MATCH_COUNT, count);
}

int SvrUserProfile::getCountLoseMatchUser()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_LOSE_MATCH_COUNT, 0);
}

void SvrUserProfile::setCountWinGameUser(int count)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_WIN_GAME_COUNT, count);
}

int SvrUserProfile::getCountWinGameUser()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_WIN_GAME_COUNT, 0);
}

void SvrUserProfile::setCountLoseGameUser(int count)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_USER_LOSE_GAME_COUNT, count);
}

int SvrUserProfile::getCountLoseGameUser()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_USER_LOSE_GAME_COUNT, 0);
}

void SvrUserProfile::setVersionCode(int version)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_VERSION_CODE, version);
}

int SvrUserProfile::getVersionCode()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_VERSION_CODE, 0);
}

void SvrUserProfile::setInterferingAdsEnable(bool enable)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_INTERFERING_ADS, enable ? 1 : 0);
}

bool SvrUserProfile::getInterferingAdsEnable()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_INTERFERING_ADS, 1) == 1;
}

void SvrUserProfile::getBetData()
{
    string json = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_BET_DATA, Resources::JSON::JSON_FILE_BET_DATA);
    s_betRange.clear();
    rapidjson::Document betJson = JsonHelper::parseStringToJson(json);
    if (betJson.IsArray())
    {
        for (int i = 0; i < betJson.Size(); i++)
        {
            const rapidjson::Value& ruleBet = betJson[i];
            const rapidjson::Value& cities  = JsonHelper::getValue(ruleBet, "cities", rapidjson::Value(rapidjson::kNullType));
            map<int, map<double, vector<double>>> mapCities;
            if (cities.IsArray())
            {
                for (int j = 0; j < cities.Size(); j++)
                {
                    map<double, vector<double>> map;
                    const rapidjson::Value& city                   = cities[j];
                    const rapidjson::Value& cityBets               = JsonHelper::getValue(city, "bet", rapidjson::Value(rapidjson::kNullType));

                    if (cityBets.IsArray())
                    {
                        for (int k = 1; k < cityBets.Size(); k++)
                        {
                            const rapidjson::Value& bet  = cityBets[k];
                            const rapidjson::Value& bets = bet[1];
                            vector<double> listBet;
                            for (int            g = 0; g < bets.Size(); g++)
                                listBet.push_back(bets[g].GetDouble());
                            map[bet[0].GetDouble()] = listBet;
                        }
                    }
                    mapCities[JsonHelper::getInt(city, "city", 0)] = map;
                }
            }
            s_betRange[JsonHelper::getInt(ruleBet, "rule", 0)] = mapCities;
        }
    }
}

void SvrUserProfile::getTargetScoreData()
{
    string json = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_TARGET_SCORE_DATA, Resources::JSON::JSON_FILE_TARGET_SCORE_DATA);
    s_targetScoreDataRange.clear();
    rapidjson::Document targetScoreJson = JsonHelper::parseStringToJson(json);
    if (targetScoreJson.IsArray())
    {
        for (int i = 0; i < targetScoreJson.Size(); i++)
        {
            const rapidjson::Value& ruleTargetScore = targetScoreJson[i];
            const rapidjson::Value& cities          = JsonHelper::getValue(ruleTargetScore, "cities", rapidjson::Value(rapidjson::kNullType));
            map<int, vector<TargetScoreData>> mapCities;
            if (cities.IsArray())
            {
                for (int j = 0; j < cities.Size(); j++)
                {
                    const rapidjson::Value& city             = cities[j];
                    const rapidjson::Value& cityTargetScores = JsonHelper::getValue(city, "target_scores", rapidjson::Value(rapidjson::kNullType));

                    vector<TargetScoreData> targetScores;
                    if (cityTargetScores.IsArray())
                    {
                        for (int k = 0; k < cityTargetScores.Size(); k++)
                        {
                            const rapidjson::Value& jsonData = cityTargetScores[k];
                            TargetScoreData data;
                            data._targetScore        = JsonHelper::getInt(jsonData, "target_score", 0);
                            data._jackpotBetMultiply = JsonHelper::getFloat(jsonData, "jackpot_bet_multiply", 0);

                            const rapidjson::Value& mtpJsonData = JsonHelper::getValue(jsonData, "multiply_data", rapidjson::Value(rapidjson::kNullType));
                            if (mtpJsonData.IsArray())
                            {
                                for (int l = 0; l < mtpJsonData.Size(); l++)
                                {
                                    int   scoreGap = JsonHelper::getInt(mtpJsonData[l], "score_gap", 0);
                                    float value    = JsonHelper::getFloat(mtpJsonData[l], "multiply", 0);

                                    data._multiplyDataMap[scoreGap] = value;
                                }
                            }
                            targetScores.push_back(data);
                        }
                        if (targetScores.size() > 0)
                        {
                            mapCities[JsonHelper::getInt(city, "city", 0)] = targetScores;
                        };
                    }
                }
            }
            s_targetScoreDataRange[JsonHelper::getInt(ruleTargetScore, "rule", 0)] = mapCities;
        }
    }
}

void SvrUserProfile::initMoneyRangeName()
{
    string json = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_MONEY_RANGE_NAME, Resources::JSON::JSON_FILE_MONEY_RANGE_DATA);
    s_moneyRangeName.clear();
    rapidjson::Document data = JsonHelper::parseStringToJson(json);
    if (data.IsArray())
    {
        for (int i = 0; i < data.Size(); i++)
        {
            MoneyRangeDefine moneyDefine;
            const rapidjson::Value& range = data[i];
            moneyDefine._name = JsonHelper::getString(range, "name", "");
            moneyDefine._min  = JsonHelper::getDouble(range, "min", -1);
            moneyDefine._max  = JsonHelper::getDouble(range, "max", -1);
            s_moneyRangeName.push_back(moneyDefine);
        }
    }
}

int SvrUserProfile::getBotVersion()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_BOT_VERSION, (int) BotConstant::Version::VERSION_0);
}

void SvrUserProfile::setBotVersion(int version)
{
    GameUtils::setIntegerForKeyUserDefault(KEY_BOT_VERSION, version);
}