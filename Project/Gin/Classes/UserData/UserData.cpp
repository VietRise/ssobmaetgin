#include "UserData.h"
#include "Define/GameDefine.h"
#include "Manager/NotificationManager.h"
#include "Manager/TextManager.h"
#include "Helper/GameUtils.h"
#include "ServerOffline/SvrUserProfile.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Helper/JsonHelper.h"
#include "Define/ResourcesDefine.h"
#include "Native/CallToOtherLanguage.h"
#include "Define/InGameDefine.h"
#include "Define/ServerClientMessageDefine.h"

#define MAX_DAILY_NEWS_SHOW_PER_DAY 1
#define MAX_DAILY_NEWS_SHOW_DAY 3
#define MIN_MATCH_COUNT_TO_SHOW_NEWS 50
#define DEFAULT_NEWS_ENABLE false

#define DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_DAY "max_show_day"
#define DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_PER_DAY "max_show_per_day"
#define DAILY_NEWS_JSON_KEY_MIN_MATCH_COUNT "min_match_count"
#define DAILY_NEWS_JSON_KEY_VERSION_CODE "version_code"
#define EVENT_DECOR_JSON_KEY_START_TIME "start_time"
#define EVENT_DECOR_JSON_KEY_END_TIME "end_time"
#define KEY_USER_LAST_LOGIN "last_login"

USING_NS_CC;
using namespace std;

static UserData* _instance = nullptr;

UserData::UserData()
{
    this->_deviceInfo           = new DeviceInfo();
    this->_luckyValue           = 0;
    this->_level                = 0;
    this->_levelexp             = 0;
    this->_nextlevelexp         = 0;
    this->_enableInterferingAds = true;

    if (!UserDefault::getInstance()->getBoolForKey(USERDEFAULT_FILEEXIST))
    {
        UserDefault::getInstance()->setBoolForKey(USERDEFAULT_FILEEXIST, true);
        this->setRestoreSoundBG(false);
        this->setSoundEffect(true);
        this->setSoundBackground(true);
        this->setVibration(true);
        this->setIdPlaceSelected(1);
        this->setIdTableSelected(1);
        this->setBet(0);
    }
    else
    {
        this->setRestoreSoundBG(UserDefault::getInstance()->getBoolForKey(USERDEFAULT_RESTORE_SOUND_BG));
        this->setSoundEffect(UserDefault::getInstance()->getBoolForKey(USERDEFAULT_SOUNDEFFECT, true));
        this->setSoundBackground(UserDefault::getInstance()->getBoolForKey(USERDEFAULT_SOUNDBACKGROUND, true));
        this->setVibration(UserDefault::getInstance()->getBoolForKey(USERDEFAULT_VIBRATON));
        this->setIdPlaceSelected(UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_IDPLACESELECTED, 0));
        this->setIdTableSelected(UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_IDTABLESELECTED, 1));
        this->setBet(UserDefault::getInstance()->getDoubleForKey(USERDEFAULT_BET, 0));
    }

    s_winNumber            = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_WIN_NUMBER, 0);
    s_loseNumber           = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_LOSE_NUMBER, 0);
    s_winHitpotNumber      = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_WIN_HITPOT_NUMBER, 0);
    s_totalWatchVideoCount = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_WV_NUMBER, 0);
    _gameCenterVersion     = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_GAME_CENTER_VERSION, 0);
}

UserData::~UserData()
{
    CC_SAFE_DELETE(this->_deviceInfo);
}

UserData* UserData::getInstance()
{
    if (_instance == nullptr)
        _instance = new UserData();
    return _instance;
}

void UserData::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

DeviceInfo* UserData::getDeviceInfo()
{
    return this->_deviceInfo;
}

void UserData::setMoney(double money)
{
    this->_userMoney = money;
    cocos2d::Value moneyVl((double) money);
    NotificationManager::getInstance()->postNotification(NOTIFY_USERDATA_CHANGE_MONEY, moneyVl);
}

double UserData::getMoney()
{
    return this->_userMoney;
}

void UserData::setUserName(const std::string& userName)
{
    this->_userName = userName;
}

string UserData::getUserName()
{
    return this->_userName;
}

void UserData::setLevel(int level)
{
    this->_level = level;
}

int UserData::getLevel()
{
    return this->_level;
}

void UserData::setLevelExp(int levelexp)
{
    this->_levelexp = levelexp;
}

int UserData::getLevelExp()
{
    return this->_levelexp;
}

void UserData::setNextLevelExp(int nextlevelexp)
{
    _nextlevelexp = nextlevelexp;
}

int UserData::getNextLevelExp()
{
    return _nextlevelexp;
}

void UserData::setAvatarLink(const std::string& avatarLink)
{
    this->_avatarLink = avatarLink;
}

string UserData::getAvatarLink()
{
    return this->_avatarLink;
}

void UserData::setLuckyValue(float luckyVal)
{
    _luckyValue = luckyVal;
}

float UserData::getLuckyValue()
{
    return _luckyValue;
}

void UserData::obtainDeviceInfo()
{
    this->_deviceInfo->obtainDeviceInfo();
}

void UserData::setRestoreSoundBG(bool shouldRestore)
{
    _restoreSoundBackground = shouldRestore;
    UserDefault::getInstance()->setBoolForKey(USERDEFAULT_RESTORE_SOUND_BG, _restoreSoundBackground);
}

bool UserData::getRestoreSoundBG()
{
    return _restoreSoundBackground;
}

void UserData::setSoundEffect(bool sound)
{
    _soundEffect = sound;
    UserDefault::getInstance()->setBoolForKey(USERDEFAULT_SOUNDEFFECT, _soundEffect);
}

bool UserData::getSoundEffect()
{
    return _soundEffect;
}

void UserData::setSoundBackground(bool sound)
{
    _soundBackground = sound;
    UserDefault::getInstance()->setBoolForKey(USERDEFAULT_SOUNDBACKGROUND, _soundBackground);
}

bool UserData::getSoundBackground()
{
    return _soundBackground;
}

void UserData::setBet(double bet)
{
    _bet = bet;
    UserDefault::getInstance()->setDoubleForKey(USERDEFAULT_BET, _bet);
}

double UserData::getBet()
{
    return _bet;
}

void UserData::setTargetScore(int targetScore)
{
    _targetScore = targetScore;
    UserDefault::getInstance()->setDoubleForKey(USERDEFAULT_TARGET_SCORE, _targetScore);
}

double UserData::getTargetScore()
{
    return _targetScore;
}

void UserData::setVibration(bool vibration)
{
    _vibration = vibration;
    UserDefault::getInstance()->setBoolForKey(USERDEFAULT_VIBRATON, _vibration);
}

bool UserData::getVibration()
{
    return _vibration;
}

void UserData::setIdPlaceSelected(int idPlaceSelected)
{
    _idPlaceSelected = idPlaceSelected;
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_IDPLACESELECTED, _idPlaceSelected);
}

int UserData::getIdPlaceSelected()
{
    return _idPlaceSelected;
}

void UserData::setIdTableSelected(int idTableSelected)
{
    _idTableSelected = idTableSelected;
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_IDTABLESELECTED, _idTableSelected);
}

int UserData::getIdTableSelected()
{
    return _idTableSelected;
}

void UserData::saveWinOrLoseNumber()
{
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_WIN_NUMBER, s_winNumber);
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_LOSE_NUMBER, s_loseNumber);
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_WIN_HITPOT_NUMBER, s_winHitpotNumber);
}

void UserData::saveWatchVideoNumber()
{
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_WV_NUMBER, s_totalWatchVideoCount);
}

bool UserData::shouldShowDailyNews(int versionCode)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return true;
#endif
    std::string dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_NEWS_POPUP_DATA, Resources::JSON::JSON_FILE_NEWS);
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(dataString);

    int maxDailyNewsShowDay    = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_DAY, MAX_DAILY_NEWS_SHOW_DAY);
    int maxDailyNewsShowPerDay = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_PER_DAY, MAX_DAILY_NEWS_SHOW_PER_DAY);
    int minMathCountToShowNews = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MIN_MATCH_COUNT, MIN_MATCH_COUNT_TO_SHOW_NEWS);
    int versionCodeData        = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_VERSION_CODE, 0);

    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_NEWS_POPUP_ENABLE, DEFAULT_NEWS_ENABLE) == false || versionCode < versionCodeData)
        return false;

    if (!CallToOtherLanguage::getInstance()->checkInternet())
        return false;

    if (SvrUserProfile::getTotalMatchCount() < minMathCountToShowNews)
        return false;

    int dayCount = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT, 0);
    int count    = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_COUNT, 0);
    if (dayCount >= maxDailyNewsShowDay && count >= maxDailyNewsShowPerDay)
        return false;

    double currentTimeDouble = GameUtils::getCurrentTime();
    double lastLogInDouble   = GameUtils::getDoubleForKeyUserDefault(KEY_USER_LAST_LOGIN);

    double dateDiff = GameUtils::dayDiff(currentTimeDouble, lastLogInDouble);
    if (dateDiff > 0)
    {
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_COUNT, 1);
        return true;
    }
    else if (count < maxDailyNewsShowPerDay)
    {
        count++;
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_COUNT, count);
        return true;
    }
    return false;
}

void UserData::resetNewsShowCounter()
{
    std::string dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_NEWS_POPUP_DATA, Resources::JSON::JSON_FILE_NEWS);
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(dataString);
    int maxDailyNewsShowDay = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_DAY, MAX_DAILY_NEWS_SHOW_DAY);
    int dayCount            = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT, 0);
    if (dayCount < maxDailyNewsShowDay)
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_COUNT, 0);
}

void UserData::updateNewsShowCounter(int versionCode)
{
    std::string dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_NEWS_POPUP_DATA, Resources::JSON::JSON_FILE_NEWS);
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(dataString);
    int maxDailyNewsShowDay    = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MAX_DAILY_NEWS_SHOW_DAY, MAX_DAILY_NEWS_SHOW_DAY);
    int minMathCountToShowNews = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_MIN_MATCH_COUNT, MIN_MATCH_COUNT_TO_SHOW_NEWS);
    int versionCodeData        = JsonHelper::getInt(doc, DAILY_NEWS_JSON_KEY_VERSION_CODE, 0);

    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_NEWS_POPUP_ENABLE, DEFAULT_NEWS_ENABLE) == false || versionCode < versionCodeData)
        return;

    int dayCount = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT, 0);
    if (SvrUserProfile::getTotalMatchCount() >= minMathCountToShowNews && dayCount < maxDailyNewsShowDay)
    {
        dayCount++;
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT, dayCount);
    }
}

void UserData::updateNewsFunctionVersion()
{
    int localVersion  = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_DAILY_NEWS_VERSION, 0);
    int remoteVersion = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_NEWS_POPUP_VERSION, 0);
    if (localVersion < remoteVersion)
    {
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_COUNT, 0);
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_SHOW_DAY_COUNT, 0);
        UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_DAILY_NEWS_VERSION, remoteVersion);
    }
}

bool UserData::isEnableShowRatingPopup()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#else
    if(!CallToOtherLanguage::getInstance()->checkInternet())
        return false;
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_RATING_POPUP_ENABLE, false);
#endif
}

bool UserData::isEnableGameCenterPopup()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#else
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_GAME_CENTER_POPUP_ENABLE, true);
#endif
}

bool UserData::isEnableSuggestUpdate()
{
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_SUGGEST_UPDATE_ENABLE, false);
}

bool UserData::isEnableLocalNotification()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#else
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_LOCAL_NOTIFICATION_ENABLE, false);
#endif
}

bool UserData::isEnableButtonRating()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    return true;
#else
    return RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_RATING_IN_SETTING_ENABLE, false);
#endif
}

int UserData::getGameCenterVersion()
{
    return _gameCenterVersion;
}

void UserData::setGameCenterVersion(int version)
{
    _gameCenterVersion = version;
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_GAME_CENTER_VERSION, version);
}

int UserData::getSuggestUpdateVersionCode()
{
    return UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_SUGGEST_UPDATE_VERSION_CODE, -1);
}

void UserData::setSuggestUpdateVersionCode(int version)
{
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_SUGGEST_UPDATE_VERSION_CODE, version);
}

void UserData::setMailRead(const std::vector<std::string>& ids)
{
    std::vector<std::string> list = getMailRead();
    for (int                 i    = 0; i < ids.size(); i++)
    {
        bool     add = true;
        for (int j   = 0; j < list.size(); j++)
        {
            if (ids[i] == list[j])
            {
                add = false;
                break;
            }
        }
        if (add)
            list.push_back(ids[i]);
    }

    int limit = 100;
    if (list.size() > limit)
    {
        for (auto iter = list.begin(); iter != list.end();)
        {
            if (list.size() > limit - 10)
                iter = list.erase(iter);
            else
                break;
        }
    }

    rapidjson::Document doc;
    doc.SetArray();
    _mailReadlist.clear();
    for (int i = 0; i < list.size(); i++)
    {
        _mailReadlist.push_back(list[i]);
        doc.PushBack(rapidjson::Value(rapidjson::kStringType).SetString(list[i].c_str(), doc.GetAllocator()), doc.GetAllocator());
    }

    GameUtils::writeStringToFileEncode(MAILBOX_READ_FILENAME, JsonHelper::convertDocumentToString(doc));
}

std::vector<std::string> UserData::getMailRead()
{
    if (_mailReadlist.size() == 0)
    {
        std::string         json = GameUtils::readStringFromFileDecode(MAILBOX_READ_FILENAME, "[]");
        rapidjson::Document doc  = JsonHelper::parseStringToJson(json);
        if (doc.IsArray())
            for (int        i    = 0; i < doc.Size(); i++)
                if (doc[i].IsString())
                    _mailReadlist.push_back(doc[i].GetString());
    }
    return _mailReadlist;
}

bool UserData::isEventDecorEnabled()
{
    bool isEnable = RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_EVENT_DECOR_ENABLE, false);
    if (isEnable == false)
        return false;

    std::string dataJson = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_EVENT_DECOR_JSON, Resources::JSON::JSON_FILE_EVENT_DECOR_DATA);
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(dataJson);
    double startTime = JsonHelper::getDouble(doc, EVENT_DECOR_JSON_KEY_START_TIME, 0);
    double endTime   = JsonHelper::getDouble(doc, EVENT_DECOR_JSON_KEY_END_TIME, 0);
    if (startTime > endTime)
        return false;
    double currentTime = GameUtils::getCurrentTime();
    if (currentTime < startTime || currentTime > endTime)
        return false;
    return true;
}

void UserData::setInterferingAdsEnable(bool enable)
{
    _enableInterferingAds = enable;
}

bool UserData::isEnableInterferingAds()
{
    return _enableInterferingAds;
}

void UserData::parseLocalNotificationData(const rapidjson::Value& listNotificationData)
{
    s_localNotficationData = "[]";
    if (listNotificationData.IsArray() && listNotificationData.Size() > 0)
    {
        rapidjson::Document dataJson;
        rapidjson::Value& listNotificationJson = dataJson.SetArray();
        for (int i = 0; i < listNotificationData.Size(); i++)
        {
            const rapidjson::Value& notificationData = listNotificationData[i];
            rapidjson::Value notificationJson;
            notificationJson.SetObject();
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_ID, JsonHelper::getInt(notificationData, KEY_LOCAL_NOTIFICATION_ID, -1), dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_NAME,
                                       rapidjson::Value().SetString(JsonHelper::getString(notificationData, KEY_LOCAL_NOTIFICATION_NAME, "").c_str(), dataJson.GetAllocator()),
                                       dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_TYPE,
                                       rapidjson::Value().SetString(JsonHelper::getString(notificationData, KEY_LOCAL_NOTIFICATION_TYPE, "").c_str(), dataJson.GetAllocator()),
                                       dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_TITLE,
                                       rapidjson::Value().SetString(GET_TEXT_BY_JSON(JsonHelper::parseStringToJson(JsonHelper::getString(notificationData, KEY_LOCAL_NOTIFICATION_TITLE, "").c_str())),
                                                                    dataJson.GetAllocator()),
                                       dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_MESSAGE,
                                       rapidjson::Value().SetString(GET_TEXT_BY_JSON(JsonHelper::parseStringToJson(JsonHelper::getString(notificationData,
                                                                                                                                         KEY_LOCAL_NOTIFICATION_MESSAGE,
                                                                                                                                         "").c_str())), dataJson.GetAllocator()),
                                       dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_IMAGE,
                                       rapidjson::Value().SetString(JsonHelper::getString(notificationData, KEY_LOCAL_NOTIFICATION_IMAGE, "").c_str(), dataJson.GetAllocator()),
                                       dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_DELAY_TIME, JsonHelper::getDouble(notificationData, KEY_LOCAL_NOTIFICATION_DELAY_TIME, 0), dataJson.GetAllocator());
            notificationJson.AddMember(KEY_LOCAL_NOTIFICATION_PERIOD_TIME, JsonHelper::getDouble(notificationData, KEY_LOCAL_NOTIFICATION_PERIOD_TIME, 0), dataJson.GetAllocator());
            listNotificationJson.PushBack(notificationJson, dataJson.GetAllocator());
        }
        s_localNotficationData = JsonHelper::convertDocumentToString(dataJson);
    }
}
