#include "RemoteConfig.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "FirebaseRC.h"

std::string RemoteConfig::getString(const std::string& key, const std::string& defaultValue)
{
    return [[FirebaseRC getString:[NSString stringWithUTF8String:key.c_str()] :[NSString stringWithUTF8String:defaultValue.c_str()]] UTF8String];
}

long RemoteConfig::getLong(const std::string& key, long defaultValue)
{
    return [FirebaseRC getLong:[NSString stringWithUTF8String:key.c_str()] :defaultValue];
}

int RemoteConfig::getInteger(const std::string& key, int defaultValue)
{
    return [FirebaseRC getInteger:[NSString stringWithUTF8String:key.c_str()] :defaultValue];
}

double RemoteConfig::getDouble(const std::string& key, double defaultValue)
{
    return [FirebaseRC getDouble:[NSString stringWithUTF8String:key.c_str()] :defaultValue];
}

bool RemoteConfig::getBoolean(const std::string& key, bool defaultValue)
{
    return [FirebaseRC getBoolean:[NSString stringWithUTF8String:key.c_str()] :defaultValue];
}

#endif
