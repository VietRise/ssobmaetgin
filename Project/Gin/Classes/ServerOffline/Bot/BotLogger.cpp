#include "BotLogger.h"

#if DEBUG_LOG_BOT

#include "Helper/GameUtils.h"
#include "ServerOffline/Bot/BotConfig.h"
#include "ServerOffline/Rules/RuleConstant.h"

USING_NS_CC;
using namespace std;

map<BotLogger::Event, int>           BotLogger::s_count;
map<BotLogger::Event, double>        BotLogger::s_money;
map<BotLogger::Event, map<int, int>> BotLogger::s_score;

map<BotLogger::Event, map<BotLogger::Param, int>>    BotLogger::s_paramCount;
map<BotLogger::Event, map<BotLogger::Param, double>> BotLogger::s_paramMoney;

float BotLogger::calculatePercentage(const int& number, const int& total)
{
    return total != 0 ? (float) number / total * 100.0f : 0;
}

string BotLogger::winRate2Str(const int& number, const int& total)
{
    return StringUtils::format("(win rate:%.0f%%)", calculatePercentage(number, total));
}

string BotLogger::percentOfTotal2Str(const int& number, const int& total)
{
    float percent = calculatePercentage(number, total);
    return (percent != 0) ? StringUtils::format("(%.0f%% of total)", percent) : "";
}

string BotLogger::match2Str(const string& name, const int& win, const int& lose, const bool& showWinRate, const int& draw)
{
    int count = win + lose;
    if (draw > -1)
        count += draw;
    string s = StringUtils::format("count:%d | win:%d | lose:%d", count, win, lose);
    if (draw > -1)
        s = StringUtils::format("(%s | draw:%d)", s.c_str(), draw);
    else
        s = StringUtils::format("(%s)", s.c_str());
    if (showWinRate)
        s = StringUtils::format("%s %s", s.c_str(), winRate2Str(win, count).c_str());
    return StringUtils::format("%s %s", name.c_str(), s.c_str());
}

string BotLogger::match2Str(const string& name, const Event& win, const Event& lose, const bool& showWinRate, const Event& draw)
{
    return match2Str(name, s_count[win], s_count[lose], showWinRate, (draw != Event::UNKNOWN ? s_count[draw] : -1));
}

string BotLogger::money2Str(const string& name, const double& win, const double& lose)
{
    const string& profitStr = GameUtils::MoneyFormat(win + lose);
    const string& winStr    = GameUtils::MoneyFormat(win);
    const string& loseStr   = GameUtils::MoneyFormat(lose);
    return StringUtils::format("%s (profit:%s | win:%s | lose:%s)", name.c_str(), profitStr.c_str(), winStr.c_str(), loseStr.c_str());
}

string BotLogger::money2Str(const string& name, const Event& win, const Event& lose)
{
    return money2Str(name, s_money[win], s_money[lose]);
}

int BotLogger::sumCount(const map<int, int>& mapCount)
{
    int total = 0;
    for (const auto& it : mapCount)
        total += it.second;
    return total;
}

int BotLogger::sumCountInRange(const Event& start, const Event& end)
{
    int        sum = 0;
    for (Event e   = start; e <= end; e = (Event) ((int) e + 1))
        sum += s_count[e];
    return sum;
}

int BotLogger::sumParamCount(const Event& event, const vector<Param>& listParam)
{
    int sum = 0;
    for (const auto& param : listParam)
        sum += s_paramCount[event][param];
    return sum;
}

void BotLogger::logCount(const Event& event)
{
    s_count[event]++;
}

void BotLogger::logMoney(const Event& event, const double& money)
{
    s_money[event] += money;
}

void BotLogger::logScore(const Event& event, const int& score)
{
    s_score[event][score]++;
}

void BotLogger::logParamCount(const BotLogger::Event& event, const BotLogger::Param& param)
{
    s_paramCount[event][param]++;
}

void BotLogger::logParamMoney(const Event& event, const Param& param, const double& money)
{
    s_paramMoney[event][param] += money;
}

BotLogger::Event BotLogger::convertEvent(const RuleConstant::ResultType& type)
{
    switch (type)
    {
        case RuleConstant::ResultType::WIN_KNOCK:return Event::RESULT_WIN_KNOCK;
        case RuleConstant::ResultType::WIN_GIN:return Event::RESULT_WIN_GIN;
        case RuleConstant::ResultType::WIN_BIG_GIN:return Event::RESULT_WIN_BIG_GIN;
        case RuleConstant::ResultType::LOSE_KNOCK:return Event::RESULT_LOSE_KNOCK;
        case RuleConstant::ResultType::LOSE_GIN:return Event::RESULT_LOSE_GIN;
        case RuleConstant::ResultType::LOSE_BIG_GIN:return Event::RESULT_LOSE_BIG_GIN;
        case RuleConstant::ResultType::DRAW:return Event::MATCH_DRAW;
        case RuleConstant::ResultType::WIN_UNDERCUT:return Event::RESULT_WIN_UNDERCUT;
        case RuleConstant::ResultType::LOSE_UNDERCUT:return Event::RESULT_LOSE_UNDERCUT;
        default:return Event::UNKNOWN;
    }
}

void BotLogger::logPrint(const int& level)
{
    debugStats("%s", "------------------------------------------------------------------");
    debugStats("Bot (version:%d | level:%d)", BotConfig::s_currentVersion, level);
    debugStats("%s", match2Str("Match", Event::MATCH_WIN, Event::MATCH_LOSE, true, Event::MATCH_DRAW).c_str());
    debugStats("%s", money2Str("Money", Event::MATCH_WIN, Event::MATCH_LOSE).c_str());

    debugStats("%s", "Result Type:");
    debugStats("%s", match2Str("\t\t   KNOCK", Event::RESULT_WIN_KNOCK, Event::RESULT_LOSE_KNOCK).c_str());
    debugStats("%s", match2Str("\t\t     GIN", Event::RESULT_WIN_GIN, Event::RESULT_LOSE_GIN).c_str());
    debugStats("%s", match2Str("\t\t BIG_GIN", Event::RESULT_WIN_BIG_GIN, Event::RESULT_LOSE_BIG_GIN).c_str());
    debugStats("%s", match2Str("\t\tUNDERCUT", Event::RESULT_WIN_UNDERCUT, Event::RESULT_LOSE_UNDERCUT).c_str());

    debugStats("%s", "Action Type:");
    debugStats("%s", match2Str("\t\t  KNOCK", Event::ACTION_KNOCK_WIN, Event::ACTION_KNOCK_LOSE, true).c_str());
    debugStats("%s", match2Str("\t\t    GIN", Event::ACTION_GIN_WIN, Event::ACTION_GIN_LOSE, true).c_str());
    debugStats("%s", match2Str("\t\tBIG_GIN", Event::ACTION_BIG_GIN_WIN, Event::ACTION_BIG_GIN_LOSE, true).c_str());

    vector<Param> listParam;
    for (const auto& it : s_paramCount[Event::ACTION_KNOCK])
        listParam.push_back(it.first);
    int sumParam = sumParamCount(Event::ACTION_KNOCK, listParam);

    int sum = sumCount(s_score[Event::ACTION_KNOCK]);
    debugStats("%s", "Knock Score:");
    for (const auto& it : s_score[Event::ACTION_KNOCK])
    {
        string scoreStr = StringHelper::format("SCORE_%d", it.first);
        debugStats("\t\t%8s (count:%d) %s", scoreStr.c_str(), it.second, percentOfTotal2Str(it.second, sum).c_str());
    }

    debugStats("Jackpot (count:%d | win:%d | money:%s) %s",
               s_count[Event::JACKPOT_COUNT],
               s_count[Event::JACKPOT_WIN],
               GameUtils::MoneyFormat(s_money[Event::JACKPOT_WIN]).c_str(),
               winRate2Str(s_count[Event::JACKPOT_WIN], s_count[Event::JACKPOT_COUNT]).c_str());
}
#endif // DEBUG_LOG_BOT