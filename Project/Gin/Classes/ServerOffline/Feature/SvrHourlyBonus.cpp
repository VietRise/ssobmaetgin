#include "SvrHourlyBonus.h"
#include "ServerOffline/SvrUserProfile.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "ServerOffline/ServerOffline.h"

std::map<int, std::vector<double>> SvrHourlyBonus::_hourlyBonus;
std::vector<double>                SvrHourlyBonus::_hourlyNextTime;

void SvrHourlyBonus::initHourlyBonus()
{
    _hourlyNextTime.clear();
    _hourlyBonus.clear();

    //_hourlyNextTime = {
    //	/*1*/5*1.0f/60,
    //	/*2*/10*1.0f/60,
    //	/*3*/15*1.0f/60,
    //	/*4*/20*1.0f/60,
    //	/*5*/25*1.0f/60,
    //	/*6*/30*1.0f/60
    //};

    _hourlyNextTime = {
            /*1*/15,
            /*2*/15,
            /*3*/15,
            /*4*/15,
            /*5*/30,
            /*6*/30,
            /*7*/30,
            /*8*/30,
            /*9*/30,
            /*10*/30,
            /*11*/30,
            /*12*/30, };

    _hourlyBonus[0] = {
            /*1*/50000, //15
            /*2*/50000, //30
            /*3*/100000, //45
            /*4*/100000, //60
            /*4*/200000, //90
            /*5*/200000, //120
    };
}

double SvrHourlyBonus::getCurrentHourlyBonus()
{
    int index = getCurrentHourlyBonusIndex();
    if (index >= 0 && index < _hourlyBonus[0].size())
        return _hourlyBonus[0][index];
    if (index >= _hourlyBonus[0].size())
        return _hourlyBonus[0][_hourlyBonus[0].size() - 1];
    return 100000;
}

double SvrHourlyBonus::getCurrentHourlyBonusCountDown()
{
    long long time = GameUtils::getDoubleForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_NEXT_TIME, 0);
    return time >= 0 ? time : 0;
}

int SvrHourlyBonus::getCurrentHourlyBonusIndex()
{
    int index = GameUtils::getIntegerForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_INDEX, -1);
    if (index == -1)
        return toNextHourlyBonusReward();
    return index;
}

int SvrHourlyBonus::toNextHourlyBonusReward()
{
    double currentTime  = GameUtils::getCurrentTime();
    double startTime    = GameUtils::getStartTimeOfDay(currentTime);
    double prestartTime = GameUtils::getDoubleForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_START_TIME, 0);
    int    index        = GameUtils::getIntegerForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_INDEX, -1);
    if (index != -1 && startTime != prestartTime) //new day
        index = 0;
    else
        index++;
    if (index >= _hourlyBonus[0].size())
        index = _hourlyBonus[0].size() - 1;
    double bonusNextTime = currentTime + _hourlyNextTime[(index < _hourlyNextTime.size() ? index : _hourlyNextTime.size() - 1)] * 60;
    GameUtils::setIntegerForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_INDEX, index);
    GameUtils::setDoubleForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_NEXT_TIME, bonusNextTime);
    GameUtils::setDoubleForKeyUserDefault(KEY_CURRENT_HOURLY_BONUS_START_TIME, startTime);
    return index;
}

int SvrHourlyBonus::applyClaimHourlyReward()
{
    double countDown = getCurrentHourlyBonusCountDown() - GameUtils::getCurrentTime();
    if (countDown <= 0)
    {
        double money = getCurrentHourlyBonus();
        SvrUserProfile::setMoney("ClaimHourlyReward", money);
        toNextHourlyBonusReward();
        return 1;
    }
    return 0;
}