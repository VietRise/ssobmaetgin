#include "SvrLocalNotification.h"
#include "Define/GameDefine.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Define/ResourcesDefine.h"
#include "ServerOffline/Feature/SvrTimeBonus.h"
#include "ServerOffline/Feature/SvrHourlyBonus.h"
#include "ServerOffline/Feature/SvrWelcomeBackBonus.h"
#include "ServerOffline/Feature/SvrLevel.h"

std::vector<LocalNotification> SvrLocalNotification::getLocalNotificationData()
{
    std::vector<LocalNotification> listLocalNotifications;
    std::string                    json = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_LOCAL_NOTIFICATION_DATA, Resources::JSON::JSON_FILE_LOCAL_NOTIFICATION_DATA);
    const rapidjson::Document& objectNotificationJson = JsonHelper::parseStringToJson(json);
    if (objectNotificationJson.IsObject())
    {
        int versionNotification = JsonHelper::getInt(objectNotificationJson, JSON_LOCAL_NOTIFICATION_VERSION, 1);
        if (s_versionCode >= versionNotification)
        {
            const rapidjson::Value& listNotificationJson = JsonHelper::getValue(objectNotificationJson, JSON_LOCAL_NOTIFICATION_NOTIFICATIONS);
            if (listNotificationJson.IsArray())
            {
                for (int i = 0; i < listNotificationJson.Size(); i++)
                {
                    const rapidjson::Value& notification = listNotificationJson[i];
                    int version = JsonHelper::getInt(notification, JSON_LOCAL_NOTIFICATION_VERSION, 1);
                    int number  = JsonHelper::getInt(notification, JSON_LOCAL_NOTIFICATION_NUMBER, 0);
                    const rapidjson::Value& listType = JsonHelper::getValue(notification, JSON_LOCAL_NOTIFICATION_TYPE);
                    std::vector<std::string> types;
                    if (listType.IsArray())
                        for (int             t = 0; t < listType.Size(); t++)
                            types.push_back(listType[t].GetString());
                    if (number == types.size() && s_versionCode >= version)
                    {
                        int id = JsonHelper::getInt(notification, JSON_LOCAL_NOTIFICATION_ID, -1);
                        const std::string& name    = JsonHelper::getString(notification, JSON_LOCAL_NOTIFICATION_NAME, "");
                        const std::string& title   = JsonHelper::convertValueToString(JsonHelper::getValue(notification, JSON_LOCAL_NOTIFICATION_TITLE, rapidjson::Value(rapidjson::kNullType)));
                        const std::string& message = JsonHelper::convertValueToString(JsonHelper::getValue(notification, JSON_LOCAL_NOTIFICATION_MESSAGE, rapidjson::Value(rapidjson::kNullType)));
                        const std::string& image   = JsonHelper::getString(notification, JSON_LOCAL_NOTIFICATION_IMAGE, "");
                        double                         delayTime   = JsonHelper::getDouble(notification, JSON_LOCAL_NOTIFICATION_DELAY_TIME, 0);
                        std::vector<LocalNotification> localNotifications;
                        std::string                    contentType = JsonHelper::getString(notification, JSON_LOCAL_NOTIFICATION_CONTENT_TYPE, "");

                        const rapidjson::Value& listValidTime = JsonHelper::getValue(notification, JSON_LOCAL_NOTIFICATION_VALID_TIME);
                        std::vector<double> validTime;
                        if (listValidTime.IsArray())
                            for (int        t = 0; t < listValidTime.Size(); t++)
                                validTime.push_back(listValidTime[t].GetDouble());

                        if (contentType == "daily_bonus")
                        {
                            if (SvrLocalNotification::getLocalNotificationDailyBonus(localNotifications, id, name, types, title, message, image, delayTime, number, validTime))
                                if (localNotifications.size() > 0)
                                    listLocalNotifications.insert(listLocalNotifications.end(), localNotifications.begin(), localNotifications.end());
                        }
                        else if (contentType == "time_bonus")
                        {
                            if (SvrLocalNotification::getLocalNotificationTimeBonus(localNotifications, id, name, types, title, message, image, delayTime, number, validTime))
                                if (localNotifications.size() > 0)
                                    listLocalNotifications.insert(listLocalNotifications.end(), localNotifications.begin(), localNotifications.end());
                        }
                        else if (contentType == "hourly_bonus")
                        {
                            if (SvrLocalNotification::getLocalNotificationHourlyBonus(localNotifications, id, name, types, title, message, image, delayTime, number, validTime))
                                if (localNotifications.size() > 0)
                                    listLocalNotifications.insert(listLocalNotifications.end(), localNotifications.begin(), localNotifications.end());
                        }
                        else if (contentType == "welcome_back")
                        {
                            if (SvrLocalNotification::getLocalNotificationWelcomeBack(localNotifications, id, name, types, title, message, image, delayTime, number, validTime))
                                if (localNotifications.size() > 0)
                                    listLocalNotifications.insert(listLocalNotifications.end(), localNotifications.begin(), localNotifications.end());
                        }
                        else if (contentType == "unfinished_game")
                        {
                            if (SvrLocalNotification::getLocalNotificationUnfinishedGame(localNotifications, id, name, types, title, message, image, delayTime, number, validTime))
                                if (localNotifications.size() > 0)
                                    listLocalNotifications.insert(listLocalNotifications.end(), localNotifications.begin(), localNotifications.end());
                        }
                    }
                }
            }

            // Validate notifications
            long delayBetweenTwoNotifications = JsonHelper::getInt(objectNotificationJson, JSON_LOCAL_NOTIFICATION_DELAY, 1);
            if (listLocalNotifications.size() > 1)
            {
                std::sort(listLocalNotifications.begin(), listLocalNotifications.end(), [=](const LocalNotification& n1, const LocalNotification& n2) { return n1.delayTime < n2.delayTime; });
                for (int i = 1; i < listLocalNotifications.size(); i++)
                {
                    long gap = listLocalNotifications[i].delayTime - listLocalNotifications[i - 1].delayTime;
                    if (gap < delayBetweenTwoNotifications)
                        listLocalNotifications[i].delayTime = listLocalNotifications[i - 1].delayTime + delayBetweenTwoNotifications;
                }
            }
        }
    }
    return listLocalNotifications;
}

bool SvrLocalNotification::getLocalNotificationDailyBonus(std::vector<LocalNotification>& listNotification,
                                                          const int& id,
                                                          const std::string& name,
                                                          const std::vector<std::string>& type,
                                                          const std::string& title,
                                                          const std::string& message,
                                                          const std::string& image,
                                                          const double& delayTime,
                                                          const int& number,
                                                          const std::vector<double>& validTime)
{
    long long         addTime            = 15.0f;
    long long         currentTime        = GameUtils::getCurrentTime();
    long long         startTimeOfNextDay = GameUtils::getStartTimeOfNextDay(currentTime);
    LocalNotification localNotification;
    localNotification.id         = id;
    localNotification.name       = name;
    localNotification.type       = type[0];
    localNotification.title      = title;
    localNotification.message    = message;
    localNotification.image      = image;
    localNotification.periodTime = startTimeOfNextDay + delayTime + addTime;
    localNotification.delayTime  = localNotification.periodTime - currentTime;
    if (localNotification.periodTime > 0 && delayTime > 0 && SvrLocalNotification::isValidTime(localNotification.periodTime, validTime))
    {
        listNotification.push_back(localNotification);
        return true;
    }
    return false;
}

bool SvrLocalNotification::getLocalNotificationTimeBonus(std::vector<LocalNotification>& listNotification,
                                                         const int& id,
                                                         const std::string& name,
                                                         const std::vector<std::string>& type,
                                                         const std::string& title,
                                                         const std::string& message,
                                                         const std::string& image,
                                                         const double& delayTime,
                                                         const int& number,
                                                         const std::vector<double>& validTime)
{
    std::string videoRewardData     = "";
    std::string videoRewardUserData = "";
    SvrTimeBonus::getVideoRewardData(videoRewardData, videoRewardUserData, true);
    if (videoRewardData != "" && videoRewardUserData != "")
    {
        long long currentTime = GameUtils::getCurrentTime();
        const rapidjson::Document& videoRewardUserDataDoc = JsonHelper::parseStringToJson(videoRewardUserData);
        bool      enable             = videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_ENABLE].GetBool();
        long long avaibleAt          = (double) std::max(videoRewardUserDataDoc[JSON_VIDEO_REWARDS_USER_DATA_AVAILABLE_AT].GetDouble(), (double) 0);
        long long startTimeOfNextDay = GameUtils::getStartTimeOfNextDay(currentTime);

        if (enable && avaibleAt - currentTime > 0)
        {
            double            addTime = 15.0f;
            LocalNotification localNotification;
            localNotification.id         = id;
            localNotification.name       = name;
            localNotification.type       = type[0];
            localNotification.title      = title;
            localNotification.message    = message;
            localNotification.image      = image;
            localNotification.periodTime = avaibleAt + addTime;
            localNotification.delayTime  = localNotification.periodTime - currentTime;
            if (localNotification.periodTime > 0 && localNotification.delayTime > 0 && SvrLocalNotification::isValidTime(localNotification.periodTime, validTime) &&
                localNotification.periodTime < startTimeOfNextDay)
            {
                listNotification.push_back(localNotification);
                return true;
            }
        }
    }
    return false;
}

bool SvrLocalNotification::getLocalNotificationHourlyBonus(std::vector<LocalNotification>& listNotification,
                                                           const int& id,
                                                           const std::string& name,
                                                           const std::vector<std::string>& type,
                                                           const std::string& title,
                                                           const std::string& message,
                                                           const std::string& image,
                                                           const double& delayTime,
                                                           const int& number,
                                                           const std::vector<double>& validTime)
{
    long long avaibleAt          = SvrHourlyBonus::getCurrentHourlyBonusCountDown();
    long long currentTime        = GameUtils::getCurrentTime();
    long long countDown          = avaibleAt - currentTime;
    long long startTimeOfNextDay = GameUtils::getStartTimeOfNextDay(currentTime);
    if (countDown > 0)
    {
        double            addTime = 15.0f;
        LocalNotification localNotification;
        localNotification.id         = id;
        localNotification.name       = name;
        localNotification.type       = type[0];
        localNotification.title      = title;
        localNotification.message    = message;
        localNotification.image      = image;
        localNotification.periodTime = avaibleAt + addTime;
        localNotification.delayTime  = localNotification.periodTime - currentTime;
        if (localNotification.periodTime > 0 && localNotification.delayTime > 0 && SvrLocalNotification::isValidTime(localNotification.periodTime, validTime) &&
            localNotification.periodTime < startTimeOfNextDay)
        {
            listNotification.push_back(localNotification);
            return true;
        }
    }
    return false;
}

bool SvrLocalNotification::getLocalNotificationWelcomeBack(std::vector<LocalNotification>& listNotification,
                                                           const int& id,
                                                           const std::string& name,
                                                           const std::vector<std::string>& type,
                                                           const std::string& title,
                                                           const std::string& message,
                                                           const std::string& image,
                                                           const double& delayTime,
                                                           const int& number,
                                                           const std::vector<double>& validTime)
{
    std::vector<std::pair<int, double>> listWelcomeBack = SvrWelcomeBackBonus::getWelcomeBackBonus(SvrLevel::getLevel());
    if (SvrWelcomeBackBonus::isWelcomeBackEnable() && listWelcomeBack.size() > 0)
    {
        double    addTime            = 15.0f;
        long long currentTime        = GameUtils::getCurrentTime();
        long long startTimeOfNextDay = GameUtils::getStartTimeOfNextDay(currentTime);
        for (int  i                  = 0; i < number; i++)
        {
            if (i < listWelcomeBack.size())
            {
                LocalNotification localNotification;
                localNotification.id         = id + i;
                localNotification.name       = name;
                localNotification.type       = type[i];
                localNotification.title      = title;
                localNotification.message    = message;
                localNotification.image      = image;
                localNotification.periodTime = addTime + startTimeOfNextDay + delayTime + (listWelcomeBack[i].first - 1) * (DATE_IN_MILISECOND / 1000);
                localNotification.delayTime  = localNotification.periodTime - currentTime;
                if (localNotification.periodTime > 0 && localNotification.delayTime > 0 && SvrLocalNotification::isValidTime(localNotification.periodTime, validTime))
                    listNotification.push_back(localNotification);
            }
        }
        return true;
    }
    return false;
}

bool SvrLocalNotification::getLocalNotificationUnfinishedGame(std::vector<LocalNotification>& listNotification,
                                                              const int& id,
                                                              const std::string& name,
                                                              const std::vector<std::string>& type,
                                                              const std::string& title,
                                                              const std::string& message,
                                                              const std::string& image,
                                                              const double& delayTime,
                                                              const int& number,
                                                              const std::vector<double>& validTime)
{
    return false;
}

bool SvrLocalNotification::isValidTime(double periodTime, const std::vector<double>& validTime)
{
    if (validTime.size() == 2)
    {
        double startTimeOfDay = GameUtils::getStartTimeOfDay(periodTime);
        double startValidTime = startTimeOfDay + validTime[0];
        double endValidTime   = startTimeOfDay + validTime[1];
        if (startValidTime <= periodTime && periodTime <= endValidTime)
            return true;
    }
    else
        return true;

    return false;
}
