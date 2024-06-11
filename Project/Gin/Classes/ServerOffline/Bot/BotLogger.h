#ifndef __BOT_LOGGER_H__
#define __BOT_LOGGER_H__

#include "Define/InGameDefine.h"

#if DEBUG_LOG_BOT

#include "Helper/LogHelper.h"
#include "ServerOffline/Rules/RuleConstant.h"

#include <vector>
#include <string>
#include <map>

class BotLogger
{
public:
    enum class Event
    {
        UNKNOWN,

        MATCH_COUNT,
        MATCH_LOSE,
        MATCH_WIN,
        MATCH_DRAW,

        RESULT_LOSE_KNOCK,
        RESULT_LOSE_GIN,
        RESULT_LOSE_BIG_GIN,
        RESULT_LOSE_UNDERCUT,
        RESULT_WIN_KNOCK,
        RESULT_WIN_GIN,
        RESULT_WIN_BIG_GIN,
        RESULT_WIN_UNDERCUT,

        ACTION_KNOCK,
        ACTION_GIN,
        ACTION_BIG_GIN,

        ACTION_KNOCK_WIN,
        ACTION_KNOCK_LOSE,
        ACTION_GIN_WIN,
        ACTION_GIN_LOSE,
        ACTION_BIG_GIN_WIN,
        ACTION_BIG_GIN_LOSE,

        JACKPOT_COUNT,
        JACKPOT_WIN,
    };

    enum class Param
    {
        UNKNOWN,
    };

private:
    static std::map<Event, int>                s_count;
    static std::map<Event, double>             s_money;
    static std::map<Event, std::map<int, int>> s_score;

    static std::map<Event, std::map<Param, int>>    s_paramCount;
    static std::map<Event, std::map<Param, double>> s_paramMoney;

    static float calculatePercentage(const int& number, const int& total);

    static std::string winRate2Str(const int& number, const int& total);
    static std::string percentOfTotal2Str(const int& number, const int& total);
    static std::string match2Str(const std::string& name, const int& win, const int& lose, const bool& showWinRate = false, const int& draw = -1);
    static std::string match2Str(const std::string& name, const Event& win, const Event& lose, const bool& showWinRate = false, const Event& draw = Event::UNKNOWN);
    static std::string money2Str(const std::string& name, const double& win, const double& lose);
    static std::string money2Str(const std::string& name, const Event& win, const Event& lose);

    static int sumCount(const std::map<int, int>& mapCount);
    static int sumCountInRange(const Event& start, const Event& end);
    static int sumParamCount(const Event& event, const std::vector<Param>& listParam);

    template<typename T>
    static int sumCount(T first)
    {
        return s_count[first];
    }

    template<typename T, typename... Args>
    static int sumCount(T first, Args... args)
    {
        return s_count[first] + sumCount(args...);
    }

    template<typename E, typename T>
    static int sumParamCount(E event, T first)
    {
        return s_paramCount[event][first];
    }

    template<typename E, typename T, typename... Args>
    static int sumParamCount(E event, T first, Args... args)
    {
        return s_paramCount[event][first] + sumParamCount(event, args...);
    }

    template<typename ...Args>
    static void debugStats(const std::string& format, Args&& ... args)
    {
        LogHelper::debugColorArgs(LogHelper::Group::BOT, cocos2d::Color3B::WHITE, cocos2d::StringUtils::format("STATS: %s", format.c_str()), std::forward<Args>(args)...);
    }

public:
    static void logCount(const Event& event);
    static void logMoney(const Event& event, const double& money);
    static void logScore(const Event& event, const int& score);

    static void logParamCount(const Event& event, const Param& param);
    static void logParamMoney(const Event& event, const Param& param, const double& money);

    static void logPrint(const int& level);

    static Event convertEvent(const RuleConstant::ResultType& type);
};

#endif // DEBUG_LOG_BOT
#endif // __BOT_LOGGER_H__