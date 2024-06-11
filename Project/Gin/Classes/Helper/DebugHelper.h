#ifndef __DEBUG_HELPER_H__
#define __DEBUG_HELPER_H__

#include "cocos2d.h"
#include <string>

#include "Helper/LogHelper.h"
#include "ServerOffline/HandCard.h"

class DebugHelper
{
public:
    static std::string player2Str(int playerIndex);
    static cocos2d::Color3B player2Color(int playerIndex);

    static void action2Log(const std::string& name, int value, LogHelper::vlogs& refLogs);
    static void action2Log(const std::string& name, long value, LogHelper::vlogs& refLogs);
    static void action2Log(const std::string& name, float value, LogHelper::vlogs& refLogs);
    static void action2Log(const std::string& name, double value, LogHelper::vlogs& refLogs);
    static void action2Log(const std::string& name, const std::string& value, LogHelper::vlogs& refLogs);
    static void action2Log(const std::string& name, const std::vector<int>& cards, LogHelper::vlogs& refLogs);

    static std::string suits2Str(int suit);
    static std::string ranks2Str(int rank);
    static std::string cards2Str(const std::vector<int>& cards, bool onlyShowIndex = false, bool hideIndex = true);

    static void melds2Log(const std::vector<std::vector<int>>& melds, LogHelper::vlogs& refLogs);

    static void printDealCards(const std::vector<HandCard*>& handCards, int lastWinIndex, int botLevel, int botVersion, int upCard, const std::vector<int>& pileDiscards, const std::vector<int>& remainCards);
    static void
    printHandAction(const std::string& action, int cardIndex, HandCard* hand, const std::vector<int>& pileDiscards, const std::vector<int>& remainCards, int knockPoints, bool isLogKnockPoint = false, bool isLogPick = false, bool isUpCard = false);
    static void printHandCommon(const std::string& action, int cardIndex, HandCard* hand, int knockPoints, bool isLogKnockPoint, LogHelper::vlogs& refLogs, bool isEndMatch = false, bool isUpCard = false);
    static void printMatchResult(const std::vector<HandCard*>& handCards,
                                 int lastWinIndex,
                                 const std::vector<int>& winOrder,
                                 const std::map<int, std::map<int, double>>& results,
                                 const std::map<int, int>& targetPoints,
                                 int targetScore,
                                 bool isSpadesDouble,
                                 bool hasJackpot);
};

#endif // __DEBUG_HELPER_H__