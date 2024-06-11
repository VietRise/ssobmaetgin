#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__

#include "cocos2d.h"
#include <string>

class DeviceInfo
{
private:
    std::string _deviceLanguage;
    std::string _deviceLanguageCountry;
protected:
public:
    DeviceInfo();
    ~DeviceInfo();

    void setDeviceLanguage(const std::string& language);
    void setDeviceLanguageCountry(const std::string& country);

    const std::string& getDeviceLanguage();
    const std::string& getDeviceLanguageCountry();

    void obtainDeviceInfo();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    std::string IOS_getDeviceLanguage();
    std::string IOS_getDeviceLanguageCountry();
#endif
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
extern "C"
{
    extern std::string JNI_getDeviceLanguage();
    extern std::string JNI_getDeviceLanguageCountry();
}
#endif

#endif //__DEVICE_INFO_H__