#include "Firebase.h"
#include "cocos2d.h"
#include "Helper/GameUtils.h"
#import "FirebaseAnalyticsController.h"

using namespace std;
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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
        [FirebaseAnalyticsController logGetMoneyDefault:[NSString stringWithUTF8String:screenId.c_str()] :money];
}

void Firebase::logGetMoneyWatchVideo(const std::string& screenId, int orderId, float money)
{
    if (shouldLog())
        [FirebaseAnalyticsController logGetMoneyWatchVideo:[NSString stringWithUTF8String:screenId.c_str()] :orderId :money];
}

void Firebase::logGetMoneyWelcomeBack(const std::string& screenId, int day, float money)
{
    if (shouldLog())
        [FirebaseAnalyticsController logGetMoneyWelcomeBack:[NSString stringWithUTF8String:screenId.c_str()] :day :money];
}

void Firebase::logOpenAppRating()
{
    if (shouldLog())
        [FirebaseAnalyticsController logOpenAppRating];
}

void Firebase::logOpenApp(const std::string& appName)
{
    if (shouldLog())
        [FirebaseAnalyticsController logOpenApp:[NSString stringWithUTF8String:appName.c_str()]];
}

void Firebase::logOpenWatchVideo(const std::string& state)
{
    if (shouldLog())
        [FirebaseAnalyticsController logOpenWatchVideo:[NSString stringWithUTF8String:state.c_str()]];
}

void Firebase::logOpenTutorial()
{
    if (shouldLog())
        [FirebaseAnalyticsController logOpenTutorial];
}

void Firebase::logContinueToPlay(double bet, float ratio)
{
    if (shouldLog())
        [FirebaseAnalyticsController logContinueToPlay:bet :ratio];
}

void Firebase::logUnlockCity(const std::string& type, double money)
{
    if (shouldLog())
        [FirebaseAnalyticsController logUnlockCity:[NSString stringWithUTF8String:type.c_str()] :money];
}

void Firebase::logUnlockTable(const std::string& type, double money)
{
    if (shouldLog())
        [FirebaseAnalyticsController logUnlockTable:[NSString stringWithUTF8String:type.c_str()] :money];
}

void Firebase::logStartMatch(const std::string& cityType, const std::string& tableType , double money, double bet, float ratio, float speed, int orderId)
{
    if (shouldLog())
        [FirebaseAnalyticsController logStartMatch:[NSString stringWithUTF8String:cityType.c_str()] :[NSString stringWithUTF8String:tableType.c_str()] :money :bet :ratio :speed :orderId];
}

void Firebase::logUserProperties(const std::string& gameVersion, const std::string& moneyName, int winNumber, int loseNumber, int winHitpotNumber, int level)
{
    if (shouldLog())
        [FirebaseAnalyticsController logUserProperties:[NSString stringWithUTF8String:gameVersion.c_str()] :[NSString stringWithUTF8String:moneyName.c_str()] :winNumber :loseNumber :winHitpotNumber :level];
}

void Firebase::logWinResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecretCard)
{
    if (shouldLog())
        [FirebaseAnalyticsController logWinResult:tier :[NSString stringWithUTF8String:type.c_str()] :money :currentMoney :bet :[NSString stringWithUTF8String:cityType.c_str()] :matchCount :score :numBonusCard :numSecretCard];
}

void Firebase::logLoseResult(int tier, const std::string& type, double money, double currentMoney, double bet, const std::string& cityType, int matchCount, int score, int numBonusCard, int numSecretCard)
{
    if (shouldLog())
        [FirebaseAnalyticsController logLoseResult:tier :[NSString stringWithUTF8String:type.c_str()] :money :currentMoney :bet :[NSString stringWithUTF8String:cityType.c_str()] :matchCount :score :numBonusCard :numSecretCard];
}

void Firebase::logTotalWatchVideoNumber(int videoCount)
{
    if (shouldLog())
        [FirebaseAnalyticsController logTotalWatchVideoNumber:videoCount];
}

void Firebase::logShowFullAds()
{
    if (shouldLog())
        [FirebaseAnalyticsController logShowFullAds];
}

void Firebase::logOpenRatingPopup(int stateRatingPopup, const std::string& conditionName)
{
    if (shouldLog())
        [FirebaseAnalyticsController logOpenRatingPopup:stateRatingPopup :[NSString stringWithUTF8String:conditionName.c_str()]];
}

void Firebase::logClickButtonRatingPopup(int stateRatingPopup, const std::string& conditionName, int star, const std::string& btnClickName, const std::string& where)
{
    if (shouldLog())
        [FirebaseAnalyticsController logClickButtonRatingPopup:stateRatingPopup :[NSString stringWithUTF8String:conditionName.c_str()] :star :[NSString stringWithUTF8String:btnClickName.c_str()] :[NSString stringWithUTF8String:where
                .c_str()]];
}

void Firebase::logClickStarPopup(int stateRatingPopup, const std::string& conditionName, int star)
{
    if (shouldLog())
        [FirebaseAnalyticsController logClickStarPopup:stateRatingPopup :[NSString stringWithUTF8String:conditionName.c_str()] :star];
}

void Firebase::logGameCenterAction(const std::string& game, int state)
{
    if (shouldLog())
        [FirebaseAnalyticsController logGameCenterAction:[NSString stringWithUTF8String:game.c_str()] :state];
}

void Firebase::logClaimTimeBonus(int step, double value, bool watchedVideoReward, int lv)
{
    if (shouldLog())
        [FirebaseAnalyticsController logClaimTimeBonus:step :value :watchedVideoReward :lv];
}

void Firebase::logClaimMailboxReward(const std::string& id, double value)
{
    if (shouldLog())
        [FirebaseAnalyticsController logClaimMailboxReward:[NSString stringWithUTF8String:id.c_str()] :value];
}

void Firebase::logWinHitpot(int slotOrder, int round, double moneyUser, double moneyHitpot, double moneyBet)
{
    if (shouldLog())
        [FirebaseAnalyticsController logWinHitPot:slotOrder :round :moneyUser :moneyHitpot :moneyBet];
}

void Firebase::logDoAction(const std::string& type, int score)
{
    if (shouldLog())
        [FirebaseAnalyticsController logDoAction: [NSString stringWithUTF8String:type.c_str()] : score];
}

void Firebase::logClickSortCard(const std::string& type)
{
    if (shouldLog())
        [FirebaseAnalyticsController logClickSortCard: [NSString stringWithUTF8String:type.c_str()]];
}

void Firebase::logClickViewResult()
{
    if (shouldLog())
        [FirebaseAnalyticsController logClickViewResult];
}

void Firebase::logChangeAvatar(const std::string& name)
{
    if (shouldLog())
        [FirebaseAnalyticsController logChangeAvatar:[NSString stringWithUTF8String:name.c_str()]];
}

void Firebase::logSpendMoney(const std::string& name, double amount)
{
    if (shouldLog())
        [FirebaseAnalyticsController logSpendMoney:[NSString stringWithUTF8String:name.c_str()] :amount];
}

void Firebase::logEarnMoney(const std::string& name, double amount)
{
    if (shouldLog())
        [FirebaseAnalyticsController logEarnMoney:[NSString stringWithUTF8String:name.c_str()] :amount];
}

void Firebase::logNetwork(bool online)
{
    if (shouldLog())
        [FirebaseAnalyticsController logNetwork:online];
}

void Firebase::logBotWin(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        [FirebaseAnalyticsController logBotWin:version :level :rewardMoney :cityId];
}

void Firebase::logBotLose(int version, int level, double rewardMoney, int cityId)
{
    if (shouldLog())
        [FirebaseAnalyticsController logBotLose:version :level :rewardMoney :cityId];
}

void Firebase::logBotUpgradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        [FirebaseAnalyticsController logBotUpgradeVersion:newVersion :oldVersion :userWinRate :userMatchCount :userMoney];
}

void Firebase::logBotDowngradeVersion(int newVersion, int oldVersion, double userWinRate, int userMatchCount, double userMoney)
{
    if (shouldLog())
        [FirebaseAnalyticsController logBotDowngradeVersion:newVersion :oldVersion :userWinRate :userMatchCount :userMoney];
}

#endif
