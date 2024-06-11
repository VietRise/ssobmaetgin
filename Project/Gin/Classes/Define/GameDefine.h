#ifndef __GAME_DEFINE_H__
#define __GAME_DEFINE_H__

#include "cocos2d.h"
#include <vector>

#define BUILD_RELEASE 1

#if BUILD_RELEASE //Release
    #define USE_DEBUG_VIEW 0
    #define DEBUG_LOG_RECEIVE_MESSAGE 0
    #define DEBUG_LOG_SEND_MESSAGE 0

    #define USE_CHEAT_CARD 0
    #define USE_CHEAT_MONEY 0

    #define STRESS_TEST 0
    #define USE_AUTO_PLAY_CARD 0

    #define USE_CHEAT_SHOW_RATING 0
#else //Debug
    #define USE_DEBUG_VIEW 0
    #define DEBUG_LOG_RECEIVE_MESSAGE 0
    #define DEBUG_LOG_SEND_MESSAGE 0

    #define USE_CHEAT_CARD 0
    #define USE_CHEAT_MONEY 0

    #define STRESS_TEST 0
    #define USE_AUTO_PLAY_CARD 0

    #define USE_CHEAT_SHOW_RATING 0
#endif

#define AVATAR_TAG 0

enum
{
    kMiddleground    = 100,
    kForeground      = 200,
    kAboveForeground = 300
};

enum CheatCardType
{
    CHEAT_CARD_TYPE_ADD        = 1,
    CHEAT_CARD_TYPE_REMOVE     = 2,
    CHEAT_CARD_TYPE_CLEAR      = 3,
    CHEAT_CARD_TYPE_FIRST_MOVE = 4
};

enum VideoAdsRequestSourceEnum
{
    UNKNOWN                   = 0,
    OLD_VIDEO_REWARD          = 1,
    VIDEO_REWARD_SKIP_WAITING = 2,
};

#define POPUP_BACKGROUND_COLOR cocos2d::Color4B(0, 0, 0, 175)
#define BUTTON_COLOR_DISABLE cocos2d::Color3B(150,150,150)
#define LOADING_ADS_DELAYTIME 1.0f

#define DATE_IN_MILISECOND 24*60*60*1000
#define HOUR_IN_MILISECOND 60*60*1000
#define MINUTE_IN_SECOND 60
#define MINUTE_IN_MILISECOND 60*1000

#define PI 3.14159265
#define ANGLE_PI 180.0f
#define SAFE_AREA_PADDING 45

#define RESOLUTION_1_6 1.6f
#define RESOLUTION_1_4 1.4f

#define STRINGIFY(name) (#name)

// START: D_GET_VAR
#define _D_CONCAT_VAR(a, b) StringHelper::joinArgsDelimiter(", ", a, b)
#define _D_GET_VAR_1(a1) LogHelper::getVar(#a1, a1)
#define _D_GET_VAR_2(a1, a2)                                     _D_CONCAT_VAR(_D_GET_VAR_1(a1),                                _D_GET_VAR_1(a2))
#define _D_GET_VAR_3(a1, a2, a3)                                 _D_CONCAT_VAR(_D_GET_VAR_2(a1, a2),                            _D_GET_VAR_1(a3))
#define _D_GET_VAR_4(a1, a2, a3, a4)                             _D_CONCAT_VAR(_D_GET_VAR_3(a1, a2, a3),                        _D_GET_VAR_1(a4))
#define _D_GET_VAR_5(a1, a2, a3, a4, a5)                         _D_CONCAT_VAR(_D_GET_VAR_4(a1, a2, a3, a4),                    _D_GET_VAR_1(a5))
#define _D_GET_VAR_6(a1, a2, a3, a4, a5, a6)                     _D_CONCAT_VAR(_D_GET_VAR_5(a1, a2, a3, a4, a5),                _D_GET_VAR_1(a6))
#define _D_GET_VAR_7(a1, a2, a3, a4, a5, a6, a7)                 _D_CONCAT_VAR(_D_GET_VAR_6(a1, a2, a3, a4, a5, a6),            _D_GET_VAR_1(a7))
#define _D_GET_VAR_8(a1, a2, a3, a4, a5, a6, a7, a8)             _D_CONCAT_VAR(_D_GET_VAR_7(a1, a2, a3, a4, a5, a6, a7),        _D_GET_VAR_1(a8))
#define _D_GET_VAR_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)         _D_CONCAT_VAR(_D_GET_VAR_8(a1, a2, a3, a4, a5, a6, a7, a8),    _D_GET_VAR_1(a9))
#define _D_GET_VAR_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)   _D_CONCAT_VAR(_D_GET_VAR_9(a1, a2, a3, a4, a5, a6, a7, a8, a9),_D_GET_VAR_1(a10))
#define _D_GET_VAR_MACROS(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, N, ...) N
#define D_GET_VAR(...) _D_GET_VAR_MACROS(__VA_ARGS__, _D_GET_VAR_10, _D_GET_VAR_9, _D_GET_VAR_8, _D_GET_VAR_7, _D_GET_VAR_6, _D_GET_VAR_5, _D_GET_VAR_4, _D_GET_VAR_3, _D_GET_VAR_2, _D_GET_VAR_1, UNUSED)(__VA_ARGS__)
// END: D_GET_VAR

extern int         s_winNumber;
extern int         s_loseNumber;
extern int         s_watchVideoCount;
extern int         s_startMatchCount;
extern int         s_totalWatchVideoCount;
extern int         s_countShowInterstitial;
extern int         s_showInterstitial;
extern std::string s_localNotficationData;
extern std::string s_versionName;
extern int         s_versionCode;
extern float       s_resolution_ratio;
extern bool        s_enableSafeArea;
extern bool        s_enableShowAds;
extern bool        s_enableGameService;

// NOTIFICATION EVENT
#define NOTIFY_USERDATA_CHANGE_MONEY 101
#define EVENT_USER_DONE 103
#define EVENT_CHANGE_LANGUAGE 107
#define EVENT_INTERSTITIAL_HIDE 108
#define EVENT_CHANGE_TABLE 110
#define EVENT_PLAYGAME_SIGNIN 111
#define EVENT_PLAYGAME_SIGNOUT 112
#define EVENT_SHOW_CUSTOM_APP_REVIEW 115
#define EVENT_RATING_REVIEW_COMPLETED 116

#define EVENT_KEY_SHOW_RATING_STATE "state"
#define EVENT_KEY_SHOW_RATING_CONDITION "condition"

// CLOSE INTERSTITIAL ADS ADMOB
#define INTERSTITIAL_ADMOB_CLOSE_ENDGAME "endgame"
#define INTERSTITIAL_ADMOB_GET_MONEY_DEFAULT "getmoneydefault"
#define INTERSTITIAL_ADMOB_GET_MONEY_BONUS "getmoneybonus"
#define INTERSTITIAL_ADMOB_GET_MONEY_TIME_BONUS "getmoneytimebonus"
#define INTERSTITIAL_ADMOB_EXIT_INGAME "exitingame"
#define INTERSTITIAL_ADMOB_EXIT_GAME "exitgame"
#define INTERSTITIAL_ADMOB_SCHEDULE 1*30 // 1 minutes
#define REWARD_VIDEO_ADMOB_SCHEDULE_NAME "schedule_fetch_video"
#define REWARD_VIDEO_ADMOB_SCHEDULE_TIME 5

#define LANGUAGE_TYPE_KEY "language_type"
#define AD_NAME_FUNCTION_KEY "name_function"
#define AD_CLOSED_KEY "closed"

#define VALUE_SHOW_RATING_AT_LOBBY 1
#define VALUE_SHOW_RATING_AT_GAMEPLAY 2

#define PRIVACY_POLICY_LINK_ANDROID "https://greenleafgame.com/privacy-policy.html"
#define PRIVACY_POLICY_LINK_IOS "https://idreamgs.com/privacy-policy.html"
#define SHARE_GAME_URL_JSON_DATA "{ \"data\": \"\"}"

//mailbox type
#define MAILBOX_TYPE_UPDATE "update"
#define MAILBOX_TYPE_READ "read"
#define MAILBOX_TYPE_REWARD "reward"
#define MAILBOX_TYPE_FUNCTION "function"
#define MAILBOX_TYPE_SHARE "share"

#define MAILBOX_FUNCTION_SHOP_TABLE "shop_table"
#define MAILBOX_FUNCTION_TIMEBONUS "time_bonus"
#define MAILBOX_FUNCTION_GAMECENTER "game_center"
#define MAILBOX_FUNCTION_PLAY "play"
#define MAILBOX_FUNCTION_QUICK_PLAY "quick_play"
#define MAILBOX_FUNCTION_LEADER_BOARD "leader_board"
#define MAILBOX_FUNCTION_ADS_KILLER "ads_killer"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    #define USE_DEBUG_VIEW 0
    #define DEBUG_LOG_RECEIVE_MESSAGE 1
    #define DEBUG_LOG_SEND_MESSAGE 1

    #define USE_CHEAT_CARD 1
    #define USE_CHEAT_MONEY 0

    #define STRESS_TEST 0
    #define USE_AUTO_PLAY_CARD 0
#endif

#if STRESS_TEST == 1
    #define USE_DEBUG_VIEW 0
    #define DEBUG_LOG_RECEIVE_MESSAGE 0
    #define DEBUG_LOG_SEND_MESSAGE 0
    #define USE_AUTO_PLAY_CARD 1
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    #define DEVICEINFO_CLASS_NAME "org/cocos2dx/cpp/DeviceInfo"
    #define FIREBASE_ANALYTICS_CLASS_NAME "org/cocos2dx/cpp/FirebaseAnalyticsController"
    #define FIREBASE_REMOTE_CONFIG_CLASS_NAME "org/cocos2dx/cpp/FirebaseRC"
    #define APP_ACTIVITY_CLASS_NAME "org/cocos2dx/cpp/AppActivity"
    #define PLAY_GAME_SERVICE_CLASS_NAME "org/cocos2dx/cpp/PlayGameService"
    #define STORE_SERVICE_CLASS_NAME "org/cocos2dx/cpp/StoreService"
    #define AD_MANAGER_CLASS_NAME "org/cocos2dx/cpp/Ads/AdManager"
#endif

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
    #define CCLOG(...)      do {} while (0)
    #define CCLOGINFO(...)  do {} while (0)
    #define CCLOGERROR(...) do {} while (0)
    #define CCLOGWARN(...)  do {} while (0)

    #define DLOG_FB(...)    do {} while (0)
#elif COCOS2D_DEBUG > 0

    #include "Helper/LogHelper.h"

    #define CCLOGINFO(s, ...)   LogHelper::info(LogHelper::Group::EMPTY, cocos2d::StringUtils::format(s, ##__VA_ARGS__));
    #define CCLOGWARN(s, ...)   LogHelper::warn(__FILE__, __FUNCTION__, __LINE__, cocos2d::StringUtils::format(s, ##__VA_ARGS__));
    #define CCLOGERROR(s, ...)  LogHelper::error(__FILE__, __FUNCTION__, __LINE__, cocos2d::StringUtils::format(s, ##__VA_ARGS__));

// [Start CCLOG]
    #define _CCLOG_1(s)         LogHelper::debug(LogHelper::Group::EMPTY, cocos2d::StringUtils::format("%s", s));
    #define _CCLOG_N(s, ...)    LogHelper::debugArgs(LogHelper::Group::EMPTY, StringHelper::EMPTY_STRING, StringHelper::EMPTY_STRING, s, ##__VA_ARGS__);
    #define _SELECT(PREFIX, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, SUFFIX, ...) PREFIX##_##SUFFIX
    #define CCLOG(...) _SELECT(_CCLOG,__VA_ARGS__,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,1)(__VA_ARGS__);
// [End CCLOG]

// [START: DLOG_FB]
    #define _DLOG_FB_(s) LogHelper::debug(LogHelper::Group::FIREBASE, s, __FILE_NAME__, __FUNCTION__);
    #define _DLOG_FB_0()                                            _DLOG_FB_("")
    #define _DLOG_FB_1(a1)                                          _DLOG_FB_(_D_GET_VAR_1(a1))
    #define _DLOG_FB_2(a1, a2)                                      _DLOG_FB_(_D_GET_VAR_2(a1, a2))
    #define _DLOG_FB_3(a1, a2, a3)                                  _DLOG_FB_(_D_GET_VAR_3(a1, a2, a3))
    #define _DLOG_FB_4(a1, a2, a3, a4)                              _DLOG_FB_(_D_GET_VAR_4(a1, a2, a3, a4))
    #define _DLOG_FB_5(a1, a2, a3, a4, a5)                          _DLOG_FB_(_D_GET_VAR_5(a1, a2, a3, a4, a5))
    #define _DLOG_FB_6(a1, a2, a3, a4, a5, a6)                      _DLOG_FB_(_D_GET_VAR_6(a1, a2, a3, a4, a5, a6))
    #define _DLOG_FB_7(a1, a2, a3, a4, a5, a6, a7)                  _DLOG_FB_(_D_GET_VAR_7(a1, a2, a3, a4, a5, a6, a7))
    #define _DLOG_FB_8(a1, a2, a3, a4, a5, a6, a7, a8)              _DLOG_FB_(_D_GET_VAR_8(a1, a2, a3, a4, a5, a6, a7, a8))
    #define _DLOG_FB_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)          _DLOG_FB_(_D_GET_VAR_9(a1, a2, a3, a4, a5, a6, a7, a8, a9))
    #define _DLOG_FB_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)    _DLOG_FB_(_D_GET_VAR_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))
    #define _DLOG_FB_MACROS(x, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, N, ...) N
    #define DLOG_FB(...) _DLOG_FB_MACROS(,##__VA_ARGS__, _DLOG_FB_10, _DLOG_FB_9, _DLOG_FB_8, _DLOG_FB_7, _DLOG_FB_6, _DLOG_FB_5, _DLOG_FB_4, _DLOG_FB_3, _DLOG_FB_2, _DLOG_FB_1, _DLOG_FB_0, UNUSED)(__VA_ARGS__)
// [END: DLOG_FB]
#endif

#endif //__GAME_DEFINE_H__
