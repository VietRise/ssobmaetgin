#include "RuleThrowCard.h"
#include "RuleCommon.h"
#include "RuleInstantWin.h"
#include "RuleKnocking.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

bool RuleThrowCard::canThrow(const int& throwCard, const vector<int>& cards)
{
    return SvrCard::containsCard(throwCard, cards);
}

pair<RuleConstant::ActionType, int> RuleThrowCard::getActionType(const vector<int>& cards, const int& knockPoint)
{
    return getActionTypeDeadwood(RuleCommon::getResultInfo(cards).deadwood, knockPoint);
}

pair<RuleConstant::ActionType, int> RuleThrowCard::getActionTypeDeadwood(const vector<int>& deadwood, const int& knockPoint)
{
    RuleConstant::ResultType type   = RuleInstantWin::getResultType(deadwood);
    RuleConstant::ActionType action = RuleConstant::ActionType::NONE;
    int                      point  = -1;
    if (type == RuleConstant::ResultType::WIN_BIG_GIN)
    {
        action = RuleConstant::ActionType::BIG_GIN;
        point  = 0;
    }
    else if (type == RuleConstant::ResultType::WIN_GIN)
    {
        action = RuleConstant::ActionType::GIN;
        point  = 0;
    }
    else
    {
        int handPoints = RuleCommon::getPoints(deadwood, true);
        if (RuleKnocking::canKnock(knockPoint, handPoints))
        {
            action = RuleConstant::ActionType::KNOCK;
            point  = handPoints;
        }
    }
    return std::make_pair(action, point);
}
