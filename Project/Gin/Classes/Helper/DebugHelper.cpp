#include "DebugHelper.h"
#include "Helper/StringHelper.h"
#include "ServerOffline/Rules/RuleCommon.h"
#include "ServerOffline/Rules/RuleKnocking.h"
#include "ServerOffline/InGameServerOfflineDefines.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

string DebugHelper::player2Str(int playerIndex)
{
    return (playerIndex == INDEX_NONE ? "" : (playerIndex == INDEX_PLAYER ? "USER " : "BOT " + to_string(playerIndex)));
}

cocos2d::Color3B DebugHelper::player2Color(int playerIndex)
{
    switch (playerIndex)
    {
        case 0: return cocos2d::Color3B::GREEN;
        case 1: return cocos2d::Color3B::YELLOW;
        case 2: return cocos2d::Color3B::ORANGE;
        case 3: return cocos2d::Color3B::YELLOW;
        case 4: return cocos2d::Color3B::ORANGE;
        case 5: return cocos2d::Color3B::YELLOW;
        case 6: return cocos2d::Color3B::ORANGE;
        case 7: return cocos2d::Color3B::YELLOW;
        case 8: return cocos2d::Color3B::ORANGE;
        default: return cocos2d::Color3B::WHITE;
    }
}

void DebugHelper::action2Log(const string& name, int value, LogHelper::vlogs& refLogs)
{
    action2Log(name, to_string(value), refLogs);
}

void DebugHelper::action2Log(const string& name, long value, LogHelper::vlogs& refLogs)
{
    action2Log(name, to_string(value), refLogs);
}

void DebugHelper::action2Log(const string& name, float value, LogHelper::vlogs& refLogs)
{
    action2Log(name, to_string(value), refLogs);
}

void DebugHelper::action2Log(const string& name, double value, LogHelper::vlogs& refLogs)
{
    action2Log(name, to_string(value), refLogs);
}

void DebugHelper::action2Log(const string& name, const string& value, LogHelper::vlogs& refLogs)
{
    refLogs.emplace_back(name, value);
}

void DebugHelper::action2Log(const string& name, const vector<int>& cards, LogHelper::vlogs& refLogs)
{
    action2Log(StringHelper::format("%s (%d)", name.c_str(), cards.size()), cards2Str(cards), refLogs);
}

string DebugHelper::suits2Str(int suit)
{
    return SvrCard::getSuitName(suit);
}

string DebugHelper::ranks2Str(int rank)
{
    return SvrCard::getRankName(rank);
}

string DebugHelper::cards2Str(const vector<int>& cards, bool onlyShowIndex, bool hideIndex)
{
    if (cards.empty())
        return StringHelper::EMPTY_STRING;

    string showIndexStr;
    if (onlyShowIndex || !hideIndex)
    {
        int max = 0;
        for (const int& card : cards)
        {
            int len = to_string(card).length();
            if (len > max)
                max = len;
        }

        vector<string> elements;
        for (const int& card : cards)
        {
            string fmt = StringHelper::format("%%%dd", max);
            string s   = StringHelper::format(fmt, card);
            elements.push_back(s);
        }
        showIndexStr = StringHelper::joinDelimiterPrefixSuffix(StringHelper::COMMA_STRING, "{", "}", elements);
    }

    string s;
    if (onlyShowIndex)
        s = showIndexStr;
    else
    {
        for (const int& card : cards)
        {
            const string& rankStr = ranks2Str(SvrCard::convertRank(card));
            const string& suitStr = suits2Str(SvrCard::convertSuit(card));
            if (!s.empty())
                s.append(StringHelper::SPACE_STRING);
            s.append(rankStr).append(suitStr);
        }
        if (!hideIndex)
            s.append(" | Index: ").append(showIndexStr);
    }
    return s;
}

void DebugHelper::melds2Log(const vector<vector<int>>& melds, LogHelper::vlogs& refLogs)
{
    if (!melds.empty())
    {
        string   s;
        for (int i = 0; i < melds.size(); i++)
        {
            vector<int> temp = melds.at(i);
            SvrCard::sortAsc(temp);
            if (i > 0)
                s += " - ";
            s += "[" + StringHelper::trim(cards2Str(temp)) + "]";
        }
        action2Log(StringHelper::format("Meld (%d)", melds.size()), s, refLogs);
    }
}

void DebugHelper::printDealCards(const vector<HandCard*>& handCards, int lastWinIndex, int botLevel, int botVersion, int upCard, const vector<int>& pileDiscards, const vector<int>& remainCards)
{
    // [START: deal_card]
    LogHelper::vlogs logs;
    action2Log("lastWinIndex", lastWinIndex, logs);
    for (const auto& hand : handCards)
        action2Log(player2Str(hand->index), hand->handCards, logs);
    action2Log("Up card", vector<int>(upCard), logs);
    action2Log("Discards", pileDiscards, logs);
    action2Log("Remain cards", remainCards, logs);
    LogHelper::match("DEAL CARD", logs);
    // [END: deal_card]

    // [START: c++ auto generated]
    logs.clear(); // reset
    action2Log("_cheatFirstMoveSlotIdx", to_string(lastWinIndex) + ";", logs);
    for (int i = 0; i < handCards.size(); i++)
    {
        const string& name  = "_mapCheatCardPlayers[" + to_string(i) + "]";
        const string& value = cards2Str(handCards.at(i)->handCards, true) + "; // " + player2Str(handCards.at(i)->index);
        action2Log(name, value, logs);
    }
    vector<int> temp(remainCards.begin(), remainCards.end());
    temp.push_back(upCard);
    logs.emplace_back("cheatRemainCards", cards2Str(temp, true) + ";");
    action2Log("_levelBot", to_string(botLevel) + ";", logs);

    LogHelper::match("C++ (generated)", logs, cocos2d::Color3B::WHITE, " =");
    LogHelper::match("C++ (generated)", "BotHelper::cheatVersion(" + to_string(botVersion) + ");");
    // [END: c++ auto generated]
}

void DebugHelper::printHandAction(const string& action, int cardIndex, HandCard* hand, const vector<int>& pileDiscards, const vector<int>& remainCards, int knockPoints, bool isLogKnockPoint, bool isLogPick, bool isUpCard)
{
    LogHelper::vlogs logs;
    printHandCommon(action, cardIndex, hand, knockPoints, isLogKnockPoint, logs, false, isUpCard);

    if (hand != nullptr)
        LogHelper::match(player2Str(hand->index), logs, player2Color(hand->index));

    if (isLogPick)
    {
        logs.clear();
        action2Log("Discards", pileDiscards, logs);
        action2Log("Remain cards", remainCards, logs);
        LogHelper::match("DECK ", logs);
        LogHelper::matchEmptyRow();
    }
}

void DebugHelper::printHandCommon(const string& action, int cardIndex, HandCard* hand, int knockPoints, bool isLogKnockPoint, LogHelper::vlogs& refLogs, bool isEndMatch, bool isUpCard)
{
    if (hand == nullptr)
        return;

    if (!action.empty())
    {
        string actionName = action;
        if (SvrCard::isValidIndex(cardIndex))
            actionName.append(StringHelper::format(" (%s)", StringHelper::trim(cards2Str({ cardIndex })).c_str()));
        if (isUpCard)
            actionName.append(" (up card)");
        action2Log("Action", actionName, refLogs);
    }

    // melds
    if (!hand->meldCards.empty())
        melds2Log(hand->meldCards, refLogs);

    // hand cards
    vector<int> tempHandCards = SvrCard::cloneCards(hand->handCards);
    if (!isEndMatch)
    {
        SvrCard::sortAsc(tempHandCards);
        action2Log("Hand", tempHandCards, refLogs);
    }

    // auto melds
    const auto& result = RuleCommon::getResultInfo(tempHandCards);
    SvrCard::removeCard(result.melds, tempHandCards);
    if (!result.melds.empty())
        melds2Log(result.melds, refLogs);

    // layoff cards
    if (isEndMatch && !hand->layoffCards.empty())
    {
        vector<int> layoffCards;
        for (auto& layoff : hand->layoffCards)
            for (const int& card : layoff.getCards())
                if (!SvrCard::containsCard(card, layoffCards))
                    layoffCards.push_back(card);
        SvrCard::sortAsc(layoffCards);
        action2Log("Layoff", layoffCards, refLogs);
    }

    // deadwood cards
    SvrCard::sortAsc(tempHandCards);
    action2Log("Deadwood", tempHandCards, refLogs);

    // point
    action2Log("Point", StringHelper::format("%d/%d", result.points, knockPoints), refLogs);

    // knock point
    if (isLogKnockPoint)
    {
        const int& knockPoint = RuleCommon::getPoints(tempHandCards, true);
        if (RuleKnocking::canKnock(knockPoints, knockPoint))
            action2Log("Knock point", knockPoint, refLogs);
    }
}

void DebugHelper::printMatchResult(const vector<HandCard*>& handCards, int lastWinIndex, const vector<int>& winOrder, const map<int, map<int, double>>& results, const map<int, int>& targetPoints, int targetScore, bool isSpadesDouble, bool hasJackpot)
{
    const set<int> keys{ RESULT_DETAIL_WIN_POINTS, RESULT_DETAIL_BONUS_POINTS_GIN, RESULT_DETAIL_BONUS_POINTS_BIG_GIN, RESULT_DETAIL_BONUS_POINTS_UNDERCUT, RESULT_DETAIL_BONUS_POINTS_SPADES_DOUBLE };
    for (const auto& hand : handCards)
    {
        if (hand != nullptr)
        {
            const auto& mapResult = results.at(hand->index);
            string       playerName = player2Str(hand->index);
            const string result     = (winOrder.empty() ? "Draw" : (lastWinIndex == hand->index ? "Winner" : ""));
            if (!result.empty())
                playerName += " (" + result + ")";

            int totalPoints = 0;
            for (const int& key : keys)
                totalPoints += (int) mapResult.at(key);

            LogHelper::vlogs logs;
            action2Log("Hand points", (int) mapResult.at(RESULT_DETAIL_HAND_POINTS), logs);
            action2Log("Hand points", (int) mapResult.at(RESULT_DETAIL_HAND_POINTS), logs);
            action2Log("Win points", (int) mapResult.at(RESULT_DETAIL_WIN_POINTS), logs);
            logs.emplace_back("Target points", StringHelper::format("%d/%d", targetPoints.at(hand->index), targetScore));
            action2Log("Bonus points GIN", (int) mapResult.at(RESULT_DETAIL_BONUS_POINTS_GIN), logs);
            action2Log("Bonus points BIG GIN", (int) mapResult.at(RESULT_DETAIL_BONUS_POINTS_BIG_GIN), logs);
            action2Log("Bonus points UNDERCUT", (int) mapResult.at(RESULT_DETAIL_BONUS_POINTS_UNDERCUT), logs);
            if (isSpadesDouble)
                action2Log("Bonus points Spades Double", (int) mapResult.at(RESULT_DETAIL_BONUS_POINTS_SPADES_DOUBLE), logs);
            logs.emplace_back("Total points", to_string(totalPoints));
            action2Log("Normal Win", (int) mapResult.at(RESULT_DETAIL_NORMAL_WIN), logs);
            action2Log("Bonus Win", (int) mapResult.at(RESULT_DETAIL_BONUS_WIN), logs);
            if (hasJackpot)
                action2Log("Jackpot Win", (int) mapResult.at(RESULT_DETAIL_JACKPOT_WIN), logs);
            action2Log("Total Win", (int) mapResult.at(RESULT_DETAIL_NORMAL_WIN) + mapResult.at(RESULT_DETAIL_BONUS_WIN) + mapResult.at(RESULT_DETAIL_JACKPOT_WIN), logs);
            LogHelper::match("RESULT DETAILS - " + playerName, logs);
        }
    }
}
