#include "Firebase.h"
#include "Define/GameDefine.h"

using namespace std;
USING_NS_CC;

bool Firebase::shouldLog()
{
    return true;
}

void Firebase::logGetMoneyDefault(const string& screenId, float money)
{
    if (shouldLog())
        DLOG_FB(screenId, money);
}

void Firebase::logGetMoneyWatchVideo(const string& screenId, int orderId, float money)
{
    if (shouldLog())
        DLOG_FB(screenId, orderId, money);
}

void Firebase::logGetMoneyWelcomeBack(const string& screenId, int day, float money)
{
    if (shouldLog())
        DLOG_FB(screenId, day, money);
}

void Firebase::logOpenAppRating()
{
    if (shouldLog())
        DLOG_FB("");
}

void Firebase::logOpenWatchVideo(const string& state)
{
    if (shouldLog())
        DLOG_FB(state);
}

void Firebase::logOpenTutorial()
{
    if (shouldLog())
        DLOG_FB("");
}
void Firebase::logContinueToPlay(double bet, float ratio)
{
    if (shouldLog())
        DLOG_FB(bet, ratio);
}

void Firebase::logUnlockCity(const string& type, double money)
{
    if (shouldLog())
        DLOG_FB(type, money);
}

void Firebase::logUnlockTable(const string& type, double money)
{
    if (shouldLog())
        DLOG_FB(type, money);
}

void Firebase::logStartMatch(const string& cityType, const string& tableType, double money, double bet, float ratio, float speed, int orderId)
{
    if (shouldLog())
        DLOG_FB(cityType, tableType, money, bet, ratio, speed, orderId);
}

void Firebase::logUserProperties(const string& gameVersion, const string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level)
{
    if (shouldLog())
        DLOG_FB(gameVersion, moneyName, winNumber, loseNumber, winHitpotNumber, level);
}

void Firebase::logWinResult(int tier, const string& type, double money, double currentMoney, double bet, const string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
    if (shouldLog())
        DLOG_FB(tier, type, money, currentMoney, bet, cityType, matchCount, score, numBonusCard, numSecret);
}

void Firebase::logLoseResult(int tier, const string& type, double money, double currentMoney, double bet, const string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
    if (shouldLog())
        DLOG_FB(tier, type.c_str(), money, currentMoney, bet, cityType.c_str(), matchCount, score, numBonusCard, numSecret);
}

void Firebase::logTotalWatchVideoNumber(int videoCount)
{
    if (shouldLog())
        DLOG_FB(videoCount);
}

void Firebase::logShowFullAds()
{
    if (shouldLog())
        DLOG_FB("");
}

void Firebase::logOpenApp(const string& appName)
{
    if (shouldLog())
        DLOG_FB(appName);
}

void Firebase::logOpenRatingPopup(int stateRatingPopup, const string& conditionName)
{
    if (shouldLog())
        DLOG_FB(stateRatingPopup, conditionName);
}

void Firebase::logClickButtonRatingPopup(int stateRatingPopup, const string& conditionName, int star, const string& btnClickName, const string& where)
{
    if (shouldLog())
        DLOG_FB(stateRatingPopup, conditionName, star, btnClickName, where);
}

void Firebase::logClickStarPopup(int stateRatingPopup, const string& conditionName, int star)
{
    if (shouldLog())
        DLOG_FB(stateRatingPopup, conditionName, star);
}

void Firebase::logGameCenterAction(const string& game, int state)
{
    if (shouldLog())
        DLOG_FB(game, state);
}

void Firebase::logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv)
{
    if (shouldLog())
        DLOG_FB(step, value, watchedVideoReward, lv);
}

void Firebase::logClaimMailboxReward(const string& id, double value)
{
    if (shouldLog())
        DLOG_FB(id, value);
}

void Firebase::logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet)
{
    if (shouldLog())
        DLOG_FB(slotOrder, round, moneyUser, moneyHitpot, moneyBet);
}

void Firebase::logDoAction(const string& type, int score)
{
    if (shouldLog())
        DLOG_FB(type, score);
}

void Firebase::logClickSortCard(const string& type)
{
    if (shouldLog())
        DLOG_FB(type);
}

void Firebase::logClickViewResult()
{
    if (shouldLog())
        DLOG_FB("");
}

void Firebase::logChangeAvatar(const string& name)
{
    if (shouldLog())
        DLOG_FB(name);
}

void Firebase::logSpendMoney(const string& name, double amount)
{
    if (shouldLog())
        DLOG_FB(name, amount);
}

void Firebase::logEarnMoney(const string& name, double amount)
{
    if (shouldLog())
        DLOG_FB(name, amount);
}

void Firebase::logNetwork(bool online)
{
    if (shouldLog())
        DLOG_FB(online);
}

void Firebase::logBotWin(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        DLOG_FB(version, level, rewardMoney, cityId);
}

void Firebase::logBotLose(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        DLOG_FB(version, level, rewardMoney, cityId);
}

void Firebase::logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        DLOG_FB(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
}

void Firebase::logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        DLOG_FB(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
}
