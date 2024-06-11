#include "SvrHelper.h"
#include "ServerOffline/SvrCard.h"
#include "Rules/RuleConstant.h"

using namespace std;

vector<vector<int>> SvrHelper::simpleDealCard(int numPlayer, vector<int>& remainCards)
{
    vector<vector<int>> result;
    vector<int>         cards = SvrCard::initAllCards();

    for (int i = 0; i < numPlayer; i++)
        result.emplace_back();

    for (int i = 0; i < RuleConstant::Common::MAX_HAND_CARD; i++)
    {
        for (int j = 0; j < numPlayer; j++)
        {
            int card = cards.back();
            cards.pop_back();
            result[j].push_back(card);
        }
    }
    // Remain cards
    remainCards.clear();
    for (int card : cards)
        remainCards.push_back(card);
    return result;
}

map<int, vector<int>> SvrHelper::simpleDealCardCheat(int numberPlayer, const map<int, vector<int>>& cheatedData, vector<int>& remainCards)
{
    map<int, vector<int>> result = cheatedData;
    vector<int>           cards  = SvrCard::initAllCards();
    vector<int>           listCheat;

    for (int i = 0; i < numberPlayer; i++)
        for (const int& j : result[i])
            if (find(listCheat.begin(), listCheat.end(), j) == listCheat.end())
                listCheat.push_back(j);

    for (auto it = cards.begin(); it != cards.end();)
    {
        if (find(listCheat.begin(), listCheat.end(), *it) == listCheat.end())
            it++;
        else
            it = cards.erase(it);
    }

    for (int i = 0; i < numberPlayer; i++)
    {
        int      length = RuleConstant::Common::MAX_HAND_CARD - result[i].size();
        for (int j      = 0; j < length; j++)
        {
            int cardIdx = -1;
            do
            {
                cardIdx = cards.back();
                cards.pop_back();
            }
            while (find(listCheat.begin(), listCheat.end(), cardIdx) != listCheat.end());
            result[i].push_back(cardIdx);
        }
    }
    remainCards.clear();
    for (int card : cards)
        remainCards.push_back(card);
    return result;
}

vector<vector<int>> SvrHelper::checkDealCard(int totalPlayer, vector<int>& remainCards)
{
    return SvrHelper::simpleDealCard(totalPlayer, remainCards);
}

map<int, vector<int>> SvrHelper::checkDealCardCheat(int totalPlayer, const map<int, vector<int>>& cheatedData, vector<int>& remainCards)
{
    return SvrHelper::simpleDealCardCheat(totalPlayer, cheatedData, remainCards);
}

vector<SvrMultiplyBonus> SvrHelper::getMatchBonus(const vector<SvrMultiplyBonus>& cityBonuses, float tableBonus)
{
    SvrMultiplyBonus bonus;
    bonus.ratio  = 2;
    bonus.chance = tableBonus / 2;

    SvrMultiplyBonus bonus2;
    bonus2.ratio  = 3;
    bonus2.chance = tableBonus / 2;

    SvrMultiplyBonus bonus3;
    bonus3.ratio  = 1;
    bonus3.chance = 1 - tableBonus;

    return { bonus3, bonus, bonus2 };
}