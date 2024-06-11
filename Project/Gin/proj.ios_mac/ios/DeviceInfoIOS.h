#ifndef DeviceInfoIOS_h
#define DeviceInfoIOS_h

#import <Foundation/Foundation.h>

@interface DeviceInfoIOS : NSObject {
}

+(NSString*) getDeviceLanguage;
+(NSString*) getDeviceLanguageCountry;

@end

#endif /* DeviceInfoIOS_h */
