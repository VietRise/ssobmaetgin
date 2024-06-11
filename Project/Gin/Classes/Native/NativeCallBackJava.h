#ifndef __NATIVE_CALLBACK_JAVA_H__
#define __NATIVE_CALLBACK_JAVA_H__

#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

extern "C"
{
    //Ad Managers
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Ads_AdManager_interstitialHide(JNIEnv* env, jobject thiz, jstring jni_nameFunction, jboolean adClosed);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Ads_AdManager_watchVideoCompleted(JNIEnv* env, jobject thiz, jint source);

    //Play Games Service
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PlayGameService_onPlayGameServiceSignedIn(JNIEnv* env, jobject thiz);
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PlayGameService_onPlayGameServiceSignedOut(JNIEnv* env, jobject thiz);

    //Play Core Service
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_StoreService_completedAppReview(JNIEnv* env, jobject thiz);

};
#endif

#endif