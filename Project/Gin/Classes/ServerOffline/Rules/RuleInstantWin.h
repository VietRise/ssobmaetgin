#ifndef __RULE_INSTANT_WIN_H__
#define __RULE_INSTANT_WIN_H__

#include "RuleConstant.h"

#include <vector>

class RuleInstantWin
{
public:
    static RuleConstant::ResultType getResultType(const std::vector<int>& deadwood);
    static bool canInstantWin(const std::vector<int>& deadwood);
    static int getDeadwood(const std::vector<int>& cards);
};

#endif // __RULE_INSTANT_WIN_H__