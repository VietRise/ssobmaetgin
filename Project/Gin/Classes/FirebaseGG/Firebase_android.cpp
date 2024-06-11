#include "Firebase.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Define/GameDefine.h"
#include "jni/JniHelper.h"

using namespace std;
USING_NS_CC;

bool Firebase::shouldLog()
{
#if USE_AUTO_PLAY_CARD == 0
    return true;
#else
    return false;
#endif
}

void Firebase::logGetMoneyDefault(const std::string& screenId, float money)
{
    if (shouldLog())
        JNI_logGetMoneyDefault(screenId, money);
}

void Firebase::logGetMoneyWatchVideo(const std::string& screenId, int orderId, float money)
{
    if (shouldLog())
        JNI_logGetMoneyWatchVideo(screenId, orderId, money);
}

void Firebase::logGetMoneyWelcomeBack(const std::string& screenId, int day, float money)
{
    if (shouldLog())
        JNI_logGetMoneyWelcomeBack(screenId, day, money);
}

void Firebase::logOpenAppRating()
{
    if (shouldLog())
        JNI_logOpenAppRating();
}

void Firebase::logOpenWatchVideo(const std::string& state)
{
    if (shouldLog())
        JNI_logOpenWatchVideo(state);
}

void Firebase::logOpenTutorial()
{
    if (shouldLog())
        JNI_logOpenTutorial();
}

void Firebase::logContinueToPlay(double bet, float ratio)
{
    if (shouldLog())
        JNI_logContinueToPlay(bet, ratio);
}

void Firebase::logUnlockCity(const std::string& type, double money)
{
    if (shouldLog())
        JNI_logUnlockCity(type, money);
}

void Firebase::logUnlockTable(const std::string& type, double money)
{
    if (shouldLog())
        JNI_logUnlockTable(type, money);
}

void Firebase::logStartMatch(const std::string& cityType, const std::string& tableType, double money, double bet, float ratio, float speed, int orderId)
{
    if (shouldLog())
        JNI_logStartMatch(cityType, tableType, money, bet, ratio, speed, orderId);
}

void Firebase::logUserProperties(const std::string& gameVersion, const std::string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level)
{
    if (shouldLog())
        JNI_logUserProperties(gameVersion, moneyName, winNumber, loseNumber, winHitpotNumber, level);
}

void Firebase::logWinResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
    if (shouldLog())
        JNI_logWinResult(tier, type, money, currentMoney, bet, cityType, matchCount, score, numBonusCard, numSecret);
}

void Firebase::logLoseResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
    if (shouldLog())
        JNI_logLoseResult(tier, type, money, currentMoney, bet, cityType, matchCount, score, numBonusCard, numSecret);
}

void Firebase::logTotalWatchVideoNumber(int videoCount)
{
    if (shouldLog())
        JNI_logTotalWatchVideoNumber(videoCount);
}

void Firebase::logShowFullAds()
{
    if (shouldLog())
        JNI_logShowFullAds();
}

void Firebase::logOpenApp(const std::string& appName)
{
    if (shouldLog())
        JNI_logOpenApp(appName);
}

void Firebase::logOpenRatingPopup(int stateRatingPopup, const std::string& conditionName)
{
    if (shouldLog())
        JNI_logOpenRatingPopup(stateRatingPopup, conditionName);
}

void Firebase::logClickButtonRatingPopup(int stateRatingPopup, const std::string& conditionName, int star, const std::string& btnClickName, const std::string& where)
{
    if (shouldLog())
        JNI_logClickButtonRatingPopup(stateRatingPopup, conditionName, star, btnClickName, where);
}

void Firebase::logClickStarPopup(int stateRatingPopup, const std::string& conditionName, int star)
{
    if (shouldLog())
        JNI_logClickStarPopup(stateRatingPopup, conditionName, star);
}

void Firebase::logGameCenterAction(const std::string& game, int state)
{
    if (shouldLog())
        JNI_logGameCenterAction(game, state);
}

void Firebase::logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv)
{
    if (shouldLog())
        JNI_logClaimTimeBonus(step, value, watchedVideoReward, lv);
}

void Firebase::logClaimMailboxReward(const std::string& id, double value)
{
    if (shouldLog())
        JNI_logClaimMailboxReward(id, value);
}

void Firebase::logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet)
{
    if (shouldLog())
        JNI_logWinHitpot(slotOrder, round, moneyUser, moneyHitpot, moneyBet);
}

void Firebase::logDoAction(const std::string& type, int score)
{
    if (shouldLog())
        JNI_logDoAction(type, score);
}

void Firebase::logClickSortCard(const std::string& type)
{
    if (shouldLog())
        JNI_logClickSortCard(type);
}

void Firebase::logClickViewResult()
{
    if (shouldLog())
        JNI_logClickViewResult();
}

void Firebase::logChangeAvatar(const std::string& name)
{
    if (shouldLog())
        JNI_logChangeAvatar(name);
}

void Firebase::logSpendMoney(const std::string& name, double amount)
{
    if (shouldLog())
        JNI_logSpendMoney(name, amount);
}

void Firebase::logEarnMoney(const std::string& name, double amount)
{
    if (shouldLog())
        JNI_logEarnMoney(name, amount);
}

void Firebase::logNetwork(bool online)
{
    if (shouldLog())
        JNI_logNetwork(online);
}

void Firebase::logBotWin(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        JNI_logBotWin(version, level, rewardMoney, cityId);
}

void Firebase::logBotLose(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        JNI_logBotLose(version, level, rewardMoney, cityId);
}

void Firebase::logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        JNI_logBotUpgradeVersion(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
}

void Firebase::logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        JNI_logBotDowngradeVersion(newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
}

//#####################################################################################

void Firebase::JNI_logGetMoneyDefault(const std::string& screenId, float money)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logGetMoneyDefault", // method name
                                       "(Ljava/lang/String;F)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(screenId.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, money);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logGetMoneyWatchVideo(const std::string& screenId, int orderId, float money)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logGetMoneyWatchVideo", // method name
                                       "(Ljava/lang/String;IF)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(screenId.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, orderId, money);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logGetMoneyWelcomeBack(const std::string& screenId, int day, float money)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logGetMoneyWelcomeBack", // method name
                                       "(Ljava/lang/String;IF)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(screenId.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, day, money);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logOpenAppRating()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logOpenAppRating", // method name
                                       "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logOpenWatchVideo(const std::string& state)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logOpenWatchVideo", // method name
                                       "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(state.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
    }
}

void Firebase::JNI_logOpenTutorial()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logOpenTutorial", // method name
                                       "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logContinueToPlay(double bet, float ratio)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logContinueToPlay", // method name
                                       "(DF)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, bet, ratio);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logUnlockCity(const std::string& type, double money)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logUnlockCity", // method name
                                       "(Ljava/lang/String;D)V"))
    {

        jstring stringArg1 = t.env->NewStringUTF(type.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, money);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logUnlockTable(const std::string& type, double money)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logUnlockTable", // method name
                                       "(Ljava/lang/String;D)V"))
    {

        jstring stringArg1 = t.env->NewStringUTF(type.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, money);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logStartMatch(const std::string& cityType, const std::string& tableType, double money, double bet, float ratio, float speed, int orderId)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logStartMatch", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;DDFFI)V"))
    {

        jstring stringArg1 = t.env->NewStringUTF(cityType.c_str());
        jstring stringArg2 = t.env->NewStringUTF(tableType.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, money, bet, ratio, speed, orderId);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logUserProperties(const std::string& gameVersion, const std::string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logUserProperties", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;IIII)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(gameVersion.c_str());
        jstring stringArg2 = t.env->NewStringUTF(moneyName.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, winNumber, loseNumber, winHitpotNumber, level);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

 void Firebase::JNI_logWinResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
     JniMethodInfo t;
     if (JniHelper::getStaticMethodInfo(t,
                                        FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                        "logWinResult", // method name
                                        "(ILjava/lang/String;DDDLjava/lang/String;IIII)V"))
     {

         jstring stringArg1 = t.env->NewStringUTF(type.c_str());
         jstring stringArg2 = t.env->NewStringUTF(cityType.c_str());

         t.env->CallStaticVoidMethod(t.classID, t.methodID, tier, stringArg1, money, currentMoney, bet, stringArg2, matchCount, score, numBonusCard, numSecret);
         // Delete unused data
         t.env->DeleteLocalRef(stringArg1);
         t.env->DeleteLocalRef(stringArg2);
         t.env->DeleteLocalRef(t.classID);
    }
}

 void Firebase::JNI_logLoseResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecret)
{
     JniMethodInfo t;
     if (JniHelper::getStaticMethodInfo(t,
                                        FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                        "logLoseResult", // method name
                                        "(ILjava/lang/String;DDDLjava/lang/String;IIII)V"))
     {

         jstring stringArg1 = t.env->NewStringUTF(type.c_str());
         jstring stringArg2 = t.env->NewStringUTF(cityType.c_str());

         t.env->CallStaticVoidMethod(t.classID, t.methodID, tier, stringArg1, money, currentMoney, bet, stringArg2, matchCount, score, numBonusCard, numSecret);
         // Delete unused data
         t.env->DeleteLocalRef(stringArg1);
         t.env->DeleteLocalRef(stringArg2);
         t.env->DeleteLocalRef(t.classID);
     }
}

void Firebase::JNI_logTotalWatchVideoNumber(int videoCount)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logTotalWatchVideoNumber", // method name
                                       "(I)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, videoCount);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logShowFullAds()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logShowFullAds", // method name
                                       "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logOpenApp(const std::string& appName)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logOpenApp", // method name
                                       "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(appName.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logOpenRatingPopup(int stateRatingPopup, const std::string& conditionName)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logOpenRatingPopup", // method name
                                       "(ILjava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(conditionName.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stateRatingPopup, stringArg1);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClickButtonRatingPopup(int stateRatingPopup, const std::string& conditionName, int star, const std::string& btnClickName, const std::string& where)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClickButtonRatingPopup", // method name
                                       "(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V"))
    {
        jstring stringArg0 = t.env->NewStringUTF(conditionName.c_str());
        jstring stringArg1 = t.env->NewStringUTF(btnClickName.c_str());
        jstring stringArg2 = t.env->NewStringUTF(where.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stateRatingPopup, stringArg0, star, stringArg1, stringArg2);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg0);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClickStarPopup(int stateRatingPopup, const std::string& conditionName, int star)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClickStarPopup", // method name
                                       "(ILjava/lang/String;I)V"))
    {
        jstring stringArg0 = t.env->NewStringUTF(conditionName.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stateRatingPopup, stringArg0, star);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg0);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logGameCenterAction(const std::string& game, int state)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logGameCenterAction", // method name
                                       "(Ljava/lang/String;I)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(game.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, state);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClaimTimeBonus", // method name
                                       "(IDZI)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, step, value, watchedVideoReward, lv);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClaimMailboxReward(const std::string& id, double value)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClaimMailboxReward", // method name
                                       "(Ljava/lang/String;D)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(id.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, value);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logWinHitPot", // method name
                                       "(IIDDD)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, slotOrder, round, moneyUser, moneyHitpot, moneyBet);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logDoAction(const std::string& type, int score)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logDoAction", // method name
                                       "(Ljava/lang/String;I)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(type.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, score);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClickSortCard(const std::string& type)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClickSortCard", // method name
                                       "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(type.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logClickViewResult()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logClickViewResult", // method name
                                       "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logChangeAvatar(const std::string& name)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logChangeAvatar", // method name
                                       "(Ljava/lang/String;)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logSpendMoney(const std::string& name, double amount)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logSpendMoney", // method name
                                       "(Ljava/lang/String;D)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, amount);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logEarnMoney(const std::string& name, double amount)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logEarnMoney", // method name
                                       "(Ljava/lang/String;D)V"))
    {
        jstring stringArg1 = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, amount);
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logNetwork(bool online)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logNetwork", // method name
                                       "(Z)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, online);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logBotWin(int version, int level, double rewardMoney, int cityId)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logBotWin", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;DDFFI)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, version, level, rewardMoney, cityId);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logBotLose(int version, int level, double rewardMoney, int cityId)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logBotLose", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;DDFFI)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, version, level, rewardMoney, cityId);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logBotUpgradeVersion", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;DDFFI)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

void Firebase::JNI_logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       FIREBASE_ANALYTICS_CLASS_NAME, // fully qualified class name
                                       "logBotDowngradeVersion", // method name
                                       "(Ljava/lang/String;Ljava/lang/String;DDFFI)V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, newVersion, oldVersion, userWinRate, userMatchCount, userMoney);
        // Delete unused data
        t.env->DeleteLocalRef(t.classID);
    }
}

#endif