#include "SvrDailyBonus.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"

void SvrDailyBonus::resetLogInBonusCounter()
{
    GameUtils::setIntegerForKeyUserDefault(KEY_CURRENT_LOG_IN_BONUS_COUNTER, 1);
}

int SvrDailyBonus::getLogInBonusCounter()
{
    return GameUtils::getIntegerForKeyUserDefault(KEY_CURRENT_LOG_IN_BONUS_COUNTER, 1);
}

void SvrDailyBonus::toNextLogInBonusCounter()
{
    int currentCounter = getLogInBonusCounter();
    currentCounter = (currentCounter + 1);
    currentCounter = currentCounter <= MAX_LOG_IN_BONUS_COUNTER ? currentCounter : MAX_LOG_IN_BONUS_COUNTER;
    GameUtils::setIntegerForKeyUserDefault(KEY_CURRENT_LOG_IN_BONUS_COUNTER, currentCounter);
}

double SvrDailyBonus::getLogInBonusMoney()
{
    return USER_DEFAULT_DAILY_LOGIN_BONUS * getLogInBonusCounter();
}