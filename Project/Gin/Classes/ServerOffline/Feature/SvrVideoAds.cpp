#include "SvrVideoAds.h"
#include "Define/GameDefine.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"

void SvrVideoAds::initWatchVideos()
{
    GameUtils::setStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO, "{\"last_update\":0,\"watch_count\":0}");
}

bool SvrVideoAds::isReachVideoWatchPerDay()
{
    return (getWatchedVideoCount() >= USER_MAXIMUM_VIDEO_WATCHED_PER_DAY);
}

void SvrVideoAds::updateWatchVideoCount()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    if (json == "")
        initWatchVideos();
    json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    rapidjson::Document newDoc = JsonHelper::copyDocument(JsonHelper::parseStringToJson(json));

    double lastUpdateDouble  = newDoc[JSON_LAST_UPDATE_DATE].GetDouble();
    double currentTimeDouble = GameUtils::getCurrentTime();

    if (lastUpdateDouble != 0)
    {
        if (GameUtils::isInSameDay(currentTimeDouble, lastUpdateDouble) == false)
            revewWatchVideoCount();
    }
    else
    {
        revewWatchVideoCount();
    }
    int count = getWatchedVideoCount();
    newDoc[JSON_WATCHED_VIDEO_COUNT].SetInt(count + 1);
    newDoc[JSON_LAST_UPDATE_DATE].SetDouble(currentTimeDouble);
    std::string newJson = JsonHelper::convertDocumentToString(newDoc);
    GameUtils::setStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO, newJson);
}

void SvrVideoAds::revewWatchVideoCount()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    if (json.empty() == true)
        initWatchVideos();
    json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    rapidjson::Document newDoc      = JsonHelper::copyDocument(JsonHelper::parseStringToJson(json));
    double              currentTime = GameUtils::getCurrentTime();
    newDoc[JSON_LAST_UPDATE_DATE].SetDouble(currentTime);
    newDoc[JSON_WATCHED_VIDEO_COUNT].SetInt(0);
    GameUtils::setStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO, JsonHelper::convertDocumentToString(newDoc));
}

int SvrVideoAds::getWatchedVideoCount()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    if (json.empty() == true)
        initWatchVideos();
    json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    return JsonHelper::getInt(JsonHelper::parseStringToJson(json), JSON_WATCHED_VIDEO_COUNT, 0);
}

double SvrVideoAds::getWatchVideoBonus()
{
    int count = getWatchedVideoCount();
    if (count > 1)
    {
        double bonus = USER_DEFAULT_VIDEO_BONUS + count * USER_DEFAULT_VIDEO_BONUS_STEP * USER_DEFAULT_VIDEO_BONUS;
        if (bonus >= USER_DEFAULT_VIDEO_BONUS_MAX)
            return USER_DEFAULT_VIDEO_BONUS_MAX;
        return bonus;
    }
    else if (count == 1)
        return USER_DEFAULT_VIDEO_BONUS;
    return 0;
}

double SvrVideoAds::getNextWatchVideoBonus()
{
    if (isReachVideoWatchPerDay() == false)
    {
        int count = getWatchedVideoCount() + 1;
        if (count > 1)
        {
            double bonus = USER_DEFAULT_VIDEO_BONUS + count * USER_DEFAULT_VIDEO_BONUS_STEP * USER_DEFAULT_VIDEO_BONUS;
            if (bonus >= USER_DEFAULT_VIDEO_BONUS_MAX)
                return USER_DEFAULT_VIDEO_BONUS_MAX;
            return bonus;
        }
        else if (count == 1)
            return USER_DEFAULT_VIDEO_BONUS;
        else
            return 0;
    }
    return 0;
}

void SvrVideoAds::resetWatchVideoBonus()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    if (json.empty() == true)
        initWatchVideos();
    json = GameUtils::getStringForKeyUserDefault(KEY_LIST_WATCHED_VIDEO);
    const rapidjson::Document& newDoc = JsonHelper::parseStringToJson(json);
    double lastUpdateDouble  = newDoc[JSON_LAST_UPDATE_DATE].GetDouble();
    double currentTimeDouble = GameUtils::getCurrentTime();
    if (lastUpdateDouble != 0)
    {
        if (GameUtils::isInSameDay(currentTimeDouble, lastUpdateDouble) == false)
            revewWatchVideoCount();
    }
    else
    {
        revewWatchVideoCount();
    }
}
