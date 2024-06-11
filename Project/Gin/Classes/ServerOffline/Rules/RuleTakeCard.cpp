#include "RuleTakeCard.h"
#include "RuleCommon.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

bool RuleTakeCard::canTake(const int& takeCard, const vector<int>& cards)
{
    if (SvrCard::isValidIndex(takeCard))
        for (const auto& meld : RuleCommon::getResultInfo(SvrCard::mergeCard(cards, takeCard)).melds)
            if (SvrCard::containsCard(takeCard, meld))
                return true;
    return false;
}
