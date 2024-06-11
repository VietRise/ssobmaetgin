#import <Foundation/Foundation.h>
#import "FirebaseNotification.h"
#import "AppController.h"
@import Firebase;
@import FirebaseMessaging;
// @import FirebaseInstanceID;

#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
@import UserNotifications;
#endif

// Implement UNUserNotificationCenterDelegate to receive display notification via APNS for devices
// running iOS 10 and above. Implement FIRMessagingDelegate to receive data message via FCM for
// devices running iOS 10 and above.
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
@interface FirebaseNotification () <UNUserNotificationCenterDelegate, FIRMessagingDelegate>
@end
#endif

@implementation FirebaseNotification

NSString *const kGCMMessageIDKey = @"gcm.message_id";
NSString *const FIREBASE_OPEN_NOTIFICATION = @"firebase_open";
NSString *const FIREBASE_OPEN_MAIL_BOX = @"mailbox";
NSString *const FIREBASE_OPEN_LUCKY_WHEEL = @"luckywheel";
NSString *const FIREBASE_OPEN_PET = @"pet";
static NSArray* TOPICS;

+(void) initialize
{
    TOPICS = @[@"global"];
    // TOPICS = @[@"test"];
}

-(void) initFirebaseNotification
{
    _isRegisterAuthorization = false;
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tokenRefreshNotification:)
//                                                 name:kFIRInstanceIDTokenRefreshNotification object:nil];
    [FIRMessaging messaging].delegate = self;
    [[FIRMessaging messaging] tokenWithCompletion:^(NSString * _Nullable token, NSError * _Nullable error) {
        if (error != nil) {
            NSLog(@"Error getting FCM registration token: %@", error);
          } else {
            NSLog(@"FCM registration token: %@", token);
            _fcmToken = token;
          }
    }];
    
}

-(void) registerForRemoteNotifications : (UIApplication*) application
{
    if (!_isRegisterAuthorization)
    {
        _isRegisterAuthorization = true;
        if ([UNUserNotificationCenter class] != nil) {
          // iOS 10 or later
          // For iOS 10 display notification (sent via APNS)
          [UNUserNotificationCenter currentNotificationCenter].delegate = self;
          UNAuthorizationOptions authOptions = UNAuthorizationOptionAlert |
              UNAuthorizationOptionSound | UNAuthorizationOptionBadge;
          [[UNUserNotificationCenter currentNotificationCenter]
              requestAuthorizationWithOptions:authOptions
              completionHandler:^(BOOL granted, NSError * _Nullable error) {
                // ...
              NSLog(@"requestAuthorizationWithOptions Finish");
              [self requestAuthorizationCompleted];
          }];
        } else {
          // iOS 10 notifications aren't available; fall back to iOS 8-9 notifications.
          UIUserNotificationType allNotificationTypes =
          (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
          UIUserNotificationSettings *settings =
          [UIUserNotificationSettings settingsForTypes:allNotificationTypes categories:nil];
          [application registerUserNotificationSettings:settings];
        }

        [application registerForRemoteNotifications];
    }
}

-(void) subscirbleTopic
{
    for (int i = 0; i < TOPICS.count; i++) {
        [[FIRMessaging messaging] subscribeToTopic: TOPICS[i]];
        NSLog(@"Subscribed topic: %@", TOPICS[i]);
    }
}


-(void) registerForRemoteNotificationsWithDeviceToken: (NSData *)deviceToken
{
    [FIRMessaging messaging].APNSToken = deviceToken;
//    [[FIRInstanceID instanceID] setAPNSToken:deviceToken
//                                        type:FIRInstanceIDAPNSTokenTypeSandbox];
    //[[FIRInstanceID instanceID] setAPNSToken:deviceToken type:FIRInstanceIDAPNSTokenTypeProd];
}

-(void) receiveRemoteNotification: (NSDictionary *)userInfo
{
    // Print message ID.
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }
    
    // Print full message.
    NSLog(@"%@", userInfo);
}

-(void) receiveRemoteNotification: (NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler
{
    // Print message ID.
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }
    
    // Print full message.
    NSLog(@"%@", userInfo);
    
    completionHandler(UIBackgroundFetchResultNewData);
}

// Handle custom data from notification
+(void) checkIncomingIntentFromNotification: (NSDictionary*) userInfo
{
    NSString* open = [userInfo objectForKey: FIREBASE_OPEN_NOTIFICATION];
    if (open != NULL) {
        NSLog(@"Open: %@", open);
        AppController* app = (AppController*) [[UIApplication sharedApplication] delegate];
        [app callBackSetFirstOpen:open];
    }
}

-(void) requestAuthorizationCompleted
{
    double delayInSeconds = 1.0;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [AppController showAppTrackingTransparency];
    });
}

// ---------------- Delegate UNUserNotificationCenterDelegate -----------------
// [START ios_10_message_handling]
// Receive displayed notifications for iOS 10 devices.
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
// The method will be called on the delegate only if the application is in the foreground. If the method is not implemented or the handler is not called in a timely manner then the notification will not be presented. The application can choose to have the notification presented as a sound, badge, alert and/or in the notification list. This decision should be based on whether the information in the notification is otherwise visible to the user.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
       willPresentNotification:(UNNotification *)notification
         withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler API_AVAILABLE(macos(10.14), ios(10.0), watchos(3.0), tvos(10.0))
{
    // Print message ID.
    NSDictionary *userInfo = notification.request.content.userInfo;
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }
    
    // Print full message.
    NSLog(@"%@", userInfo);
    
    // Change this to your preferred presentation option
    completionHandler(UNNotificationPresentationOptionNone);
}

// The method will be called on the delegate when the user responded to the notification by opening the application, dismissing the notification or choosing a UNNotificationAction. The delegate must be set before the application returns from application:didFinishLaunchingWithOptions:.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
didReceiveNotificationResponse:(UNNotificationResponse *)response
         withCompletionHandler:(void(^)(void))completionHandler API_AVAILABLE(macos(10.14), ios(10.0), watchos(3.0)) API_UNAVAILABLE(tvos)
{
    NSDictionary *userInfo = response.notification.request.content.userInfo;
    if (userInfo[kGCMMessageIDKey]) {
        NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    }
    
    // Print full message.
    NSLog(@"%@", userInfo);
    
    // Check incomming data
    [FirebaseNotification checkIncomingIntentFromNotification:userInfo];
    
    completionHandler();
}

// The method will be called on the delegate when the application is launched in response to the user's request to view in-app notification settings. Add UNAuthorizationOptionProvidesAppNotificationSettings as an option in requestAuthorizationWithOptions:completionHandler: to add a button to inline notification settings view and the notification settings view in Settings. The notification will be nil when opened from Settings.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
   openSettingsForNotification:(nullable UNNotification *)notification API_AVAILABLE(macos(10.14), ios(12.0)) API_UNAVAILABLE(watchos, tvos)
{
    
}
#endif
// [END ios_10_message_handling]

// -------------------- Delegate FIRMessagingDelegate ----------------------
// [START ios_10_data_message_handling]
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
// Receive data message on iOS 10 devices while app is in the foreground.
//- (void)applicationReceivedRemoteMessage:(FIRMessagingRemoteMessage *)remoteMessage {
//    // Print full message
//    NSLog(@"%@", remoteMessage.appData);
//}
- (void)messaging:(FIRMessaging *)messaging
    didReceiveRegistrationToken:(nullable NSString *)fcmToken
    NS_SWIFT_NAME(messaging(_:didReceiveRegistrationToken:))
{
    // Notify about received token.
    NSDictionary *dataDict = [NSDictionary dictionaryWithObject:fcmToken forKey:@"token"];
    [[NSNotificationCenter defaultCenter] postNotificationName:
     @"FCMToken" object:nil userInfo:dataDict];
    _fcmToken = fcmToken;
    // TODO: If necessary send token to application server.
    // Note: This callback is fired at each app startup and whenever a new token is generated.
}
#endif
// [END ios_10_data_message_handling]

@end

