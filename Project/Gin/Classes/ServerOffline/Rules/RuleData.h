#ifndef __RULE_DATA_H__
#define __RULE_DATA_H__

#include "RuleConstant.h"
#include "Helper/LogHelper.h"
#include "Helper/StringHelper.h"
#include "ServerOffline/SvrCard.h"

#include <vector>

struct RDPlayer
{
    int                      index      = INDEX_NONE;
    std::vector<int>         cards;
    std::vector<int>         takeCards;
    std::vector<int>         throwCards;
    RuleConstant::ActionType actionType = RuleConstant::ActionType::NONE;
};

struct RDResult
{
    RuleConstant::ResultType      type       = RuleConstant::ResultType::NONE;
    std::vector<std::vector<int>> melds;
    std::vector<int>              deadwood;
    int                           points     = 0;
    int                           knockPoint = 0;

    void pushMeld(const std::vector<int>& meld)
    {
        melds.push_back(meld);
    }

    int getHighestDeadwood() const
    {
        return SvrCard::getMaxIndex(deadwood);
    }

    std::string type2Str() const
    {
        switch (type)
        {
            case RuleConstant::ResultType::NONE: return STRINGIFY(RuleConstant::ResultType::NONE);
            case RuleConstant::ResultType::POINT: return STRINGIFY(RuleConstant::ResultType::POINT);
            case RuleConstant::ResultType::WIN_KNOCK: return STRINGIFY(RuleConstant::ResultType::WIN_KNOCK);
            case RuleConstant::ResultType::WIN_GIN: return STRINGIFY(RuleConstant::ResultType::WIN_GIN);
            case RuleConstant::ResultType::WIN_BIG_GIN: return STRINGIFY(RuleConstant::ResultType::WIN_BIG_GIN);
            case RuleConstant::ResultType::LOSE_KNOCK: return STRINGIFY(RuleConstant::ResultType::LOSE_KNOCK);
            case RuleConstant::ResultType::LOSE_GIN: return STRINGIFY(RuleConstant::ResultType::LOSE_GIN);
            case RuleConstant::ResultType::LOSE_BIG_GIN: return STRINGIFY(RuleConstant::ResultType::LOSE_BIG_GIN);
            case RuleConstant::ResultType::DRAW: return STRINGIFY(RuleConstant::ResultType::DRAW);
            case RuleConstant::ResultType::WIN_UNDERCUT: return STRINGIFY(RuleConstant::ResultType::WIN_UNDERCUT);
            case RuleConstant::ResultType::LOSE_UNDERCUT: return STRINGIFY(RuleConstant::ResultType::LOSE_UNDERCUT);
            default: return StringHelper::NOTFOUND_STRING;
        }
    }
};

#endif // __RULE_DATA_H__
