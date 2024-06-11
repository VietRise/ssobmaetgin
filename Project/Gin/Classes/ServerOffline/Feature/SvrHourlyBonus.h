#ifndef __SVR_HOURLY_BONUS_H__
#define __SVR_HOURLY_BONUS_H__

#include <map>
#include <vector>

class SvrHourlyBonus
{
private:
    static std::map<int, std::vector<double>> _hourlyBonus;
    static std::vector<double>                _hourlyNextTime;

    static int getCurrentHourlyBonusIndex();
    static int toNextHourlyBonusReward();
protected:
public:
    static void initHourlyBonus();
    static double getCurrentHourlyBonusCountDown();
    static double getCurrentHourlyBonus();
    static int applyClaimHourlyReward();
};

#endif //__SVR_HOURLY_BONUS_H__
