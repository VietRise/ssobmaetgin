#include "RuleCommon.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

vector<bool> RuleCommon::s_listCardFlag;
vector<int>  RuleCommon::s_listCardRank;
vector<int>  RuleCommon::s_listCardSuit;

RDResult RuleCommon::initResult(const vector<vector<int>>& melds, const vector<int>& deadwood)
{
    RDResult result;
    result.melds      = melds;
    result.deadwood   = deadwood;
    result.points     = RuleCommon::getPoints(result.deadwood);
    result.knockPoint = RuleCommon::getPoints(result.deadwood, true);
    return result;
}

vector<vector<int>> RuleCommon::getAllMeldOverlap(const vector<int>& cards)
{
    vector<vector<int>> results = getAllSameRank(cards);
    for (const vector<int>& v : getAllStraightSuit(cards))
        results.push_back(v);
    return results;
}

vector<RDResult> RuleCommon::parseCards(const vector<int>& cards)
{
    vector<RDResult> allResults;
    vector<int>      firstCards = cards;

    // find first
    vector<vector<int>> firstMelds = getAllMeldOverlap(firstCards);
    const int           size       = firstMelds.size();
    if (size <= 0)
    {
        auto result = initResult({}, firstCards);
        allResults.push_back(result);
        return allResults;
    }
    if (size == 1)
    {
        SvrCard::removeCard(firstMelds.at(0), firstCards);
        auto result = initResult(firstMelds, firstCards);
        allResults.push_back(result);
        return allResults;
    }
    for (const vector<int>& meld : firstMelds)
    {
        // find second
        vector<int> secondCards = SvrCard::cloneCards(firstCards);
        SvrCard::removeCard(meld, secondCards);
        vector<RDResult> resultLevel2 = parseCards(secondCards);
        for (auto& result : resultLevel2)
        {
            result.pushMeld(meld);
            allResults.push_back(result);
        }
    }
    return allResults;
}

vector<vector<int>> RuleCommon::getAllStraightSuit(const vector<int>& cards)
{
    vector<vector<int>> result;
    if (!isSizeMeld(cards))
        return result;

    const vector<bool>& listCardFlag = initListCardFlag(cards);
    const vector<int> & listCardSuit = initListCardSuit(cards);

    vector<vector<int>> straights;

    for (int s = 0; s < SvrCard::Common::MAX_SUIT; s++)
    {
        if (listCardSuit.at(s) < RuleConstant::Common::MIN_NUM_CARD_ON_MELD)
            continue;

        vector<int> straight;

        for (int r = 0; r < SvrCard::Common::MAX_RANK; r++)
        {
            const int index = SvrCard::convertIndex(r, s);
            if (listCardFlag.at(index))
                straight.push_back(index);
            else
            {
                if (isSizeMeld(straight))
                    straights.push_back(SvrCard::cloneCards(straight));
                straight.clear();
            }
        }
        if (isSizeMeld(straight))
            straights.push_back(SvrCard::cloneCards(straight));
        straight.clear();
    }

    for (const vector<int>& straight : straights)
    {
        int number = RuleConstant::Common::MAX_HAND_CARD;
        while (number > 2)
        {
            const int sj = straight.size() - (number - 1);
            for (int  j  = 0; j < sj; j++)
            {
                vector<int> helper(number);
                copy(straight.begin() + j, straight.begin() + j + number, helper.begin());
                result.push_back(helper);
            }
            number--;
        }
    }
    return result;
}

void RuleCommon::combinations(const vector<int>& input, int len, int startPosition, vector<int>& arr, vector<vector<int>>& output)
{
    if (len == 0)
    {
        vector<int> v;
        for (const int& a : arr)
            v.push_back(a);
        output.push_back(v);
        return;
    }
    for (int i = startPosition; i <= input.size() - len; i++)
    {
        arr.at(arr.size() - len) = input.at(i);
        combinations(input, len - 1, i + 1, arr, output);
    }
}

vector<vector<int>> RuleCommon::getAllSameRank(const vector<int>& cards)
{
    vector<vector<int>> result;
    if (!isSizeMeld(cards))
        return result;

    const vector<bool>& listCardFlag = initListCardFlag(cards);
    const vector<int> & listCardRank = initListCardRank(cards);

    for (int r = 0; r < SvrCard::Common::MAX_RANK; r++)
    {
        if (listCardRank.at(r) < RuleConstant::Common::MIN_NUM_CARD_ON_MELD)
            continue;

        vector<int> helper;
        for (int    s = 0; s < SvrCard::Common::MAX_SUIT; s++)
        {
            const int index = SvrCard::convertIndex(r, s);
            if (listCardFlag.at(index))
                helper.push_back(index);
        }
        if (isSizeMeld(helper))
        {
            if (helper.size() == 4)
                result.push_back(helper);
            vector<int> arr(RuleConstant::Common::MIN_NUM_CARD_ON_MELD);
            combinations(helper, RuleConstant::Common::MIN_NUM_CARD_ON_MELD, 0, arr, result); // tat ca bo 3 co the co.
        }
    }
    return result;
}

string RuleCommon::rule2Str(const RuleConstant::GameRule& gameRule)
{
    switch (gameRule)
    {
        case RuleConstant::GameRule::CLASSIC_GIN:return STRINGIFY(RuleConstant::GameRule::CLASSIC_GIN);
        case RuleConstant::GameRule::OKLAHOMA_GIN:return STRINGIFY(RuleConstant::GameRule::OKLAHOMA_GIN);
        case RuleConstant::GameRule::STRAIGHT_GIN:return STRINGIFY(RuleConstant::GameRule::STRAIGHT_GIN);
        default:return StringHelper::NOTFOUND_STRING;
    }
}

vector<bool> RuleCommon::initListCardFlag(const vector<int>& cards)
{
    if (s_listCardFlag.empty())
        for (int i = 0; i < SvrCard::Common::MAX_CARD; i++)
            s_listCardFlag.push_back(false);
    if (!cards.empty())
    {
        vector<bool> result = s_listCardFlag;
        for (const int& index : cards)
            if (index >= 0 && index < result.size())
                result[index] = true;
        return result;
    }
    return s_listCardFlag;
}

vector<int> RuleCommon::initListCardRank(const vector<int>& cards)
{
    if (s_listCardRank.empty())
        for (int i = 0; i < SvrCard::Common::MAX_RANK; i++)
            s_listCardRank.push_back(0);

    if (!cards.empty())
    {
        vector<int> result = s_listCardRank;
        for (const int& index : cards)
        {
            const int rank = SvrCard::convertRank(index);
            result.at(rank) = result.at(rank) + 1;
        }
        return result;
    }
    return s_listCardRank;
}

vector<int> RuleCommon::initListCardSuit(const vector<int>& cards)
{
    if (s_listCardSuit.empty())
        for (int i = 0; i < SvrCard::Common::MAX_SUIT; i++)
            s_listCardSuit.push_back(0);
    if (!cards.empty())
    {
        vector<int> result = s_listCardSuit;
        for (const int& index : cards)
        {
            const int suit = SvrCard::convertSuit(index);
            result.at(suit) = result.at(suit) + 1;
        }
        return result;
    }
    return s_listCardSuit;
}

RDResult RuleCommon::getResultInfo(const vector<int>& cards)
{
    vector<int> firstCards = cards;

    // find first
    vector<vector<int>> firstMelds     = getAllMeldOverlap(firstCards);
    const int           sizeFirstMelds = firstMelds.size();
    if (sizeFirstMelds == 0)
        return initResult({}, firstCards);
    if (sizeFirstMelds == 1)
    {
        SvrCard::removeCard(firstMelds.at(0), firstCards);
        return initResult(firstMelds, firstCards);
    }

    vector<RDResult> allResults;
    for (const vector<int>& meld : firstMelds)
    {
        // find second
        vector<int> secondCards = SvrCard::cloneCards(firstCards);
        SvrCard::removeCard(meld, secondCards);
        vector<RDResult> resultLevel2 = parseCards(secondCards);
        for (auto& result : resultLevel2)
        {
            result.pushMeld(meld);
            allResults.push_back(result);
        }
    }

    RDResult result;
    if (allResults.size() == 1)
        result = allResults.at(0);
    else if (allResults.size() > 1)
    {
        sort(allResults.begin(), allResults.end(), [](const RDResult& info1, const RDResult& info2) {
            if (info1.points < info2.points)
                return true;
            if (info1.points > info2.points)
                return false;

            vector<int> deadwoodCards1 = SvrCard::sortDesc(info1.deadwood);
            vector<int> deadwoodCards2 = SvrCard::sortDesc(info2.deadwood);
            if (!deadwoodCards1.empty())
            {
                const int size = (deadwoodCards1.size() <= deadwoodCards2.size()) ? deadwoodCards1.size() : deadwoodCards2.size();
                for (int  i    = 0; i < size; i++)
                {
                    if (deadwoodCards1.at(i) < deadwoodCards2.at(i))
                        return true;
                    if (deadwoodCards1.at(i) > deadwoodCards2.at(i))
                        return false;
                }
                if (deadwoodCards1.size() < deadwoodCards2.size())
                    return true;
                if (deadwoodCards1.size() > deadwoodCards2.size())
                    return false;
            }
            return false;
        });

        result = allResults.at(0);
    }
    return result;
}

int RuleCommon::getPoints(const vector<int>& cards, bool isKnock)
{
    vector<int> vec    = SvrCard::sortAsc(cards);
    const int   size   = isKnock ? (vec.size() - 1) : vec.size();
    int         points = 0;

    for (int i = 0; i < size; i++)
        points += SvrCard::getPoints(vec.at(i));
    return points;
}

int RuleCommon::getPointSpadesDoubleBonus(int point, bool isSpadesDouble)
{
    return (isSpadesDouble ? (point * 2 - point) : 0);
}

int RuleCommon::getKnockPoints(const RuleConstant::GameRule& gameRule)
{
    return gameRule == RuleConstant::GameRule::STRAIGHT_GIN ? 0 : RuleConstant::Common::KNOCK_POINTS;
}

bool RuleCommon::isMeld(const vector<int>& cards)
{
    return !SvrCard::hasDuplicates(cards) && (isSameRank(cards) || isStraightSuit(cards));
}

bool RuleCommon::isStraightSuit(const vector<int>& cards)
{
    return isSizeMeld(cards) && SvrCard::isStraightSuit(cards);
}

bool RuleCommon::isSameRank(const vector<int>& cards)
{
    return isSizeMeld(cards) && SvrCard::isSameRank(cards);
}

bool RuleCommon::isSizeMeld(const vector<int>& cards)
{
    return cards.size() >= RuleConstant::Common::MIN_NUM_CARD_ON_MELD;
}

RuleConstant::ResultType RuleCommon::getResultTypeWin(RuleConstant::ActionType actionType)
{
    switch (actionType)
    {
        case RuleConstant::ActionType::KNOCK:return RuleConstant::ResultType::WIN_KNOCK;
        case RuleConstant::ActionType::GIN:return RuleConstant::ResultType::WIN_GIN;
        case RuleConstant::ActionType::BIG_GIN:return RuleConstant::ResultType::WIN_BIG_GIN;
        case RuleConstant::ActionType::UNDERCUT:return RuleConstant::ResultType::WIN_UNDERCUT;
        default:return RuleConstant::ResultType::NONE;
    }
}

RuleConstant::ResultType RuleCommon::getResultTypeLose(RuleConstant::ActionType actionType)
{
    switch (actionType)
    {
        case RuleConstant::ActionType::KNOCK:return RuleConstant::ResultType::LOSE_KNOCK;
        case RuleConstant::ActionType::GIN:return RuleConstant::ResultType::LOSE_GIN;
        case RuleConstant::ActionType::BIG_GIN:return RuleConstant::ResultType::LOSE_BIG_GIN;
        case RuleConstant::ActionType::UNDERCUT:return RuleConstant::ResultType::LOSE_UNDERCUT;
        default:return RuleConstant::ResultType::NONE;
    }
}
