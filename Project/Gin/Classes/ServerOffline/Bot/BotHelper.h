#ifndef __BOT_HELPER_H__
#define __BOT_HELPER_H__

#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "json/rapidjson.h"
#include "json/document.h"

#include "Define/InGameDefine.h"
#include "ServerOffline/Rules/RuleConstant.h"
#include "ServerOffline/Rules/RuleData.h"
#include "BotConstant.h"
#include "BotOpponentHandEstimation.h"

class BotHelper
{
private:
    static int s_lastTakeCard;

    // pass_<level>
    static int pass_0(const int& upCard, const RDPlayer& usr, const RDPlayer& bot);
    static int pass_1(const int& upCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, const bool& isBeginTurn);
    static int pass_2(const int& upCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, const bool& isBeginTurn);

    // takeCard_<level>
    static int takeCard_0(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, int shuffleRemainCards);
    static int takeCard_1(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, int shuffleRemainCards);
    static int takeCard_2(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, int shuffleRemainCards);
    static int takeCard_3(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, int shuffleRemainCards);

    // throwCard_<level>
    static int throwCard_0(const RDPlayer& usr, const RDPlayer& bot);
    static int throwCard_1(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule);
    static int throwCard_2(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const std::vector<int>& pileDiscards);
    static int throwCard_n(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const std::vector<int>& pileDiscards);

    // knock_<level>
    static int knock_0(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints);
    static int knock_1(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints);
    static int knock_2(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints);
    static int knock_3(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints);
    static int knock_n(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints, BotConstant::Level level);

    // −2 if the card is in the player hand,
    // −1 if the card is a safe discard or is not in the opponent hand,
    // 1 if the card is in the opponent hand or likely to be in the opponent meld
    // 0 if the card is unknown.
    static BotOpponentHandEstimation userHandEstimation(const RDPlayer& user, const RDPlayer& bot, const std::vector<int>& pileDiscards);

public:
    static int getLevel(const double& userWinRate, const int& userMatchCount, const double& userMoney);
    static int getVersion();
#if USE_CHEAT_CARD
    static void cheatVersion(const int& version);
#endif

    static void resetData();

    static int pass(const int& level, const int& upCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards, const bool& isBeginTurn);
    static int takeCard(const int& level, const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, std::vector<int>& remainCards);
    static int throwCard(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const std::vector<int>& pileDiscards);
    static int knock(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints);
    static bool gin(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule);
    static bool bigGin(const int& level, const RDPlayer& usr, const RDPlayer& bot);
};

#endif // __BOT_HELPER_H__