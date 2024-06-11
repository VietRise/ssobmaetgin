#ifndef __GAME_UTILS_H__
#define __GAME_UTILS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <algorithm>
#include <ctime>
#include <string>
#include "base/base64.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#define TIME_VIBRATION 1.0f

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#ifndef SYSOUT_F
#define SYSOUT_F(f, ...)      _RPT1( 0, f, __VA_ARGS__ ) // For Visual studio
#endif
#endif

enum MONEYUNIT
{
    None = 0,
    K    = 1000,
    M    = 1000000,
    B    = 1000000000
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#ifndef speedtest__
#define speedtest__(data)  for (long blockTime = NULL; (blockTime == NULL ? (blockTime = clock()) != NULL : false); SYSOUT_F(data "%f\n", (double) (clock() - blockTime) /*/ CLOCKS_PER_SEC*/))
//#define speedtest__(data) cocos2d::log("speedtest");
#endif

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define speedtest__(data) cocos2d::log("speedtest");
#endif

class GameUtils
{
private:
    static double DayDiff(std::time_t time1, std::time_t time2);
    static double HourDiff(std::time_t time1, std::time_t time2);
    static double MinuteDiff(std::time_t time1, std::time_t time2);
    static double SecondDiff(std::time_t time1, std::time_t time2);
    static bool isInSameDay(std::time_t& time1, std::time_t& time2);

    static std::time_t getLocalTimeFromString(const std::string& timeStr, std::string timeFormat = "%a %b %d %H:%M:%S %Y\n");
protected:
public:
    static std::string replaceString(std::string str, const std::string& tofind, const std::string& toreplace);
    static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
    static std::vector<std::string> split(const std::string& s, char delim);
    static std::string basename(const std::string& path);
    static long long getCurrentTime();

    static double randomDoubleInRange(double lowerBound, double upperBound);
    static double randomDoubleInList(const std::vector<double>& list);
    static int randomIntInRange(int lowerBound, int upperBound);
    static int randomIntInList(const std::vector<int>& list);
    static bool random50();
    static bool random20();
    static bool random10();

    static std::string MoneyFormat(double money, const char* prefix = "");
    static std::string MoneyFormatShortCut(double money, const char* prefix = "");
    static void vibration();
    static float getAngleForFacingAPoint(const cocos2d::Point& src, const cocos2d::Point& point);
    static cocos2d::Point getPositionOnCircle(int size, float order, float r, float d);
    static cocos2d::Point getCenterPositionOnCircleArc(const cocos2d::Point& p1, const cocos2d::Point& p2, float r, const cocos2d::Point& center);
    static void appendCubicBezier(int startPoint, std::vector<cocos2d::Vec2>& verts, const cocos2d::Vec2& from, const cocos2d::Vec2& control1, const cocos2d::Vec2& control2, const cocos2d::Vec2& to, uint32_t segments);

    static double unifyMoneyValue(double money);
    static std::string reverseString(const std::string& data);
    static cocos2d::Vec2 getPositionAccordingAngle(float angle, float r);

    static void log(const std::string& text, const cocos2d::Color3B& color);
    static void log(const std::string& text);
    static void clearLog();

    static int getIntegerForKeyUserDefault(const char* pKey);
    static int getIntegerForKeyUserDefault(const char* pKey, int defaultValue);
    static void setIntegerForKeyUserDefault(const char* pKey, int value);

    static std::string getStringForKeyUserDefault(const char* pKey);
    static std::string getStringForKeyUserDefault(const char* pKey, const std::string& defaultValue);
    static void setStringForKeyUserDefault(const char* pKey, const std::string& value);

    static double getDoubleForKeyUserDefault(const char* pKey);
    static double getDoubleForKeyUserDefault(const char* pKey, double defaultValue);
    static void setDoubleForKeyUserDefault(const char* pKey, double value);

    static void encodeUserDefault(const char* pKey, const std::string& value);
    static std::string decodeUserDefault(const char* pKey);
    static std::string decodeString(const std::string& val);

    static std::time_t getLocalTime();
    static bool isInSameDay(double timeDouble1, double timeDouble2);
    static double dayDiff(double timeDouble1, double timeDouble2);
    static long long getStartTimeOfDay(double currentTime);
    static long long getStartTimeOfNextDay(double currentTime);

    static std::vector<std::string> splitStringByDelim(std::string string, const std::string& delim);
    static long long getCurrentTimestamp();
    static std::string getTimeTextFromNumber(double number, std::string prefix = "", std::string subfix = "");

    static void writeStringToFileEncode(const std::string& fileName, const std::string& str);
    static std::string readStringFromFileDecode(const std::string& fileName, const std::string& defaultValue);

    static bool validateURL(const std::string& url);
    static std::string getImageSuitPath(int suit);
    static std::string getRankString(int rank);
    static std::string shortString(const std::string& str, int range);

    static void setLayoutTemplate(std::map<std::string, cocos2d::ui::Layout*>& map, const std::string& name, cocos2d::Node* node);
    static void releaseTemplate(std::map<std::string, cocos2d::ui::Layout*>& map);
    static cocos2d::ui::Layout* getLayoutTemplate(const std::map<std::string, cocos2d::ui::Layout*>& map, const std::string& name);

    static void setLayoutPosition(std::map<std::string, std::vector<cocos2d::Vec2>>& map, const std::string& name, cocos2d::Node* node);
    static const std::vector<cocos2d::Vec2>& getLayoutPosition(const std::map<std::string, std::vector<cocos2d::Vec2>>& map, const std::string& name);
    static std::string getHintDropPlaceTypeName(HintDropPlaceType type);
    static std::string getGameName();
    static std::string getResultTypeString(GinResultType type, bool win, bool challenge);
    static std::string getFightStatusString(FightStatus type);

    static void showLoading(cocos2d::Node* attachNode = nullptr);
    static void hideLoading();
    static void showAdsLoading(cocos2d::Node* attachNode, float delayTime, const std::function<void()>& callback);
    static void checkToRestoreSoundBackground();

    static cocos2d::Rect convertToWorldSpaceBoundingBox(cocos2d::Node* parent, cocos2d::Node* node);
    static cocos2d::Rect convertToNodeBoundingBox(cocos2d::Node* parent, const cocos2d::Rect& boundingBox);

    static void pauseGame();
    static void resumeGame();

    static void showRatingPopup(cocos2d::Node* parent, int state, const std::string& condition);
    static void showRatingAndReview(int state, const std::string& condition);

    static std::string getDeveloperName();

    // START: C++ Utilities
    template<typename K, typename V>
    static V getMapKey(const std::map<K, V>& m, const V& v, const K& def)
    {
        for (auto& it : m)
            if (it.second == v)
                return it.first;
        return def;
    }

    template<typename K, typename V>
    static V getMapValue(const std::map<K, V>& m, const K& k, const V& def)
    {
        typename std::map<K, V>::const_iterator it = m.find(k);
        return it != m.end() ? it->second : def;
    }

    template<typename E>
    static int getVecIndex(const std::vector<E>& v, const E& e, const int& def)
    {
        typename std::vector<E>::const_iterator it = std::find(v.begin(), v.end(), e);
        return (it != v.end() ? it - v.begin() : def);
    }

    template<typename E>
    static int getVecValue(const std::vector<E>& v, const int& index, const E& def)
    {
        return (index >= 0 && index < v.size() ? v.at(index) : def);
    }
    // END: C++ Utilities

    static std::string getMoneyRangeName(double money);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static jstring string2jString(JNIEnv* env, const std::string& str);
    static std::string jString2String(JNIEnv* env, jstring jstr);

    static std::list<std::string> jStringArray2StringList(JNIEnv* env, jobjectArray array);
    static jobjectArray jStringVector2StringArray(JNIEnv* env, const std::vector<std::string>& array);
    static std::vector<std::string> jStringArray2StringVector(JNIEnv* env, jobjectArray array);

    /* There is no 1-1 conversion between Android Bundle and cocos2d ValueMap
    * Bundle does NOT support put arbitrary object, specifically no such method Bundle.put(String key, Object value)
    * So when converting from ValueMap to Bundle, any ValueVector will be treated as vector of string
    * Assertion will fail if there is a ValueVector that contains another ValueVector or a ValueMap
    */
    static std::list<std::string> valueVector2StringList(const cocos2d::ValueVector& v);
#endif
};

#endif //__GAME_UTILS_H__