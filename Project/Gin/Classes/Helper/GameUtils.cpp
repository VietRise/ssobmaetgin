#include "GameUtils.h"
#include "Scenes/DebugLayer.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "UserData/UserData.h"
#include "Define/TextDefine.h"
#include "Scenes/Gin/GPGinSlotManager.h"
#include "Native/CallToOtherLanguage.h"
#include "Helper/fmt/format.h"
#include "Scenes/LoadingLayer.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Manager/SoundManager.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/BoardManager.h"
#include "Boards/RatingPopup.h"
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <regex>

USING_NS_CC;
using namespace ui;
using namespace std;

#define MAX_MONEY USER_DEFAULT_MAXIMUM_MONEY

std::string GameUtils::replaceString(std::string str, const std::string& tofind, const std::string& toreplace)
{
    size_t position = 0;
    for (position = str.find(tofind); position != std::string::npos; position = str.find(tofind, position))
        str.replace(position, 1, toreplace);
    return str;
}

std::vector<std::string>& GameUtils::split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string       item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

std::vector<std::string> GameUtils::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string GameUtils::basename(const std::string& path)
{
    if (path.rfind("/\\") != std::string::npos)
        return path.substr(path.rfind("/\\") + 1);
    if (path.rfind("/") != std::string::npos)
        return path.substr(path.rfind("/") + 1);
    return path;
}

long long GameUtils::getCurrentTime()
{
    return (long long) (GameUtils::getCurrentTimestamp() / 1000);
}

double GameUtils::randomDoubleInRange(double lowerBound, double upperBound)
{
    return cocos2d::RandomHelper::random_real(lowerBound, upperBound);
}

double GameUtils::randomDoubleInList(const vector<double>& list)
{
    return list.at(randomIntInRange(0, list.size() - 1));
}

int GameUtils::randomIntInRange(int lowerBound, int upperBound)
{
    return cocos2d::RandomHelper::random_int(lowerBound, upperBound);
}

int GameUtils::randomIntInList(const vector<int>& list)
{
    return list.at(randomIntInRange(0, list.size() - 1));
}

bool GameUtils::random50()
{
    return randomDoubleInRange(0.0f, 1.0f) <= 0.5f;
}

bool GameUtils::random20()
{
    return randomDoubleInRange(0.0f, 1.0f) <= 0.2f;
}

bool GameUtils::random10()
{
    return randomDoubleInRange(0.0f, 1.0f) <= 0.1f;
}

std::string GameUtils::MoneyFormat(double money, const char* prefix/* = NULL*/)
{
    string    moneyString;
    if (money == 0)
    {
        string prefixStr = prefix;
        moneyString = prefixStr + "0";
        return moneyString;
    }
    long long startMoney = 1000000000000;
    if (money >= startMoney)
    {
        startMoney          = 1000000000;
        moneyString.append(prefix);
        long long longMoney = money;
        int       rem       = 10000;
        long long billion   = (longMoney / startMoney);
        long long million   = (longMoney % startMoney) / rem;
        moneyString.append(StringUtils::format("%d", billion));
        if (million > 0)
        {
            string temStr = StringUtils::format("%d", million);
            int    size   = StringUtils::format("%d", rem).size();
            while (temStr.size() < size)
                temStr.insert(0, "0");
            for (auto iter = temStr.end() - 1; iter != temStr.begin(); iter--)
            {
                if (*iter != '0')
                    break;
                else
                    iter = temStr.erase(iter);
            }
            moneyString.append(NUMBER_SEPERATE);
            moneyString.append(temStr);
        }
        moneyString.append(" B");
        return moneyString;
    }

    bool isNegative = money < 0;
    moneyString = "";
    int split = 0;
    money = abs(money);
    long long newMoney = money;
    while (newMoney > 0)
    {
        int n = newMoney % 10;
        newMoney /= 10;
        moneyString.append(s_strNum[n]);
        split++;
        if (split == 3 && newMoney > 0)
        {
            moneyString.append(s_strNum[10]);
            split = 0;
        }
    }
    if (isNegative)
        moneyString.append("-");
    if (prefix)
        moneyString.append(prefix);
    std::reverse(moneyString.begin(), moneyString.end());
    return moneyString;
}

std::string GameUtils::MoneyFormatShortCut(double money, const char* prefix)
{
    bool   haveSpace     = false;
    string moneyString;
    if (money == 0)
    {
        string prefixStr = prefix;
        moneyString = prefixStr + "0";
        return moneyString;
    }
    int    arroundNumber = 2;
    bool   isNegative    = money < 0;
    moneyString = "";
    int split = 0;
    money = abs(money);
    long long newMoney = money;
    long long newMoney1, newMoney2;
    // Get type money
    MONEYUNIT typeMoney;
    if (newMoney >= MONEYUNIT::B)
        typeMoney = MONEYUNIT::B;
    else if (newMoney >= MONEYUNIT::M)
        typeMoney = MONEYUNIT::M;
    else if (newMoney >= MONEYUNIT::K)
        typeMoney = MONEYUNIT::K;
    else
        typeMoney = MONEYUNIT::None;
    // Append type money to string
    switch (typeMoney)
    {
        case MONEYUNIT::None:break;
        case MONEYUNIT::K:
            if (haveSpace)
                moneyString.append("K ");
            else
                moneyString.append("K");
            break;
        case MONEYUNIT::M:
            if (haveSpace)
                moneyString.append("M ");
            else
                moneyString.append("M");
            break;
        case MONEYUNIT::B:
            if (haveSpace)
                moneyString.append("B ");
            else
                moneyString.append("B");
            break;
        default:break;
    }
    // Append number after ","
    if (typeMoney != MONEYUNIT::None)
    {
        newMoney2 = newMoney % (int) typeMoney;
        if (newMoney2 != 0)
        {
            newMoney2 = newMoney2 / ((int) typeMoney / pow(10, arroundNumber + 1));
            int tem = newMoney2 % 10;
            newMoney2 = (tem >= 5) ? newMoney2 / (int) pow(10, 2) + 1 : newMoney2 / (int) pow(10, 2);
            if (newMoney2 > 0)
            {
                while (newMoney2 > 0)
                {
                    int n = newMoney2 % 10;
                    newMoney2 /= 10;
                    moneyString.append(s_strNum[n]);
                }
                moneyString.append(NUMBER_SEPERATE);
            }
        }
    }
    // Append number before ","
    if (typeMoney == MONEYUNIT::None)
        newMoney1 = newMoney / 1;
    else
        newMoney1 = newMoney / (int) typeMoney;
    while (newMoney1 > 0)
    {
        int n = newMoney1 % 10;
        newMoney1 /= 10;
        moneyString.append(s_strNum[n]);
        split++;
        if (split == 3 && newMoney1 > 0)
        {
            moneyString.append(s_strNum[10]);
            split = 0;
        }
    }
    if (isNegative)
        moneyString.append("-");
    if (prefix)
        moneyString.append(prefix);
    std::reverse(moneyString.begin(), moneyString.end());
    return moneyString;
}

void GameUtils::vibration()
{
    if (UserData::getInstance()->getVibration())
    {
        Device::vibrate(TIME_VIBRATION);
        CCLOG("vibrate");
    }
}

float GameUtils::getAngleForFacingAPoint(const cocos2d::Point& src, const cocos2d::Point& point)
{
    Point pos2            = point;
    Point pos1            = src;
    float theta           = atan((pos1.y - pos2.y) / (pos1.x - pos2.x)) * 180 * 7 / 22;
    float calculatedAngle = 0;
    if (pos1.y - pos2.y > 0)
    {
        if (pos1.x - pos2.x < 0)
            calculatedAngle = (-90 - theta);
        else if (pos1.x - pos2.x > 0)
            calculatedAngle = (90 - theta);
    }
    else if (pos1.y - pos2.y < 0)
    {
        if (pos1.x - pos2.x < 0)
            calculatedAngle = (270 - theta);
        else if (pos1.x - pos2.x > 0)
            calculatedAngle = (90 - theta);
    }
    return calculatedAngle; //in degree
}

Point GameUtils::getPositionOnCircle(int size, float order, float r, float d)
{
    float ed = 0;
    if (size >= 2)
    {
        ed           = d / (size - 1);
        float angley = order * ed + (180 - d) / 2;
        float anglex = 90 - angley;
        float y      = sin(CC_DEGREES_TO_RADIANS(angley)) * r;
        float x      = sin(CC_DEGREES_TO_RADIANS(anglex)) * r;
        return Point(x, y - r);
    }
    else
    {
        d = 0;
        float angley = order * ed + (180 - d) / 2;
        float anglex = 90 - angley;
        float y      = sin(CC_DEGREES_TO_RADIANS(angley)) * r;
        float x      = sin(CC_DEGREES_TO_RADIANS(anglex)) * r;
        return Point(x, y - r);
    }
    return Point(0, 0);
}

Point GameUtils::getCenterPositionOnCircleArc(const cocos2d::Point& p1, const cocos2d::Point& p2, float r, const cocos2d::Point& center)
{
    Vec2  vec1    = p1 - center;
    Vec2  vec2    = p2 - center;
    float radians = vec1.getAngle(vec2) / 2;
    Vec2  vect    = Point(1, 0) - Point(0, 0);
    float rP1     = vect.getAngle(vec1);
    float rP2     = vect.getAngle(vec2);
    float rCen    = 0;
    rCen = radians + rP1;
    float angley = CC_RADIANS_TO_DEGREES(rCen);
    float anglex = 90 - angley;
    float y      = sin(CC_DEGREES_TO_RADIANS(angley)) * r;
    float x      = sin(CC_DEGREES_TO_RADIANS(anglex)) * r;
    return Point(x, y - r);
}

void GameUtils::appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const cocos2d::Vec2& from, const cocos2d::Vec2& control1, const cocos2d::Vec2& control2, const cocos2d::Vec2& to, uint32_t segments)
{
    float    t = 0;
    for (int i = 0; i < segments; i++)
    {
        float x = powf(1 - t, 3) * from.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * to.x;
        float y = powf(1 - t, 3) * from.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * to.y;
        verts[startPoint + i] = Vec2(x, y);
        t += 1.0f / segments;
    }
}

void GameUtils::log(const std::string& text)
{
#if USE_DEBUG_VIEW == 1
    DebugLayer::getInstance()->addText(text, cocos2d::Color3B::WHITE);
#endif
}

void GameUtils::log(const std::string& text, const cocos2d::Color3B& color)
{
#if USE_DEBUG_VIEW == 1
    DebugLayer::getInstance()->addText(text, color);
#endif
}

void GameUtils::clearLog()
{
#if USE_DEBUG_VIEW == 1
    DebugLayer::getInstance()->onClearClink(nullptr);
#endif
}

double GameUtils::unifyMoneyValue(double money)
{
    return money <= 0 ? 0 : money > MAX_MONEY ? MAX_MONEY : money;
}

std::string GameUtils::reverseString(const std::string& data)
{
    string   newString;
    for (int n = data.length() - 1; n >= 0; n--)
        newString.push_back(data[n]);
    return newString;
}

// --------------------- CARD --------------------------------
cocos2d::Vec2 GameUtils::getPositionAccordingAngle(float angle, float r)
{
    float x = sin(angle * PI / ANGLE_PI) * r;
    float y = cos(angle * PI / ANGLE_PI) * r;
    return Vec2(x, y);
}

int GameUtils::getIntegerForKeyUserDefault(const char* pKey)
{
    string valueString = GameUtils::decodeUserDefault(pKey);
    int    value       = atoi(valueString.c_str());
    return value;
}

int GameUtils::getIntegerForKeyUserDefault(const char* pKey, int defaultValue)
{
    string valueString = GameUtils::decodeUserDefault(pKey);
    if (valueString != "")
        return atoi(valueString.c_str());
    return defaultValue;
}

void GameUtils::setIntegerForKeyUserDefault(const char* pKey, int value)
{
    char szName[50] = { 0 };
    sprintf(szName, "%i", value);
    string valueString = string(szName);
    GameUtils::encodeUserDefault(pKey, valueString);
}

double GameUtils::getDoubleForKeyUserDefault(const char* pKey)
{
    string valueString = GameUtils::decodeUserDefault(pKey);
    double value       = atof(valueString.c_str());
    return value;
}

double GameUtils::getDoubleForKeyUserDefault(const char* pKey, double defaultValue)
{
    string valueString = GameUtils::decodeUserDefault(pKey);
    if (valueString != "")
        return atof(valueString.c_str());
    return defaultValue;
}

void GameUtils::setDoubleForKeyUserDefault(const char* pKey, double value)
{
    char szName[50] = { 0 };
    sprintf(szName, "%.0lf", value);
    string valueString = string(szName);
    GameUtils::encodeUserDefault(pKey, valueString);
}

std::string GameUtils::getStringForKeyUserDefault(const char* pKey)
{
    return GameUtils::decodeUserDefault(pKey);
}

std::string GameUtils::getStringForKeyUserDefault(const char* pKey, const string& defaultValue)
{
    string value = GameUtils::decodeUserDefault(pKey);
    if (value != "")
        return value;
    return defaultValue;
}

void GameUtils::setStringForKeyUserDefault(const char* pKey, const string& value)
{
    GameUtils::encodeUserDefault(pKey, value);
}

void GameUtils::encodeUserDefault(const char* pKey, const std::string& value)
{
    char* valueEncode;
    base64Encode(reinterpret_cast<const unsigned char*>(value.c_str()), strlen(value.c_str()) + 1, &valueEncode);
    UserDefault::getInstance()->setStringForKey(pKey, valueEncode);
}

string GameUtils::decodeUserDefault(const char* pKey)
{
    string valueEncode = UserDefault::getInstance()->getStringForKey(pKey);
    if (!valueEncode.empty())
    {
        unsigned char* value;
        base64Decode(reinterpret_cast<const unsigned char*>(valueEncode.c_str()), strlen(valueEncode.c_str()) + 1, &value);
        return string(reinterpret_cast<char*>(value));
    }
    return valueEncode;
}

std::string GameUtils::decodeString(const std::string& val)
{
    unsigned char* value;
    base64Decode(reinterpret_cast<const unsigned char*>(val.c_str()), (unsigned int) (strlen(val.c_str()) + 1), &value);
    return std::string(reinterpret_cast<char*>(value));
}

std::time_t GameUtils::getLocalTime()
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return mktime(timeinfo);
}

std::time_t GameUtils::getLocalTimeFromString(const std::string& timeStr, std::string timeFormat)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    std::tm            timeinfo;
    std::istringstream stream(timeStr.c_str());
    stream >> std::get_time(&timeinfo, timeFormat.c_str());
    return mktime(&timeinfo);
#else
    double num = getCurrentTime();
    struct tm tm;
    strptime((char*)timeStr.c_str(), (char*)timeFormat.c_str(), &tm);
    time_t t = mktime(&tm);
    return t;
#endif
}

double GameUtils::DayDiff(std::time_t time1, std::time_t time2)
{
    double difference = std::difftime(time2, time1) / (60 * 60 * 24);
    return difference;
}

double GameUtils::HourDiff(std::time_t time1, std::time_t time2)
{
    double difference = std::difftime(time2, time1) / (60 * 60);
    return difference;
}

double GameUtils::MinuteDiff(std::time_t time1, std::time_t time2)
{
    double difference = std::difftime(time2, time1) / (60);
    return difference;
}

double GameUtils::SecondDiff(std::time_t time1, std::time_t time2)
{
    double difference = std::difftime(time2, time1);
    return difference;
}

bool GameUtils::isInSameDay(std::time_t& time1, std::time_t& time2)
{
    tm localTime1 = *localtime(&time1);
    tm localTime2 = *localtime(&time2);
    if (localTime1.tm_year != localTime2.tm_year)
        return false;
    if (localTime1.tm_yday != localTime2.tm_yday)
        return false;
    return true;
}

bool GameUtils::isInSameDay(double timeDouble1, double timeDouble2)
{
    time_t time1 = (time_t) timeDouble1;
    time_t time2 = (time_t) timeDouble2;
    return isInSameDay(time1, time2);
}

std::vector<std::string> GameUtils::splitStringByDelim(std::string string, const std::string& delim)
{
    std::vector<std::string> result;
    size_t                   pos = 0;
    std::string              token;
    while ((pos = string.find(delim)) != std::string::npos)
    {
        token = string.substr(0, pos);
        result.push_back(token);
        string.erase(0, pos + delim.length());
    }
    result.push_back(string);
    return result;
}

double GameUtils::dayDiff(double timeDouble1, double timeDouble2)
{
    time_t time1      = (time_t) timeDouble1;
    time_t time2      = (time_t) timeDouble2;
    tm     localTime1 = *localtime(&time1);
    tm     localTime2 = *localtime(&time2);
    return (localTime1.tm_year - localTime2.tm_year) * 365 + (localTime1.tm_yday - localTime2.tm_yday);
}

long long GameUtils::getStartTimeOfDay(double currentTime)
{
    time_t currentTimeT = (time_t) currentTime;
    tm     localTime    = *localtime(&currentTimeT);
    return currentTime - localTime.tm_hour * 60 * 60 - localTime.tm_min * 60 - localTime.tm_sec;
}

long long GameUtils::getStartTimeOfNextDay(double currentTime)
{
    long long startTimeOfNextDay = getStartTimeOfDay(currentTime + 86400);
    return startTimeOfNextDay;
}

std::string GameUtils::getTimeTextFromNumber(double number, std::string prefix, std::string subfix)
{
    int hour = number / 3600;
    number -= hour * 3600;
    int minute = number / 60;
    number -= minute * 60;

    int         second = number;
    std::string text   = StringUtils::format("%s%02d:%02d:%02d%s", prefix.c_str(), hour, minute, second, subfix.c_str());
    return text;
}

long long GameUtils::getCurrentTimestamp()
{
    return (long long) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void GameUtils::writeStringToFileEncode(const std::string& fileName, const std::string& str)
{
    if (fileName != "")
    {
        char* valueEncode;
        base64Encode(reinterpret_cast<const unsigned char*>(str.c_str()), strlen(str.c_str()) + 1, &valueEncode);
        std::string fullpath = FileUtils::getInstance()->getWritablePath() + string("/") + fileName;
        FileUtils::getInstance()->writeStringToFile(valueEncode, fullpath);
    }
}

std::string GameUtils::readStringFromFileDecode(const std::string& fileName, const std::string& defaultValue)
{
    if (fileName != "")
    {
        std::string fullpath = FileUtils::getInstance()->getWritablePath() + string("/") + fileName;
        if (FileUtils::getInstance()->isFileExist(fullpath))
        {
            std::string valueEncode = FileUtils::getInstance()->getStringFromFile(fullpath);
            if (valueEncode != "")
            {
                unsigned char* value;
                base64Decode(reinterpret_cast<const unsigned char*>(valueEncode.c_str()), strlen(valueEncode.c_str()) + 1, &value);
                if (value != nullptr)
                    return string(reinterpret_cast<char*>(value));
            }
        }
    }
    return defaultValue;
}

bool GameUtils::validateURL(const std::string& url)
{
    std::regex pattern("((\\w+:\\/\\/)[-a-zA-Z0-9:@;?&=\\/%\\+\\.\\*!'\\(\\),\\$_\\{\\}\\^~\\[\\]`#|]+)");
    if (std::regex_match(url, pattern))
        return true;
    return false;
}

std::string GameUtils::getImageSuitPath(int suit)
{
    switch (suit)
    {
        case (int) Suit::SUIT_HEARTS:return "plist/ImageCard/chat_co_small.png";
        case (int) Suit::SUIT_DIAMONDS:return "plist/ImageCard/chat_ro_small.png";
        case (int) Suit::SUIT_CLUBS:return "plist/ImageCard/chat_chuon_small.png";
        case (int) Suit::SUIT_SPADES:return "plist/ImageCard/chat_bich_small.png";
        default:return "plist/ImageCard/chat_co_small.png";
    }
}

std::string GameUtils::getRankString(int rank)
{
    switch (rank)
    {
        case (int) Rank::RANK_A:return " A";
        case (int) Rank::RANK_2:return " 2";
        case (int) Rank::RANK_3:return " 3";
        case (int) Rank::RANK_4:return " 4";
        case (int) Rank::RANK_5:return " 5";
        case (int) Rank::RANK_6:return " 6";
        case (int) Rank::RANK_7:return " 7";
        case (int) Rank::RANK_8:return " 8";
        case (int) Rank::RANK_9:return " 9";
        case (int) Rank::RANK_10:return "10";
        case (int) Rank::RANK_J:return " J";
        case (int) Rank::RANK_Q:return " Q";
        case (int) Rank::RANK_K:return " K";
        default:return "ERROR";
    }
}

std::string GameUtils::shortString(const std::string& str, int range)
{
    if (str.length() > range)
    {
        std::string subStr = str.substr(0, range);
        subStr += "...";
        return subStr;
    }
    return str;
}

std::string GameUtils::getGameName()
{
    return "Gin Rummy Offline";
}

std::string GameUtils::getResultTypeString(GinResultType type, bool win, bool challenge)
{
    switch (type)
    {
        case GinResultType::GIN_RESULT_TYPE_NONE:return "None";
        case GinResultType::GIN_RESULT_TYPE_SCORE:
        {
            if (win)
            {
                return "win_score";
            }
            else
            {
                return "lose_score";
            }
        }
        case GinResultType::GIN_RESULT_TYPE_WIN_KNOCK:return "win_knock";
        case GinResultType::GIN_RESULT_TYPE_WIN_GIN:return "win_gin";
        case GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN:return "win_big_gin";
        case GinResultType::GIN_RESULT_TYPE_WIN_UNDERCUT:return "win_undercut";
        case GinResultType::GIN_RESULT_TYPE_LOSE_KNOCK:return "lose_knock";
        case GinResultType::GIN_RESULT_TYPE_LOSE_GIN:return "lose_gin";
        case GinResultType::GIN_RESULT_TYPE_LOSE_BIG_GIN:return "lose_big_gin";
        case GinResultType::GIN_RESULT_TYPE_LOSE_UNDERCUT:return "lose_undercut";
        default:return "None";
    }
}

std::string GameUtils::getFightStatusString(FightStatus type)
{
    switch (type)
    {
        case FightStatus::NOT_ALLOW:return "NotAllow";
        case FightStatus::ALLOW:return "Allow";
        case FightStatus::WAIT_CONFIRM:return "WaitConfirm";
        case FightStatus::UNDERCUT:return "Undercut";
        case FightStatus::KNOCK:return "Knock";
        case FightStatus::PASS:return "Pass";
        default:return "Unknow";
    }
}

void GameUtils::setLayoutTemplate(std::map<std::string, cocos2d::ui::Layout*>& map, const std::string& name, cocos2d::Node* node)
{
    map[name] = nullptr;
    auto temp = static_cast<cocos2d::ui::Layout*>(node->getChildByName(name));
    if (temp)
    {
        temp->removeFromParent();
        temp->retain();
        map[name] = temp;
    }
}

void GameUtils::setLayoutPosition(std::map<std::string, std::vector<cocos2d::Vec2>>& map, const std::string& name, cocos2d::Node* node)
{
    auto it = map.find(name);
    if (it == map.end())
        map[name] = std::vector<cocos2d::Vec2>();

    auto child = node->getChildByName(name);
    if (child)
        map[name].push_back(node->convertToWorldSpace(child->getPosition()));
    else
        map[name].push_back(cocos2d::Vec2(0, 0));
}

void GameUtils::releaseTemplate(std::map<std::string, cocos2d::ui::Layout*>& map)
{
    std::map<std::string, Layout*>::iterator it;
    for (it = map.begin(); it != map.end(); it++)
        CC_SAFE_RELEASE_NULL(it->second);
    map.clear();
}

const std::vector<cocos2d::Vec2>& GameUtils::getLayoutPosition(const std::map<std::string, std::vector<cocos2d::Vec2>>& map, const std::string& name)
{
    if (map.find(name) != map.end())
        return map.at(name);
    static std::vector<cocos2d::Vec2> defaultVectorVec2;
    return defaultVectorVec2;
}

std::string GameUtils::getHintDropPlaceTypeName(HintDropPlaceType type)
{
    switch (type)
    {
        case HintDropPlaceType::HintDropPlaceNone:return "HintPlaceNone";
        case HintDropPlaceType::HintDropPlaceAtThrowCardsArea:return "HintPlaceAtThrowCardsArea";
        default:return "HintPlaceNone";
    }
    return "HintPlaceNone";
}

cocos2d::ui::Layout* GameUtils::getLayoutTemplate(const std::map<std::string, cocos2d::ui::Layout*>& map, const std::string& name)
{
    if (map.find(name) != map.end())
        return map.at(name);
    return nullptr;
}

void GameUtils::showLoading(cocos2d::Node* attachNode /*= nullptr*/)
{
    Node* node = nullptr;
    if (attachNode == nullptr)
    {
        Scene* scene = Director::getInstance()->getRunningScene();
        if (scene != nullptr)
            node = scene;
    }
    else
    {
        node = attachNode;
    }
    if (node != nullptr)
    {
        LoadingLayer* loadingLayer = LoadingLayer::createInstance();
        loadingLayer->setDelayTime(SHOW_LOADING_DELAYTIME);
        loadingLayer->attach(node);
        loadingLayer->setAutoHide(true);
        loadingLayer->show();
    }
}

void GameUtils::showAdsLoading(cocos2d::Node* attachNode, float delayTime, const std::function<void()>& callback)
{
    Node* node = nullptr;
    if (attachNode == nullptr)
    {
        Scene* scene = Director::getInstance()->getRunningScene();
        if (scene != nullptr)
            node = scene;
    }
    else
    {
        node = attachNode;
    }
    if (node != nullptr)
    {
        LoadingLayer* loadingLayer = LoadingLayer::createInstance();
        loadingLayer->setDelayTime(0);
        loadingLayer->attach(node);
        loadingLayer->setAutoHide(true);
        loadingLayer->showAdsLoading(delayTime, callback);
    }
}

void GameUtils::hideLoading()
{
    LoadingLayer* loadingLayer = LoadingLayer::getInstance();
    if (loadingLayer)
        loadingLayer->hide();
}

void GameUtils::checkToRestoreSoundBackground()
{
    if (UserData::getInstance()->getRestoreSoundBG() && !SoundManager::getInstance()->isMusicActive())
    {
        CallToOtherLanguage::getInstance()->forceToRegisterAudioFocusListenner();
        UserData::getInstance()->setRestoreSoundBG(false);
        UserData::getInstance()->setSoundBackground(true);
        SoundManager::getInstance()->stopBackgroundMusicAuto();
        SoundManager::getInstance()->playBackgroundMusicAuto();
    }
}

cocos2d::Rect GameUtils::convertToWorldSpaceBoundingBox(cocos2d::Node* parent, cocos2d::Node* node)
{
    const cocos2d::Point& pos  = node->getPosition();
    const cocos2d::Size & size = node->getContentSize();
    cocos2d::Point topLeft  = parent->convertToWorldSpace(cocos2d::Point(pos.x - size.width / 2, pos.y + size.height / 2));
    cocos2d::Point topRight = parent->convertToWorldSpace(cocos2d::Point(pos.x + size.width / 2, pos.y + size.height / 2));
    cocos2d::Point botRight = parent->convertToWorldSpace(cocos2d::Point(pos.x + size.width / 2, pos.y - size.height / 2));
    cocos2d::Point botLeft  = parent->convertToWorldSpace(cocos2d::Point(pos.x - size.width / 2, pos.y - size.height / 2));
    cocos2d::Rect  bd;
    bd.origin = botLeft;
    bd.size   = cocos2d::Size(abs(topRight.x - topLeft.x), abs(topRight.y - botRight.y));
    return bd;
}

cocos2d::Rect GameUtils::convertToNodeBoundingBox(cocos2d::Node* parent, const cocos2d::Rect& boundingBox)
{
    cocos2d::Point topLeft  = parent->convertToNodeSpace(cocos2d::Point(boundingBox.origin.x, boundingBox.origin.x + boundingBox.size.height));
    cocos2d::Point topRight = parent->convertToNodeSpace(cocos2d::Point(boundingBox.origin.x + boundingBox.size.width, boundingBox.origin.y + boundingBox.size.height));
    cocos2d::Point botRight = parent->convertToNodeSpace(cocos2d::Point(boundingBox.origin.x + boundingBox.size.width, boundingBox.origin.y));
    cocos2d::Point botLeft  = parent->convertToNodeSpace(cocos2d::Point(boundingBox.origin.x, boundingBox.origin.y));
    cocos2d::Rect  bd;
    bd.origin = botLeft;
    bd.size   = cocos2d::Size(abs(topRight.x - topLeft.x), abs(topRight.y - botRight.y));
    return bd;
}

void GameUtils::pauseGame()
{
    Director::getInstance()->stopAnimation();
    SoundManager::getInstance()->pauseBackgroundMusic();
}

void GameUtils::resumeGame()
{
    Director::getInstance()->startAnimation();
    SoundManager::getInstance()->resumeBackgroundMusic();
}

void GameUtils::showRatingPopup(cocos2d::Node* parent, int state, const std::string& condition)
{
    if (parent != nullptr)
    {
        RatingPopup* ratingPopup = BoardManager::createRatingPopup();
        ratingPopup->setStateRating(state);
        ratingPopup->setConditionOpen(condition);
        ratingPopup->attach(parent, kAboveForeground);
        ratingPopup->setTag((int) BoardID::RATING_POPUP);
    }
}

void GameUtils::showRatingAndReview(int state, const std::string& condition)
{
    if (RemoteConfig::getBoolean(REMOTECONFIG_FUNCTION_RATING_IN_APP_REVIEW_ENABLE, false))
    {
        CallToOtherLanguage::getInstance()->showInAppReview(state, condition);
    }
    else
    {
        cocos2d::ValueMap arr;
        arr[EVENT_KEY_SHOW_RATING_STATE]     = state;
        arr[EVENT_KEY_SHOW_RATING_CONDITION] = std::string(condition);
        NotificationManager::getInstance()->postNotification(EVENT_SHOW_CUSTOM_APP_REVIEW, cocos2d::Value(arr));
    }
}

std::string GameUtils::getDeveloperName()
{
    std::string name = RemoteConfig::getString(REMOTECONFIG_FUNCTION_DEVELOPER_NAME, "");
    if (name != "")
        return name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    return "iDream Game Studio";
#else
    return "Greenleaf Game";
#endif
    return "";
}


std::string GameUtils::getMoneyRangeName(double money)
{
    //min <= money < max
    for (const MoneyRangeDefine& range : s_moneyRangeName)
    {
        if (range._min != -1 && money < range._min)
            continue;
        if (range._max != -1 && money >= range._max)
            continue;
        if (range._min == -1 && range._max == -1)
            continue;
        if (range._min == -1 && money < range._max)
            return range._name;
        if (range._max == -1 && money >= range._min)
            return range._name;
        if (range._min <= money && money < range._max)
            return range._name;
    }
    return "Unknown";
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

jstring GameUtils::string2jString(JNIEnv* env, const string& str)
{
    return env->NewStringUTF(str.c_str());
}

string GameUtils::jString2String(JNIEnv* env, jstring jstr)
{
    const char* cs = env->GetStringUTFChars(jstr, NULL);
    string    ret(cs);
    env->ReleaseStringUTFChars(jstr, cs);
    return ret;
}

list<string> GameUtils::jStringArray2StringList(JNIEnv* env, jobjectArray array)
{
    list<string> ll;
    jsize        count = env->GetArrayLength(array);
    for (int     i     = 0; i < count; i++)
    {
        jstring   jstr   = (jstring) env->GetObjectArrayElement(array, i);
        const char* cstr = env->GetStringUTFChars(jstr, NULL);
        ll.push_back(cstr);
        env->ReleaseStringUTFChars(jstr, cstr);
    }

    return ll;
}

jobjectArray GameUtils::jStringVector2StringArray(JNIEnv* env, const std::vector<std::string>& vector)
{
    jclass       clazz    = ( env )->FindClass("java/lang/String");
    jobjectArray objarray = ( env )->NewObjectArray(vector.size(), clazz, 0);
    for (int     i        = 0; i < vector.size(); i++)
    {
        string  s  = vector[i];
        jstring js = ( env )->NewStringUTF(s.c_str());
        ( env )->SetObjectArrayElement(objarray, i, js);
    }
    return objarray;
}

vector<string> GameUtils::jStringArray2StringVector(JNIEnv* env, jobjectArray array)
{
    vector<string> ll;
    jsize          count = env->GetArrayLength(array);
    for (int       i     = 0; i < count; i++)
    {
        jstring   jstr   = (jstring) env->GetObjectArrayElement(array, i);
        const char* cstr = env->GetStringUTFChars(jstr, NULL);
        ll.push_back(cstr);
        env->ReleaseStringUTFChars(jstr, cstr);
    }

    return ll;
}


list<string> GameUtils::valueVector2StringList(const ValueVector& v)
{
    list<string> ret;
    for (auto    i = v.begin(); i != v.end(); i++)
    {
        ret.push_back(i->asString());
    }
    return ret;
}

#endif
