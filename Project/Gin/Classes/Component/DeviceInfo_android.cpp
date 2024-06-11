#include "DeviceInfo.h"

using namespace std;
USING_NS_CC;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "Define/GameDefine.h"
#include "jni/JniHelper.h"

void DeviceInfo::obtainDeviceInfo()
{
    
    setDeviceLanguage(JNI_getDeviceLanguage());
    setDeviceLanguageCountry(JNI_getDeviceLanguageCountry());
}

extern "C"
{
    std::string JNI_getDeviceLanguage()
    {
        JniMethodInfo t;
        std::string ret("");

        if (JniHelper::getStaticMethodInfo(t, DEVICEINFO_CLASS_NAME, "getDeviceLanguage", "()Ljava/lang/String;")) {
            jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            ret = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        }
        return ret;
    }

    std::string JNI_getDeviceLanguageCountry()
    {
        JniMethodInfo t;
        std::string ret("");

        if (JniHelper::getStaticMethodInfo(t, DEVICEINFO_CLASS_NAME, "getDeviceLanguageCountry", "()Ljava/lang/String;")) {
            jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
            ret = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        }
        return ret;
    }
}
#endif