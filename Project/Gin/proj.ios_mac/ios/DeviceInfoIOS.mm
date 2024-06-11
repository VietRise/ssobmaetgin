#import "DeviceInfoIOS.h"

@implementation DeviceInfoIOS

+(NSString*) getDeviceLanguage
{
    return [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];
}

+(NSString*) getDeviceLanguageCountry
{
    return [[NSLocale currentLocale] objectForKey: NSLocaleCountryCode];
}

@end
