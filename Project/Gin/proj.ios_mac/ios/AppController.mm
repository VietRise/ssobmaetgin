/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "Reachability.h"
#include "NativeCallBackIOS.h"
#import "PlayGameService.h"
#import "NotificationUtils.h"
#import "StoreService.h"
#import <AppTrackingTransparency/AppTrackingTransparency.h>
#include <UserMessagingPlatform/UserMessagingPlatform.h>
#import "AppDefine.h"
#import "audio/ios/CDAudioManager.h"

#include <UserMessagingPlatform/UserMessagingPlatform.h>


#define KEY_DEVICES_HAS_SAFE_AREA @"function_devices_has_safe_area"
#define VALUE_DEVICES_HAS_SAFE_AREA @"[{\"name\":\"iPhone 11/iPhone XR\",\"height\":1792},{\"name\":\"iPhone 12/13 mini\",\"height\":2340},{\"name\":\"iPhone X/XS/11 Pro\",\"height\":2436},{\"name\":\"iPhone 12/13, iPhone 12/13 Pro\",\"height\":2532},{\"name\":\"iPhone XS Max/11 Pro Max\",\"height\":2688},{\"name\":\"iPhone 12/13 Pro Max\",\"height\":2778}]"

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];


    // Use RootViewController to manage CCEAGLView
    _viewController = [[RootViewController alloc]init];
    _viewController.wantsFullScreenLayout = YES;
    [_viewController.view setMultipleTouchEnabled:NO];
    

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    _checkStartupDone = false;
    _firebaseNotifcation = nil;
    _adManager = nil;
    
    try {
        // Firebase Analytics
        [FirebaseAnalyticsController initFirebaseAnalytics];
        // Firebase Notification
        _firebaseNotifcation = [[FirebaseNotification alloc] init];
        [_firebaseNotifcation initFirebaseNotification];
        [_firebaseNotifcation subscirbleTopic];
        // Firebase Remote config
        [FirebaseRC sharedObject];
        // Game center
        [[PlayGameService shareInstance] create: _viewController];
        // Store service
        [[StoreService shareInstance] create: _viewController];
    } catch (NSException* e) {
    }

    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)_viewController.view);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //run the cocos2d-x game scene
    app->run();

    return YES;
}

- (void)application:(UIApplication*)application didReceiveRemoteNotification:(nonnull NSDictionary *)userInfo
{
    try {
        UIApplicationState state = [application applicationState];
        if(state == UIApplicationStateInactive)
        {
            // _lauched_id = 101;
            NSLog(@"application was in the bg");
        }
        else
        {
            // _lauched_id = 111;
            NSLog(@"application was in active");
        }
        
        // Firebase Notification
        if (_firebaseNotifcation != nullptr) {
            [_firebaseNotifcation receiveRemoteNotification:userInfo];
        }
    } catch (NSException* e) {
    }
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler
{
    try {
        UIApplicationState state = [application applicationState];
        if(state == UIApplicationStateInactive)
        {
            // _lauched_id = 10;
            NSLog(@"application was in the bg");
        }
        else
        {
            // _lauched_id = 11;
            NSLog(@"application was in active");
        }
        
        // Firebase Notification
        if (_firebaseNotifcation != nullptr) {
            [_firebaseNotifcation receiveRemoteNotification:userInfo fetchCompletionHandler:completionHandler];
        }
    } catch (NSException* e) {
    }
}

// Register RemoteNotificationsWithDeviceToken
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    try {
        if(_firebaseNotifcation != nullptr)
        {
            [_firebaseNotifcation registerForRemoteNotificationsWithDeviceToken:deviceToken];
        }
    } catch (NSException* e) {
    }
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    // We don't need to call this method any more. It will interrupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    NSLog(@"applicationDidBecomeActive");
    if (_firebaseNotifcation != nil) {
        [_firebaseNotifcation registerForRemoteNotifications:application];
    }
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [_viewController release];
    [super dealloc];
}
#endif
+(void) openURL:(NSString*)url
{
    try {
        NSURL* nsURL = [NSURL URLWithString:url];
        if ([[UIApplication sharedApplication] canOpenURL: nsURL]) {
            [[UIApplication sharedApplication] openURL:nsURL];
        }
    } catch (NSException* e) {
    }
}

+ (bool) checkInternet
{
    try {
        Reachability *networkReachability = [Reachability reachabilityForInternetConnection];
        NetworkStatus networkStatus = [networkReachability currentReachabilityStatus];
        return !(networkStatus == NotReachable);
    } catch (NSException* e) {
        return false;
    }
    return false;
}

+ (void) openAppInStore
{
    try {
        NSString* appUrl = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/us/app/apple-store/id%@?mt=8", APP_STORE_ID];
        [AppController openURL:appUrl];
    } catch (NSException* e) {
    }
}

+ (void) openAnotherAppInStore: (NSString*) url
{
    try {
        NSString* appUrl = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/us/app/apple-store/id%@?mt=8", url];
        [AppController openURL:appUrl];
    } catch (NSException* e) {
    }
}

+ (void) openAnotherAppInStoreByLink: (NSString*) url
{
    try {
        [AppController openURL:url];
    } catch (NSException* e) {
    }
}

+ (void) showAlert: (NSString*)  message
{
    try {
        UIAlertView *toast = [[UIAlertView alloc] initWithTitle:nil
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:nil
                                              otherButtonTitles:nil, nil];
        [toast show];
        
        int duration = 3; // duration in seconds
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, duration * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
            [toast dismissWithClickedButtonIndex:0 animated:YES];
        });
    } catch (NSException* e) {
    }
}

+ (BOOL) isPackageInstalled: (NSString*) packageName
{
    return false;
}

+ (int) getVersionCode
{
    try {
        NSString * build = [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString*)kCFBundleVersionKey];
        if (build)
            return [build intValue];
    } catch (NSException* e) {
        return 1;
    }
    return 1;
}

+ (NSString*) getVersionName
{
    try {
        NSString * versionName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
        if (versionName)
            return versionName;
    } catch (NSException* e) {
        return @"1.0.0";
    }
    return @"1.0.0";
}

+ (NSString*) getMyPackageName
{
    try {
        NSString * packageName = [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString*)kCFBundleIdentifierKey];
        if (packageName)
            return packageName;
    } catch (NSException* e) {
        return @"";
    }
    return @"";
}

- (void) shareDataToMessenger: (NSString*)title : (NSString*)message
{
    try {
        NSMutableArray *activityItems = [NSMutableArray arrayWithObjects:message, nil];
        UIActivityViewController *activityViewController = [[UIActivityViewController alloc]initWithActivityItems:activityItems applicationActivities:nil];
        activityViewController.excludedActivityTypes = @[
            UIActivityTypePrint,
            UIActivityTypeCopyToPasteboard,
            UIActivityTypeAssignToContact,
            UIActivityTypeSaveToCameraRoll,
            UIActivityTypeAddToReadingList,
            UIActivityTypeAirDrop];
        //if iPad
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            UIPopoverPresentationController* popOver = activityViewController.popoverPresentationController;
            popOver.sourceView = _viewController.view;
        }
        [_viewController presentViewController:activityViewController animated:YES completion:nil];
    } catch (NSException* e) {
    }
}

+ (BOOL) hasSafeArea
{
    try {
        NSString* dataJson = [FirebaseRC getString:KEY_DEVICES_HAS_SAFE_AREA :VALUE_DEVICES_HAS_SAFE_AREA];
        NSError *jsonError;
        NSArray *listDevices = [NSJSONSerialization JSONObjectWithData:[dataJson dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:&jsonError];
        if (listDevices != nil && listDevices.count > 0)
        {
            if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
                int heightDevice = (int)[[UIScreen mainScreen] nativeBounds].size.height;
                for (int i = 0; i < listDevices.count; i++)
                {
                    NSDictionary* device = listDevices[i];
                    if (heightDevice == [[device valueForKey:@"height"] intValue])
                    {
                        NSLog(@"%@", [[device valueForKey:@"name"] stringValue]);
                        return true;
                    }
                }
            }
        }
    } catch (NSException* e) {
        return false;
    }
    return false;
}

- (BOOL) checkStartup
{
    try {
        if ([[FirebaseRC sharedObject] isFetchCompleted])
        {
            _checkStartupDone = true;
        }
        if (_checkStartupDone) {
            return true;
        }
    } catch (NSException* e) {
    }
    return false;
}

+(void) copyToClipboard:(NSString*)str
{
    try {
        UIPasteboard *clipboard = [UIPasteboard generalPasteboard];
        clipboard.string = str;
        [AppController showAlert:@"Copied to Clipboard!"];
    } catch (NSException* e) {
    }
}

// App Tracking Transparency
+(void) showAppTrackingTransparency
{
    try {
        if (@available(iOS 14.0, *))
        {
            [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
                // Tracking authorization completed. Start loading ads here.
                // [self loadAd];
                NSLog(@"showAppTrackingTransparency result: %ld", status);
                // UnitySendMessage([NATIVE_CALLBACK_CLASS_NAME UTF8String], [NATIVE_CALLBACK_METHOD_APP_TRACKING_TRANSPARENCY UTF8String], [[NSString stringWithFormat:@"%ld", status] UTF8String]);
            }];
        }
        else
        {
            // UnitySendMessage([NATIVE_CALLBACK_CLASS_NAME UTF8String], [NATIVE_CALLBACK_METHOD_APP_TRACKING_TRANSPARENCY UTF8String], "-1");
        }
    } catch (NSException* e) {
    }
}

// Local notification
+ (void) scheduleLocalNotification:(NSString*) dataJson
{
    try {
        NSError *jsonError;
        NSArray *listNotification = [NSJSONSerialization JSONObjectWithData:[dataJson dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:&jsonError];
        if (listNotification.count > 0)
        {
            double currentTime = [[NSDate date] timeIntervalSince1970];
            for (int i = 0; i < listNotification.count; i++)
            {
                NSDictionary* notification = listNotification[i];
                int idValue = [[notification valueForKey:FIELD_DATA_ID] intValue];
                NSString* name = [[notification valueForKey:FIELD_DATA_NAME] stringValue];
                NSString* type = [[notification valueForKey:FIELD_DATA_TYPE] stringValue];
                NSString* title = [[notification valueForKey:FIELD_DATA_TITLE] stringValue];
                NSString* message = [[notification valueForKey:FIELD_DATA_MESSAGE] stringValue];
                NSString* image = [[notification valueForKey:FIELD_DATA_IMAGE] stringValue];
                double periodTime = [[notification valueForKey:FIELD_DATA_PERIOD_TIME] doubleValue];
                if (periodTime - currentTime > 0) {
                    [self addAlarmLocalNotification:idValue :name :type :title :message :image : periodTime];
                }
            }
        }
    } catch (NSException* e) {
    }
}

+ (void) cancelAllLocalNotification
{
    try {
        [NotificationUtils cancelAllAlarms];
    } catch (NSException* e) {
    }
}

+ (void) cleanAllLocalNotification
{
    try {
        [NotificationUtils cleanAllNotifications];
    } catch (NSException* e) {
        
    }
}

+ (void) addAlarmLocalNotification: (int)id : (NSString*)name : (NSString*) type : (NSString*)title : (NSString*)message : (NSString*)image : (double)periodTime
{
    try {
        [NotificationUtils sendNotificationNormal:id :name :title :message :image :periodTime : TRUE];
    } catch (NSException* e) {
        
    }
}

// ---------------------- Admob ------------------------
-(void) initAds
{
//    try {
//        // AdManger
//        if (_adManager == nil)
//        {
//            _adManager  = [AdManager shareInstance];
//            [_adManager retain];
//            [_adManager create: _viewController];
//            [_adManager loadAllAds];
//        }
//    } catch (NSException* e) {
//    }
    [self setupAdsConsentAndInitAdsManager];
}

-(void) initAdsManager
{
    try {
        // AdManger
        if (_adManager == nil)
        {
            _adManager  = [AdManager shareInstance];
            [_adManager retain];
            [_adManager create: _viewController];
            [_adManager loadAllAds];
        }
    } catch (NSException* e) {
    }
}

-(void) setupAdsConsentAndInitAdsManager
{
    // Create a UMPRequestParameters object.
    UMPRequestParameters *parameters = [[UMPRequestParameters alloc] init];
    // Set tag for under age of consent. NO means users are not under age
    // of consent.
    parameters.tagForUnderAgeOfConsent = NO;
    
    // Debug
//    UMPDebugSettings *debugSettings = [[UMPDebugSettings alloc] init];
//    debugSettings.testDeviceIdentifiers = @[ @"TEST-DEVICE-HASHED-ID" ];
//    debugSettings.geography = UMPDebugGeographyEEA;
//    parameters.debugSettings = debugSettings;
//    [UMPConsentInformation.sharedInstance reset];
    // End Debug
    
//    __weak __typeof__(self) weakSelf = self;
    // Request an update for the consent information.
    [UMPConsentInformation.sharedInstance
     requestConsentInfoUpdateWithParameters:parameters
     completionHandler:^(NSError *_Nullable requestConsentError) {
        if (requestConsentError) {
            // Consent gathering failed.
            NSLog(@"Error: %@", requestConsentError.localizedDescription);
            return;
        }
        
//        __strong __typeof__(self) strongSelf = weakSelf;
//        if (!strongSelf) {
//            return;
//        }
        
        [UMPConsentForm loadAndPresentIfRequiredFromViewController:_viewController
                                                 completionHandler:^(NSError *loadAndPresentError) {
            if (loadAndPresentError) {
                // Consent gathering failed.
                NSLog(@"Error: %@", loadAndPresentError.localizedDescription);
                return;
            }
            
            // Consent has been gathered.
//            __strong __typeof__(self) strongSelf = weakSelf;
//            if (!strongSelf) {
//                return;
//            }
            
            if (UMPConsentInformation.sharedInstance.canRequestAds) {
                [self initAdsManager];
            }
        }];
    }];
    
    // Check if you can initialize the Google Mobile Ads SDK in parallel
    // while checking for new consent information. Consent obtained in
    // the previous session can be used to request ads.
    if (UMPConsentInformation.sharedInstance.canRequestAds) {
        [self initAdsManager];
    }
}


-(void) resumeGame
{
    // Delay 0.2s to resume game
    SEL aSelectorResumeGame = @selector(scheduleResumeGame);
    [self performSelector: aSelectorResumeGame withObject:nil afterDelay:0.1];
}

-(void) pauseGame
{
    // Delay 0.2s to pause game
    SEL aSelectorPauseGame = @selector(schedulePauseGame);
    [self performSelector: aSelectorPauseGame withObject:nil afterDelay:0.1];
}

-(void) scheduleResumeGame
{
    NativeCallBackIOS::resumeGame();
}

-(void) schedulePauseGame
{
    NativeCallBackIOS::pauseGame();
}

+(void) callResumeGame
{
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    [app resumeGame];
}

+(void) callPauseGame
{
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    [app pauseGame];
}

+ (BOOL) willPlayBackgroundMusic
{
    try {
        return [[CDAudioManager sharedManager] willPlayBackgroundMusic];
    } catch (NSException* e) {
    }
    return false;
}

@end
