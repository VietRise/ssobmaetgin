#ifndef NotificationUtils_h
#define NotificationUtils_h

#import <Foundation/Foundation.h>

#define FIELD_DATA_ID @"id"
#define FIELD_DATA_NAME @"name"
#define FIELD_DATA_TYPE @"type"
#define FIELD_DATA_TITLE @"title"
#define FIELD_DATA_MESSAGE @"message"
#define FIELD_DATA_IMAGE @"image"
#define FIELD_DATA_PERIOD_TIME @"period_time"
#define FIELD_DATA_DELAY_TIME @"delay_time"

#define NOTIFICATION_TYPE_NORMAL @"normal"
#define NOTIFICATION_TYPE_BANNER @"banner"

@interface NotificationUtils : NSObject {
    
}

+ (void) sendNotificationNormal:(int)notificationId : (NSString*)name : (NSString*)title : (NSString*)message : (NSString*)urlIcon : (double)periodTime : (BOOL) allowSound;
+ (void) cancelAlarm:(int)notificationId;
+ (void) cancelAllAlarms;
+ (void) cleanAllNotifications;
@end

#endif
