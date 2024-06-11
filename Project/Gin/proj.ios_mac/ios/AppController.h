#import <UIKit/UIKit.h>
#import "FirebaseAnalyticsController.h"
#import "FirebaseNotification.h"
#import "FirebaseRC.h"
#import "Ads/AdManager.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    FirebaseNotification* _firebaseNotifcation;
    AdManager* _adManager;
    BOOL _checkStartupDone;
}

+(void) openURL:(NSString*)url;
+ (bool) checkInternet;
+ (void) openAppInStore;
+ (void) openAnotherAppInStore: (NSString*) url;
+ (void) openAnotherAppInStoreByLink: (NSString*) url;
+ (void) showAlert: (NSString*)  message;
+ (BOOL) isPackageInstalled: (NSString*) packageName;
+ (int) getVersionCode;
+ (NSString*) getVersionName;
+ (NSString*) getMyPackageName;
- (void) shareDataToMessenger: (NSString*)title : (NSString*)message;
+ (BOOL) hasSafeArea;
- (BOOL) checkStartup;
+(void) copyToClipboard: (NSString*)str;
// App Tracking Transparency
+(void) showAppTrackingTransparency;
// Local notification
+ (void) scheduleLocalNotification:(NSString*) dataJson;
+ (void) cancelAllLocalNotification;
+ (void) cleanAllLocalNotification;
+ (void) addAlarmLocalNotification: (int)id : (NSString*)name : (NSString*) type : (NSString*)title : (NSString*)message : (NSString*)image : (double)delayTime;
// Admob
-(void) initAds;
-(void) initAdsManager;
-(void) setupAdsConsentAndInitAdsManager;

-(void) resumeGame;
-(void) pauseGame;

-(void) scheduleResumeGame;
-(void) schedulePauseGame;

+(void) callResumeGame;
+(void) callPauseGame;

+ (BOOL) willPlayBackgroundMusic;

@property(nonatomic, readonly) RootViewController* viewController;

@end

