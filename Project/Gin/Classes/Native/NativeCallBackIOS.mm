#include <stdio.h>
#include "NativeCallBackIOS.h"
#include "NativeCallBack.h"
#include "Manager/SoundManager.h"
#include "Helper/GameUtils.h"

void NativeCallBackIOS::watchVideoCompleted(VideoAdsRequestSourceEnum source)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->watchVideoCompleted(source);
        }, scheduler, 0, 0, 0.5f, false, "watchVideoCompleted");
    });
}

void NativeCallBackIOS::interstitialHide(std::string nameFunction, bool adClosed)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->interstitialHide(nameFunction, adClosed);
        }, scheduler, 0, 0, 0.5f, false, "interstitialHide");
    });
}

void NativeCallBackIOS::onPlayGameServiceSignedIn()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->onPlayGameServiceSignedIn();
        }, scheduler, 0, 0, 0.5f, false, "onPlayGameServiceSignedIn");
    });
}

void NativeCallBackIOS::onPlayGameServiceSignedOut()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->onPlayGameServiceSignedOut();
        }, scheduler, 0, 0, 0.5f, false, "onPlayGameServiceSignedOut");
    });
}

void NativeCallBackIOS::pauseGame()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        GameUtils::pauseGame();
    });
}

void NativeCallBackIOS::resumeGame()
{
    GameUtils::resumeGame();
}

void NativeCallBackIOS::completedAppReview()
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=]() {
        scheduler->schedule([=](float dt) {
            NativeCallBack::getInstance()->completedAppReview();
        }, scheduler, 0, 0, 0.1f, false, "completedAppReview");
    });
}