#import "NotificationUtils.h"
#import <UserNotifications/UNUserNotificationCenter.h>
#import <UserNotifications/UNNotificationContent.h>
#import <UserNotifications/UNNotificationTrigger.h>
#import <UserNotifications/UNNotificationRequest.h>
#import <UserNotifications/UNNotificationSound.h>

@implementation NotificationUtils

+ (void) sendNotificationNormal:(int)notificationId : (NSString*)name : (NSString*)title : (NSString*)message : (NSString*)urlIcon : (double)periodTime : (BOOL) allowSound
{
    // Content notification
    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
    content.title = title;
    content.body = message;
    if (allowSound)
    {
        content.sound = [UNNotificationSound defaultSound];
    }
    content.userInfo = @{
        FIELD_DATA_ID : [@(notificationId) stringValue],
        FIELD_DATA_NAME : name
    };
    NSTimeInterval interval = periodTime;
    NSDate* dateTime_utc = [NSDate dateWithTimeIntervalSince1970:interval];
    // Get Date Components
    NSCalendar* calendar = [NSCalendar currentCalendar];
    NSDateComponents* date = [calendar components:( NSCalendarUnitYear |
                                                    NSCalendarUnitMonth |
                                                    NSCalendarUnitDay |
                                                    NSCalendarUnitHour |
                                                    NSCalendarUnitMinute |
                                                    NSCalendarUnitSecond) fromDate:dateTime_utc];
    
        NSLog(@"---------- Notification ID = %d ----------------- ", notificationId);
        NSLog(@"toDateComponents year = %d", (int)[date year]);
        NSLog(@"toDateComponents mon  = %d", (int)[date month]);
        NSLog(@"toDateComponents day  = %d", (int)[date day]);
        NSLog(@"toDateComponents hour = %d", (int)[date hour]);
        NSLog(@"toDateComponents min  = %d", (int)[date minute]);
        NSLog(@"toDateComponents sec  = %d", (int)[date second]);
    
    UNCalendarNotificationTrigger* trigger = [UNCalendarNotificationTrigger triggerWithDateMatchingComponents:date repeats:NO];
    // Create the request object.
    UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier:@(notificationId).stringValue content:content trigger:trigger];
    // Scheduling Local Notifications for Delivery
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
        if (error != nil) {
            NSLog(@"%@", error.localizedDescription);
        }
    }];
}

+ (void) cancelAlarm:(int)notificationId
{
    UNUserNotificationCenter* userNotificationCenter = [UNUserNotificationCenter currentNotificationCenter];
    [userNotificationCenter removePendingNotificationRequestsWithIdentifiers:@[@(notificationId).stringValue]];
}

+ (void) cancelAllAlarms
{
    UNUserNotificationCenter* userNotificationCenter = [UNUserNotificationCenter currentNotificationCenter];
    [userNotificationCenter removeAllPendingNotificationRequests];
}

+ (void) cleanAllNotifications
{
    UNUserNotificationCenter* userNotificationCenter = [UNUserNotificationCenter currentNotificationCenter];
    [userNotificationCenter removeAllDeliveredNotifications];
}

@end
