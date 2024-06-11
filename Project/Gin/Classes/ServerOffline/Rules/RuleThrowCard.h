#ifndef __RULE_THROW_CARD_H__
#define __RULE_THROW_CARD_H__

#include "RuleConstant.h"

#include <vector>

class RuleThrowCard
{
public:
    static bool canThrow(const int& throwCard, const std::vector<int>& cards);
    static std::pair<RuleConstant::ActionType, int> getActionType(const std::vector<int>& cards, const int& knockPoint);
    static std::pair<RuleConstant::ActionType, int> getActionTypeDeadwood(const std::vector<int>& deadwood, const int& knockPoint);
};

#endif // __RULE_THROW_CARD_H__