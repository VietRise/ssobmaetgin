#include "RuleKnocking.h"
#include "RuleCommon.h"

using namespace std;

bool RuleKnocking::canKnock(const int& knockPoints, const int& handPoints)
{
    return handPoints <= knockPoints;
}

bool RuleKnocking::canKnock(const int& knockPoints, const vector<int>& cards)
{
    return canKnock(knockPoints, RuleCommon::getResultInfo(cards).knockPoint);
}
