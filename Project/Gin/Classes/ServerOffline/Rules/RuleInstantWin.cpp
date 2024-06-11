#include "RuleInstantWin.h"
#include "RuleCommon.h"

using namespace std;

RuleConstant::ResultType RuleInstantWin::getResultType(const vector<int>& deadwood)
{
    switch (deadwood.size())
    {
        case 0:return RuleConstant::ResultType::WIN_BIG_GIN;
        case 1:return RuleConstant::ResultType::WIN_GIN;
        default:return RuleConstant::ResultType::NONE;
    }
}

bool RuleInstantWin::canInstantWin(const vector<int>& deadwood)
{
    return getResultType(deadwood) != RuleConstant::ResultType::NONE;
}

int RuleInstantWin::getDeadwood(const vector<int>& cards)
{
    const auto& result = RuleCommon::getResultInfo(cards);
    return getResultType(result.deadwood) == RuleConstant::ResultType::WIN_GIN ? result.deadwood.at(0) : -1;
}
