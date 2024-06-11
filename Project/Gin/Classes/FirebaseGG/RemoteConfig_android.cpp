#include "RemoteConfig.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "jni/JniHelper.h"
#include <jni.h>
#include "Define/GameDefine.h"

using namespace std;
USING_NS_CC;

std::string RemoteConfig::getString(const std::string& key, const std::string& defaultValue)
{
    std::string   ret = defaultValue;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, FIREBASE_REMOTE_CONFIG_CLASS_NAME, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring stringArg1 = t.env->NewStringUTF(key.c_str());
        jstring stringArg2 = t.env->NewStringUTF(defaultValue.c_str());

        jstring str = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID, stringArg1, stringArg2);
        ret = JniHelper::jstring2string(str);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

long RemoteConfig::getLong(const std::string& key, long defaultValue)
{
    long          ret = defaultValue;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, FIREBASE_REMOTE_CONFIG_CLASS_NAME, "getLong", "(Ljava/lang/String;J)J"))
    {
        jstring stringArg1 = t.env->NewStringUTF(key.c_str());

        ret = t.env->CallStaticLongMethod(t.classID, t.methodID, stringArg1, defaultValue);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

int RemoteConfig::getInteger(const std::string& key, int defaultValue)
{
    int           ret = defaultValue;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, FIREBASE_REMOTE_CONFIG_CLASS_NAME, "getInteger", "(Ljava/lang/String;I)I"))
    {
        jstring stringArg1 = t.env->NewStringUTF(key.c_str());

        ret = t.env->CallStaticIntMethod(t.classID, t.methodID, stringArg1, defaultValue);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

double RemoteConfig::getDouble(const std::string& key, double defaultValue)
{
    double        ret = defaultValue;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, FIREBASE_REMOTE_CONFIG_CLASS_NAME, "getDouble", "(Ljava/lang/String;D)D"))
    {
        jstring stringArg1 = t.env->NewStringUTF(key.c_str());

        ret = t.env->CallStaticDoubleMethod(t.classID, t.methodID, stringArg1, defaultValue);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

bool RemoteConfig::getBoolean(const std::string& key, bool defaultValue)
{
    bool          ret = defaultValue;
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, FIREBASE_REMOTE_CONFIG_CLASS_NAME, "getBoolean", "(Ljava/lang/String;Z)Z"))
    {
        jstring stringArg1 = t.env->NewStringUTF(key.c_str());

        ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1, defaultValue);
        // Delete unused data
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

#endif
