#include "SvrTimeBonus.h"
#include "Define/GameDefine.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Define/ResourcesDefine.h"
#include "ServerOffline/Feature/SvrLevel.h"
#include "cocos2d.h"

void SvrTimeBonus::initVideoRewardDatas()
{
    int         userlevel  = SvrLevel::getLevel();
    std::string masterJson = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_TIME_BONUS_MASTER_JSON, Resources::JSON::JSON_VIDEO_REWARD_MASTER_DATA);
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(masterJson);
    if (doc.Size() > 0)
    {
        int      index = 0;
        for (int i     = 0; i < doc.Size(); i++)
        {
            int lv = JsonHelper::getInt(doc[i], "lv", 0);
            if (lv > userlevel)
                break;
            index = i;
        }
        const rapidjson::Value& value = doc[index];
        std::string data = JsonHelper::convertValueToString(value);
        GameUtils::setStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA, data);
    }
    else
    {
        GameUtils::setStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA, cocos2d::FileUtils::getInstance()->getStringFromFile(Resources::JSON::JSON_VIDEO_REWARD_DATA));
    }

    std::string videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    double      lastRewardClaimedAt = GameUtils::getCurrentTime();

    GameUtils::setStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA, cocos2d::FileUtils::getInstance()->getStringFromFile(Resources::JSON::JSON_VIDEO_REWARD_USER_DATA));
    videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    rapidjson::Document videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);
    videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT].SetDouble(lastRewardClaimedAt);
    updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
}

void SvrTimeBonus::resetVideoRewardDatas()
{
    initVideoRewardDatas();
}

void SvrTimeBonus::getVideoRewardData(std::string& videoRewardData, std::string& videoRewardUserData, bool shouldReset)
{
    checkAndResetVideoRewardDataOnNewDay(shouldReset);
    videoRewardData     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
}

double SvrTimeBonus::claimVideoRewardBonus(bool& isLottery)
{
    isLottery = false;
    std::string videoRewardDataJson     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserDataJson = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    if (videoRewardDataJson.empty() == true || videoRewardUserDataJson.empty() == true)
        initVideoRewardDatas();
    const rapidjson::Document& videoRewardDataDoc     = JsonHelper::parseStringToJson(videoRewardDataJson);
    const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserDataJson);

    int    currentStep = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt();
    int    maxStep     = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_STEP].GetInt();
    double bonus       = 0;
    if (currentStep < maxStep)
    {
        isLottery = false;
        bonus     = claimVideoRewardNormalBonus();
    }
    else if (currentStep == maxStep)
    {
        isLottery = true;
        bonus     = claimVideoRewardExtraBonus();
    }
    return bonus;
}

double SvrTimeBonus::claimVideoRewardNormalBonus()
{
    std::string videoRewardDataJson     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserDataJson = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    if (videoRewardDataJson.empty() == true || videoRewardUserDataJson.empty() == true)
        initVideoRewardDatas();
    const rapidjson::Document& videoRewardDataDoc = JsonHelper::parseStringToJson(videoRewardDataJson);
    rapidjson::Document videoRewardUserDataDoc = JsonHelper::copyDocument(JsonHelper::parseStringToJson(videoRewardUserDataJson));

    bool enable = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].GetBool();
    if (enable == false)
        return 0;
    double availableAt = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble();
    double currentTime = GameUtils::getCurrentTime();
    if (currentTime < availableAt)
        return 0;
    int currentStep = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt();
    int maxStep     = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_STEP].GetInt();
    if (currentStep >= maxStep)
        return 0;
    double bonus             = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_REWARD_MONEYS][currentStep].GetInt();
    double currentTimeDouble = GameUtils::getCurrentTime();
    double lastClaimDouble   = getLastVideoRewardClaimedTime(videoRewardUserDataDoc);
    double dateDiff          = GameUtils::dayDiff(currentTimeDouble, lastClaimDouble);

    videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT].SetDouble(currentTimeDouble);
    if (dateDiff > 0)
    {
        updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
        resetVideoRewardDatas();
    }
    else
    {
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].SetInt(currentStep + 1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(currentTime + 30 * 60);
#else
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(currentTime + videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_COUNT_DOWN_TIME].GetInt());
#endif
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP_SKIPPED].SetBool(false);
        updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
    }
    return bonus;
}

double SvrTimeBonus::claimVideoRewardExtraBonus()
{
    std::string videoRewardDataJson     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserDataJson = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    if (videoRewardDataJson.empty() == true || videoRewardUserDataJson.empty() == true)
        initVideoRewardDatas();
    const rapidjson::Document& videoRewardDataDoc = JsonHelper::parseStringToJson(videoRewardDataJson);
    rapidjson::Document videoRewardUserDataDoc = JsonHelper::copyDocument(JsonHelper::parseStringToJson(videoRewardUserDataJson));

    bool enable = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].GetBool();
    if (enable == false)
        return 0;
    double availableAt = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble();
    double currentTime = GameUtils::getCurrentTime();
    if (currentTime < availableAt)
        return 0;

    int currentStep = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt();
    int maxStep     = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_STEP].GetInt();

    if (currentStep < maxStep || currentStep > maxStep)
        return 0;

    float  totalPercent = 0;
    float  rand         = GameUtils::randomDoubleInRange(0.0f, 1.0f);
    double bonus        = 0;
    const rapidjson::Value& valArray = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS];
    for (int i                 = 0; valArray.IsArray() && i < valArray.Size(); i++)
    {
        totalPercent += valArray[i][JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_RATIO].GetFloat();
        if (rand < totalPercent)
        {
            bonus = valArray[i][JSON_VIDEO_REWARDS_DATA_BONUS_REWARD_MONEYS_MONEY].GetInt();
            break;
        }
    }
    double   currentTimeDouble = GameUtils::getCurrentTime();
    double   lastClaimDouble   = getLastVideoRewardClaimedTime(videoRewardUserDataDoc);
    double   dateDiff          = GameUtils::dayDiff(currentTimeDouble, lastClaimDouble);

    videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT].SetDouble(currentTimeDouble);
    if (dateDiff > 0)
    {
        updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
        resetVideoRewardDatas();
    }
    else
    {
        int nextRound = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND].GetInt() + 1;
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND].SetInt(nextRound);
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].SetInt(0);
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP_SKIPPED].SetBool(false);
        if (nextRound < videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_ROUND].GetInt())
        {
            videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].SetBool(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(currentTime + 30 * 60);
#else
            videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(currentTime + videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_COUNT_DOWN_TIME].GetInt());
#endif
        }
        else
        {
            videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].SetBool(false);
            videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(-1);
        }
        resetVideoRewardDatas();
        updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
    }
    return bonus;
}

std::string SvrTimeBonus::skipVideoRewardStep()
{
    std::string videoRewardDataJson     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserDataJson = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    if (videoRewardDataJson.empty() == true || videoRewardUserDataJson.empty() == true)
        initVideoRewardDatas();
    const rapidjson::Document& videoRewardDataDoc = JsonHelper::parseStringToJson(videoRewardDataJson);
    rapidjson::Document videoRewardUserDataDoc = JsonHelper::copyDocument(JsonHelper::parseStringToJson(videoRewardUserDataJson));

    int currentStep   = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt();
    int skippableStep = videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_SKIPPABLE_STEP].GetInt();
    if (skippableStep <= currentStep)
    {
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].SetDouble(GameUtils::getCurrentTime());
        videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP_SKIPPED].SetBool(true);
        updateUserDataByKey(KEY_VIDEO_REWARD_USER_DATA, videoRewardUserDataDoc);
        videoRewardUserDataJson = JsonHelper::convertDocumentToString(videoRewardUserDataDoc);
    }
    return videoRewardUserDataJson;
}

double SvrTimeBonus::getLastVideoRewardClaimedTime(rapidjson::Document& videoRewardUserDataDoc)
{
    double time = JsonHelper::getDouble(videoRewardUserDataDoc, JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT, -1.0f);
    if (time == -1)
    {
        time = GameUtils::getCurrentTime();
        videoRewardUserDataDoc.AddMember(JSON_VIDEO_REWARDS_USER_DATA_LAST_REWARD_CLAIMED_AT, time, videoRewardUserDataDoc.GetAllocator());
    }
    return time;
}

double SvrTimeBonus::getLastVideoRewardClaimedTime()
{
    std::string videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);
    if (videoRewardUserData.empty() == true)
        return 0;
    rapidjson::Document doc = JsonHelper::parseStringToJson(videoRewardUserData);
    return getLastVideoRewardClaimedTime(doc);
}


bool SvrTimeBonus::isVideoRewardEnabled()
{
    bool isEnable           = RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_TIME_BONUS_ENABLE, true);
    int  currentVersionCode = s_versionCode;
    int  minimumVersionCode = RemoteConfig::getInteger(REMOTECONFIG_FUNCTION_TIME_BONUS_VERSION_CODE, 0);
    return ((isEnable == true) && (minimumVersionCode <= currentVersionCode));
}

double SvrTimeBonus::getCurrentVideoRewardAvailableAt(bool shouldReset)
{
    checkAndResetVideoRewardDataOnNewDay(shouldReset);
    std::string videoRewardData     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);

    const rapidjson::Document& videoRewardDataDoc     = JsonHelper::parseStringToJson(videoRewardData);
    const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);

    int    round       = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_ROUND].GetInt();
    double availableAt = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble();
    int    countDown   = (double) std::max(videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble(), (double) 0) - GameUtils::getCurrentTime();
    bool   skippable   = (videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_CURRENT_STEP].GetInt() >= videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_SKIPPABLE_STEP].GetInt());
    if (round < videoRewardDataDoc[JSON_VIDEO_REWARDS_DATA_MAX_ROUND].GetInt())
        return ((skippable == true) || (skippable == false && countDown <= 0)) ? (double) GameUtils::getCurrentTime() : availableAt;
    return -1;
}

void SvrTimeBonus::checkAndResetVideoRewardDataOnNewDay(bool shouldReset)
{
    double currentTimeDouble = GameUtils::getCurrentTime();
    double lastClaimAt       = getLastVideoRewardClaimedTime();
    double dateDiff          = GameUtils::dayDiff(currentTimeDouble, lastClaimAt);

    std::string videoRewardData     = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_DATA);
    std::string videoRewardUserData = GameUtils::getStringForKeyUserDefault(KEY_VIDEO_REWARD_USER_DATA);

    if (videoRewardData.empty() == true || videoRewardUserData.empty() == true || (shouldReset == true && dateDiff > 0))
        resetVideoRewardDatas();
}

double SvrTimeBonus::getVideoRewardResetTime()
{
    return GameUtils::getStartTimeOfNextDay(GameUtils::getCurrentTime());
}

void SvrTimeBonus::updateUserDataByKey(const std::string& key, const rapidjson::Document& doc)
{
    std::string json = JsonHelper::convertDocumentToString(doc);
    GameUtils::setStringForKeyUserDefault(key.c_str(), json);
}
