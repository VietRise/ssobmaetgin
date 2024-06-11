#ifndef FIREBASE_H
#define FIREBASE_H

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#endif

class Firebase
{
public:
    static bool shouldLog();

    static void logGetMoneyDefault(const std::string& screenId, float money);
    static void logGetMoneyWatchVideo(const std::string& screenId, int orderId, float money);
    static void logGetMoneyWelcomeBack(const std::string& screenId, int day, float money);
    static void logOpenAppRating();
    static void logOpenWatchVideo(const std::string& state);
    static void logOpenTutorial();
    static void logContinueToPlay(double bet, float ratio);
    static void logUnlockCity(const std::string& type, double money);
    static void logUnlockTable(const std::string& type, double money);
    static void logStartMatch(const std::string& cityType, const std::string& tableType, double money, double bet, float ratio, float speed, int orderId);
    static void logUserProperties(const std::string& gameVersion, const std::string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level);
    static void logWinResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecretCard);
    static void logLoseResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecretCard);
    static void logTotalWatchVideoNumber(int videoCount);
    static void logShowFullAds();
    static void logOpenApp(const std::string& appName);
    static void logOpenRatingPopup(int stateRatingPopup, const std::string& conditionName);
    static void logClickButtonRatingPopup(int stateRatingPopup, const std::string& conditionName, int star, const std::string& btnClickName, const std::string& where);
    static void logClickStarPopup(int stateRatingPopup, const std::string& conditionName, int star);
    static void logGameCenterAction(const std::string& game, int state);
    static void logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv);
    static void logClaimMailboxReward(const std::string& id, double value);
    static void logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet);
    static void logDoAction(const std::string& type, int score);
    static void logClickSortCard(const std::string& type);
    static void logClickViewResult();
    static void logChangeAvatar(const std::string& name);
    static void logSpendMoney(const std::string& name, double amount);
    static void logEarnMoney(const std::string& name, double amount);
    static void logNetwork(bool online);
    static void logBotWin(int version, int level, double rewardMoney, int cityId);
    static void logBotLose(int version, int level, double rewardMoney, int cityId);
    static void logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney);
    static void logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static void JNI_logGetMoneyDefault(const std::string& screenId, float money);
    static void JNI_logGetMoneyWatchVideo(const std::string& screenId, int orderId, float money);
    static void JNI_logGetMoneyWelcomeBack(const std::string& screenId, int day, float money);
    static void JNI_logOpenAppRating();
    static void JNI_logOpenWatchVideo(const std::string& state);
    static void JNI_logOpenTutorial();
    static void JNI_logContinueToPlay(double bet, float ratio);
    static void JNI_logUnlockCity(const std::string& type, double money);
    static void JNI_logUnlockTable(const std::string& type, double money);
    static void JNI_logStartMatch(const std::string& cityType, const std::string& tableType, double money, double bet, float ratio, float speed, int orderId);
    static void JNI_logUserProperties(const std::string& gameVersion, const std::string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level);
    static void JNI_logWinResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret);
    static void JNI_logLoseResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret);
    static void JNI_logTotalWatchVideoNumber(int videoCount);
    static void JNI_logShowFullAds();
    static void JNI_logOpenApp(const std::string& appName);
    static void JNI_logOpenRatingPopup(int stateRatingPopup, const std::string& conditionName);
    static void JNI_logClickButtonRatingPopup(int stateRatingPopup, const std::string& conditionName, int star, const std::string& btnClickName, const std::string& where);
    static void JNI_logClickStarPopup(int stateRatingPopup, const std::string& conditionName, int star);
    static void JNI_logGameCenterAction(const std::string& game, int state);
    static void JNI_logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv);
    static void JNI_logClaimMailboxReward(const std::string& id, double value);
    static void JNI_logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet);
    static void JNI_logDoAction(const std::string& type, int score);
    static void JNI_logClickSortCard(const std::string& type);
    static void JNI_logClickViewResult();
    static void JNI_logChangeAvatar(const std::string& name);
    static void JNI_logSpendMoney(const std::string& name, double amount);
    static void JNI_logEarnMoney(const std::string& name, double amount);
    static void JNI_logNetwork(bool online);
    static void JNI_logBotWin(int version, int level, double rewardMoney, int cityId);
    static void JNI_logBotLose(int version, int level, double rewardMoney, int cityId);
    static void JNI_logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney);
    static void JNI_logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney);
#endif

};

#endif
