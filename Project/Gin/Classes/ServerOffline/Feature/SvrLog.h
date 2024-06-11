#ifndef __SVR_LOG_H__
#define __SVR_LOG_H__

#include <string>
#include <map>

class SvrLog
{
private:
    static std::map<std::string, double> _mapEarnMoney;
    static std::map<std::string, double> _mapSpendMoney;

    static void clearSpendMoney();
    static void clearEarnMoney();
protected:
public:
    static void addSpendMoney(const std::string& name, double amount);
    static void addEarnMoney(const std::string& name, double amount);
    static void logEarnSpendMoney();
    static void botWinLose(int version, int level, double rewardMoney, int cityId, bool isWin);
    static void botUpDownVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney);
};

#endif //__SVR_LOG_H__
