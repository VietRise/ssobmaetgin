#ifndef FirebaseAnalyticsController_h
#define FirebaseAnalyticsController_h

@interface FirebaseAnalyticsController: NSObject

+ (void) initFirebaseAnalytics;

+ (void) logGetMoneyDefault: (NSString*) sceneId: (float) money;
+ (void) logGetMoneyWatchVideo: (NSString*)sceneId: (int)orderId: (float)money;
+ (void) logGetMoneyWelcomeBack: (NSString*)sceneId: (int)day: (float)money;
+ (void) logOpenAppRating;
+ (void) logOpenWatchVideo: (NSString*) state;
+ (void) logOpenTutorial;
+ (void) logContinueToPlay: (double) bet: (float)ratio;
+ (void) logUnlockCity: (NSString*) type: (double)money;
+ (void) logUnlockTable: (NSString*) type: (double)money;
+ (void) logStartMatch: (NSString*) cityType: (NSString*) tableType: (double)money: (double)bet: (float)ratio: (float)speed: (int)orderId;
+ (void) logSelectBet: (double) currentMoney: (double) money;
+ (void) logUserProperties: (NSString*) gameVersion: (NSString*)moneyName: (int)winNumber: (int)loseNumber: (int)winHitpotNumber: (int)level;
+ (void) logWinResult: (int)tier: (NSString*) type: (double)money: (double)currMoney: (double)bet: (NSString*)cityType: (int)matchCount: (int)score :(int)numBonusCard : (int)numSecret;
+ (void) logLoseResult: (int)tier: (NSString*) type: (double)money: (double)currMoney: (double)bet: (NSString*)cityType: (int)matchCount: (int)score: (int)numBonusCard: (int)numSecret;
+ (void) logWinHitPot: (int)slotOrder :(int)round : (double) moneyUser: (double) moneyHitpot : (double) moneyBet;
+ (void) logDoAction: (NSString*) type : (int) score;
+ (void) logClickSortCard: (NSString*) type;
+ (void) logClickViewResult;
+ (void) logTotalWatchVideoNumber: (int) number;
+ (void) logShowFullAds;
+ (void) logOpenApp: (NSString*) appName;
+ (void) logOpenRatingPopup: (int) stateRatingPopup : (NSString*) conditionName;
+ (void) logClickButtonRatingPopup: (int) stateRatingPopup : (NSString*) conditionName : (int) star : (NSString*) btnClickName : (NSString*) where;
+ (void) logClickStarPopup: (int) stateRatingPopup : (NSString*) conditionName : (int) star;
+ (void) logGameCenterAction: (NSString*) game : (int) state;
+ (void) logClaimTimeBonus: (int) step : (double) value : (bool) watchedVideoReward : (int) lv;
+ (void) logClaimMailboxReward: (NSString*) id : (double) value;
+ (void) logChangeAvatar: (NSString*) name;
+ (void) logSpendMoney: (NSString*) name: (double) amount;
+ (void) logEarnMoney: (NSString*) name: (double) amount;
+ (void) logOpenLocalNotification: (NSString*) name: (int) id;
+ (void) logReceiveLocalNotification: (NSString*) name: (int) id;
+ (void) logInterstitialAdsImpression:(NSString*)mediationName;
+ (void) logVideoAdsImpression:(NSString*)mediationName;
+ (void) logAdsLoadFail:(NSString*)name :(int)errorCode :(NSString*)errorMsg :(NSString*)cause :(NSString*)domain;
+ (void) logNetwork: (bool)online;
+ (void) logShowInAppReview:(int)ratingState: (NSString*)conditionName;
+ (void) logBotWin: (int)version: (int)level: (double)rewardMoney: (int)cityId;
+ (void) logBotLose: (int)version: (int)level: (double)rewardMoney: (int)cityId;
+ (void) logBotUpgradeVersion: (int)newVersion: (int)oldVersion: (double)userWinRate: (int)userMatchCount: (double)userMoney;
+ (void) logBotDowngradeVersion: (int)newVersion: (int)oldVersion: (double)userWinRate: (int)userMatchCount: (double)userMoney;
@end

#endif /* FirebaseAnalyticsController_h */
