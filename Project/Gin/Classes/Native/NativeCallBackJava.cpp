#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "NativeCallBackJava.h"
#include "NativeCallBack.h"
#include "AdapterClientServer/ClientManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Manager/NotificationManager.h"

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Ads_AdManager_interstitialHide(JNIEnv* env, jobject thiz, jstring jni_nameFunction, jboolean adClosed)
{
    const char *nameFunction = env->GetStringUTFChars(jni_nameFunction, NULL);
    std::string ret(nameFunction);
    env->ReleaseStringUTFChars(jni_nameFunction, nameFunction);
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->interstitialHide(ret, adClosed);
            }, scheduler, 0, 0, 0.1f, false, "interstitialHide");
    });
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PlayGameService_onPlayGameServiceSignedIn(JNIEnv* env, jobject thiz)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->onPlayGameServiceSignedIn();
            }, scheduler, 0, 0, 0.5f, false, "onPlayGameServiceSignedIn");
    });
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PlayGameService_onPlayGameServiceSignedOut(JNIEnv* env, jobject thiz)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->onPlayGameServiceSignedOut();
            }, scheduler, 0, 0, 0.5f, false, "onPlayGameServiceSignedOut");
    });
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Ads_AdManager_watchVideoCompleted(JNIEnv* env, jobject thiz, jint source)
{
    VideoAdsRequestSourceEnum mSource = static_cast<VideoAdsRequestSourceEnum>(source);
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->watchVideoCompleted(mSource);
            }, scheduler, 0, 0, 0.1f, false, "watchVideoCompleted");
    });
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_StoreService_completedAppReview(JNIEnv* env, jobject thiz)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->completedAppReview();
            }, scheduler, 0, 0, 0.1f, false, "completedAppReview");
    });
}

#endif