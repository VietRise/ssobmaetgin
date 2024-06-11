#ifndef __SVR_HELPER_H__
#define __SVR_HELPER_H__

#include <vector>
#include <map>

#include "Define/InGameDefine.h"

class SvrHelper
{
public:
    static std::vector<std::vector<int>> simpleDealCard(int numPlayer, std::vector<int>& remainCards);
    static std::map<int, std::vector<int>> simpleDealCardCheat(int numberPlayer, const std::map<int, std::vector<int>>& cheatedData, std::vector<int>& remainCards);
    static std::vector<std::vector<int>> checkDealCard(int totalPlayer, std::vector<int>& remainCards);
    static std::map<int, std::vector<int>> checkDealCardCheat(int totalPlayer, const std::map<int, std::vector<int>>& cheatedData, std::vector<int>& remainCards);
    static std::vector<SvrMultiplyBonus> getMatchBonus(const std::vector<SvrMultiplyBonus>& cityBonuses, float tableBonus);
};

#endif // __SVR_HELPER_H__
