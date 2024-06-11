#ifndef FirebaseNotification_h
#define FirebaseNotification_h

@import Firebase;

@interface FirebaseNotification: NSObject{
    NSString* _fcmToken;
    bool _isRegisterAuthorization;
}
-(void) initFirebaseNotification;
-(void) registerForRemoteNotifications: (UIApplication*) application;
-(void) subscirbleTopic;
-(void) registerForRemoteNotificationsWithDeviceToken: (NSData *)deviceToken;
-(void) receiveRemoteNotification: (NSDictionary *)userInfo;
-(void) receiveRemoteNotification: (NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;
-(void) requestAuthorizationCompleted;
// Handle custom data from notification
+(void) checkIncomingIntentFromNotification: (NSDictionary*) userInfo;
@end

#endif /* FirebaseNotification_h */
