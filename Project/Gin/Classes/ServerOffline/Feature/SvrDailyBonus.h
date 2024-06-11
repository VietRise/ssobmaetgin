#ifndef __SVR_DAILY_BONUS_H__
#define __SVR_DAILY_BONUS_H__

class SvrDailyBonus
{
private:
    static int getLogInBonusCounter();
protected:
public:
    static double getLogInBonusMoney();
    static void toNextLogInBonusCounter();
    static void resetLogInBonusCounter();
};

#endif //__SVR_DAILY_BONUS_H__
