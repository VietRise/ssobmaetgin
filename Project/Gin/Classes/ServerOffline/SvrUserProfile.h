#ifndef __SVR_USER_PROFILE__
#define __SVR_USER_PROFILE__

#include <string>
#include <vector>

#include "Define/InGameDefine.h"

class SvrUserProfile
{
    friend class ServerOffline;
private:
protected:
public:
    static void initUserMoney();
    static void initUserName();
    static void initUserAvatar();
    static void setPlayingMatchData(const std::string& data);
    static double getMoney();
    static void setMoney(const std::string& name, double bonusAmount);

    static std::string getName();
    static void setName(const std::string& name);

    static std::string getAvatar();
    static void setAvatar(const std::string& path);

    static void updateLastLogIn(double newLastLogIn);
    static std::vector<double> getLogInBonus();

    static void setCountWinMatchUser(int count);
    static int getCountWinMatchUser();
    static void setCountLoseMatchUser(int count);
    static int getCountLoseMatchUser();

    static void setCountWinGameUser(int count);
    static int getCountWinGameUser();
    static void setCountLoseGameUser(int count);
    static int getCountLoseGameUser();

    static void setVersionCode(int version);
    static int getVersionCode();

    static bool isStartupLogin();

    static void updateRawData();

    static void addMatchCount(int city, int count = 1);
    static void addMatchCount(const CityType& city, int count = 1);
    static int getMatchCount(int city);
    static int getMatchCount(const CityType& city);
    static int getTotalMatchCount();

    static void addGameCount(int city, int count = 1);
    static void addGameCount(const CityType& city, int count = 1);
    static int getGameCount(int city);
    static int getGameCount(const CityType& city);
    static int getTotalGameCount();

    static bool isNewUser();

    static void setInterferingAdsEnable(bool enable);
    static bool getInterferingAdsEnable();

    static void getBetData();
    static void getTargetScoreData();
    static void initMoneyRangeName();

    static int getBotVersion();
    static void setBotVersion(int version);
};

#endif
