#include "DeviceInfo.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "DeviceInfoIOS.h"

using namespace std;
USING_NS_CC;

void DeviceInfo::obtainDeviceInfo()
{
    std::string deviceLanguage        = IOS_getDeviceLanguage();
    string      deviceLanguageCountry = IOS_getDeviceLanguageCountry();
    NSLog(@"Device Language: %s", deviceLanguage.c_str());
    NSLog(@"Device Country: %s", deviceLanguageCountry.c_str());
    setDeviceLanguage(deviceLanguage);
    setDeviceLanguageCountry(deviceLanguageCountry);
}

std::string DeviceInfo::IOS_getDeviceLanguage()
{
    NSString* language = [DeviceInfoIOS getDeviceLanguage];
    return [language UTF8String];
}

std::string DeviceInfo::IOS_getDeviceLanguageCountry()
{
    NSString* country = [DeviceInfoIOS getDeviceLanguageCountry];
    return [country UTF8String];
}

#endif
