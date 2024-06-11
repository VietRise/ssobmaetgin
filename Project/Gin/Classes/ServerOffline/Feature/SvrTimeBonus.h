#ifndef __SVR_TIME_BONUS_H__
#define __SVR_TIME_BONUS_H__

#include <string>
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"

class SvrTimeBonus
{
private:
    static void resetVideoRewardDatas();
    static double claimVideoRewardNormalBonus();
    static double claimVideoRewardExtraBonus();
    static double getLastVideoRewardClaimedTime(rapidjson::Document& videoRewardUserDataDoc);
    static double getLastVideoRewardClaimedTime();
    static void checkAndResetVideoRewardDataOnNewDay(bool shouldReset = true);
    static void updateUserDataByKey(const std::string& key, const rapidjson::Document& doc);
protected:
public:
    static void initVideoRewardDatas();
    static void getVideoRewardData(std::string& videoRewardData, std::string& videoRewardUserData, bool shouldReset = true);
    static bool isVideoRewardEnabled();
    static double getVideoRewardResetTime();
    static std::string skipVideoRewardStep();
    static double getCurrentVideoRewardAvailableAt(bool shouldReset = true);
    static double claimVideoRewardBonus(bool& isLottery);

};

#endif //__SVR_TIME_BONUS_H__
