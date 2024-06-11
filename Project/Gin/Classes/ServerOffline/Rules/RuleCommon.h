#ifndef __RULE_COMMON_H__
#define __RULE_COMMON_H__

#include "RuleConstant.h"
#include "RuleData.h"

#include <vector>

class RuleCommon
{
private:
    static std::vector<bool> s_listCardFlag;
    static std::vector<int>  s_listCardRank;
    static std::vector<int>  s_listCardSuit;

    static std::vector<bool> initListCardFlag(const std::vector<int>& cards);
    static std::vector<int> initListCardRank(const std::vector<int>& cards);
    static std::vector<int> initListCardSuit(const std::vector<int>& cards);

    static RDResult initResult(const std::vector<std::vector<int>>& melds, const std::vector<int>& deadwood);

    static std::vector<std::vector<int>> getAllMeldOverlap(const std::vector<int>& cards);
    static std::vector<RDResult> parseCards(const std::vector<int>& cards);
    static std::vector<std::vector<int>> getAllStraightSuit(const std::vector<int>& cards);
    static void combinations(const std::vector<int>& input, int len, int startPosition, std::vector<int>& arr, std::vector<std::vector<int>>& output);
    static std::vector<std::vector<int>> getAllSameRank(const std::vector<int>& cards);
public:
    static std::string rule2Str(const RuleConstant::GameRule& gameRule);

    static RDResult getResultInfo(const std::vector<int>& cards);
    static int getPoints(const std::vector<int>& cards, bool isKnock = false);
    static int getPointSpadesDoubleBonus(int point, bool isSpadesDouble);
    static int getKnockPoints(const RuleConstant::GameRule& gameRule);

    static bool isMeld(const std::vector<int>& cards);
    static bool isStraightSuit(const std::vector<int>& cards);
    static bool isSameRank(const std::vector<int>& cards);
    static bool isSizeMeld(const std::vector<int>& cards);

    static RuleConstant::ResultType getResultTypeWin(RuleConstant::ActionType actionType);
    static RuleConstant::ResultType getResultTypeLose(RuleConstant::ActionType actionType);
};

#endif // __RULE_COMMON_H__