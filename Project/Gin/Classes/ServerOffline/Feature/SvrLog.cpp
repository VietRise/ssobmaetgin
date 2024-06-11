#include "SvrLog.h"
#include "FirebaseGG/Firebase.h"

std::map<std::string, double>      SvrLog::_mapEarnMoney;
std::map<std::string, double>      SvrLog::_mapSpendMoney;

void SvrLog::addSpendMoney(const std::string& name, double amount)
{
    SvrLog::_mapSpendMoney[name] += -1 * amount;
}

void SvrLog::addEarnMoney(const std::string& name, double amount)
{
    SvrLog::_mapEarnMoney[name] += amount;
}

void SvrLog::clearSpendMoney()
{
    SvrLog::_mapSpendMoney.clear();
}

void SvrLog::clearEarnMoney()
{
    SvrLog::_mapEarnMoney.clear();
}

void SvrLog::logEarnSpendMoney()
{
    for (auto const& spend : SvrLog::_mapSpendMoney)
        if (spend.first != "" && spend.second != 0)
            Firebase::logSpendMoney(spend.first, spend.second);
    for (auto const& earn : SvrLog::_mapEarnMoney)
        if (earn.first != "" && earn.second != 0)
            Firebase::logEarnMoney(earn.first, earn.second);
    clearSpendMoney();
    clearEarnMoney();
}

void SvrLog::botWinLose(int version, int level, double rewardMoney, int cityId, bool isWin)
{
    if (rewardMoney > 0)
        Firebase::logBotWin(version, level, rewardMoney, cityId);
    else if (rewardMoney < 0)
        Firebase::logBotLose(version, level, rewardMoney, cityId);
}

void SvrLog::botUpDownVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (newVersion > oldVersion)
        Firebase::logBotUpgradeVersion(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
    else if (newVersion < oldVersion)
        Firebase::logBotDowngradeVersion(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
}



