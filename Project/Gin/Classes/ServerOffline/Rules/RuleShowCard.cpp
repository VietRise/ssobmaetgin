#include "RuleShowCard.h"
#include "RuleCommon.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

vector<vector<int>> RuleShowCard::suggest(const vector<int>& cards, const vector<int>& takeCards)
{
    return RuleCommon::getResultInfo(SvrCard::mergeCard(cards, takeCards)).melds;
}
