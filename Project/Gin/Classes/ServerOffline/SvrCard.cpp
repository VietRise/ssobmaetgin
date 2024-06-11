#include "SvrCard.h"
#include "Define/InGameDefine.h"
#include "Helper/StringHelper.h"

using namespace std;

int SvrCard::convertIndex(int rank, int suit)
{
    return (rank * Common::MAX_SUIT) + suit;
}

int SvrCard::convertRank(int index)
{
    return (index / Common::MAX_SUIT);
}

int SvrCard::convertSuit(int index)
{
    return (index % Common::MAX_SUIT);
}

void SvrCard::sortAsc(vector<int>& cards)
{
    sort(cards.begin(), cards.end(), [](const int& c1, const int& c2) { return c1 < c2; });
}

void SvrCard::sortDesc(vector<int>& cards)
{
    sort(cards.begin(), cards.end(), [](const int& c1, const int& c2) { return c1 > c2; });
}

vector<int> SvrCard::sortAsc(const vector<int>& cards)
{
    vector<int> vec = cards;
    sortAsc(vec);
    return vec;
}

vector<int> SvrCard::sortDesc(const vector<int>& cards)
{
    vector<int> vec = cards;
    sortDesc(vec);
    return vec;
}

void SvrCard::sortAsc(vector<vector<int>>& cards)
{
    if (cards.empty())
        return;
    sort(cards.begin(), cards.end(), [](vector<int>& v1, vector<int>& v2) {
        SvrCard::sortAsc(v1);
        SvrCard::sortAsc(v2);
        const int size = (v1.size() < v2.size()) ? v1.size() : v2.size();
        for (int  i    = 0; i < size; i++)
        {
            const int r1 = convertRank(v1.at(i));
            const int r2 = convertRank(v2.at(i));
            if (r1 > r2)
                return false;
            if (r1 < r2)
                return true;

            const int s1 = convertSuit(v1.at(i));
            const int s2 = convertSuit(v2.at(i));
            if (s1 > s2)
                return false;
            if (s1 < s2)
                return true;
        }
        return false;
    });
}

void SvrCard::sortDesc(vector<vector<int>>& cards)
{
    if (cards.empty())
        return;
    sort(cards.begin(), cards.end(), [](const vector<int>& v1, const vector<int>& v2) {
        const int size = (v1.size() < v2.size()) ? v1.size() : v2.size();
        for (int  i    = 0; i < size; i++)
        {
            const int r1 = convertRank(v1.at(i));
            const int r2 = convertRank(v2.at(i));
            if (r1 > r2)
                return true;
            if (r1 < r2)
                return false;

            const int s1 = convertSuit(v1.at(i));
            const int s2 = convertSuit(v2.at(i));
            if (s1 > s2)
                return true;
            if (s1 < s2)
                return false;
        }
        return false;
    });
}

vector<vector<int>> SvrCard::sortAsc(const vector<vector<int>>& cards)
{
    vector<vector<int>> vec = cards;
    sortAsc(vec);
    return vec;
}

vector<vector<int>> SvrCard::sortDesc(const vector<vector<int>>& cards)
{
    vector<vector<int>> vec = cards;
    sortDesc(vec);
    return vec;
}

vector<int> SvrCard::cloneCards(const vector<int>& cards)
{
    vector<int> vec = cards;
    return vec;
}

void SvrCard::removeCard(int remove, vector<int>& cards)
{
    auto it = find(cards.begin(), cards.end(), remove);
    if (it != cards.end())
        cards.erase(it);
}

void SvrCard::removeCard(const vector<int>& listRemove, vector<int>& cards)
{
    for (const int& remove : listRemove)
        removeCard(remove, cards);
}

void SvrCard::removeCard(const vector<vector<int>>& listRemove, vector<int>& cards)
{
    for (const vector<int>& remove : listRemove)
        removeCard(remove, cards);
}

vector<int> SvrCard::mergeCard(const vector<int>& cards, int mergeCard)
{
    vector<int> result = cards;
    if (!containsCard(mergeCard, result))
        result.push_back(mergeCard);
    return result;
}

vector<int> SvrCard::mergeCard(const vector<int>& cards, const vector<int>& mergeCards)
{
    vector<int> result = cards;
    for (const int& card : mergeCards)
        if (!containsCard(card, result))
            result.push_back(card);
    return result;
}

bool SvrCard::isRank(int index, int rank)
{
    return convertRank(index) == rank;
}

bool SvrCard::isSuit(int index, int suit)
{
    return convertSuit(index) == suit;
}

bool SvrCard::isSameRank(const vector<int>& cards, int rank)
{
    return cards.size() >= 2 && !std::any_of(cards.begin(), cards.end(), [&](const int& index) { return !isRank(index, rank); });
}

bool SvrCard::isSameRank(const vector<int>& cards)
{
    return cards.size() >= 2 && isSameRank(cards, convertRank(cards.at(0)));
}

bool SvrCard::isSameSuit(const vector<int>& cards, int suit)
{
    return cards.size() >= 2 && !std::any_of(cards.begin(), cards.end(), [&](const int& index) { return !isSuit(index, suit); });
}

bool SvrCard::isSameSuit(const vector<int>& cards)
{
    return cards.size() >= 2 && isSameSuit(cards, convertSuit(cards.at(0)));
}

bool SvrCard::containsCard(int card, const vector<int>& cards)
{
    return std::find(cards.begin(), cards.end(), card) != cards.end();
}

void SvrCard::shuffleCards(vector<int>& cards)
{
    for (int i = 0; i <= 8; i++)
        std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
}

bool SvrCard::hasDuplicates(const vector<int>& cards)
{
    set<int> s(cards.begin(), cards.end());
    return s.size() != cards.size();
}

bool SvrCard::isValidIndex(int index)
{
    return index >= 0 && index < Common::MAX_CARD;
}

bool SvrCard::isValidRank(int rank)
{
    return rank >= 0 && rank < Common::MAX_RANK;
}

bool SvrCard::isValidSuit(int suit)
{
    return suit >= 0 && suit < Common::MAX_SUIT;
}

int SvrCard::getMaxIndex(const vector<int>& cards)
{
    return !cards.empty() ? *std::max_element(cards.begin(), cards.end()) : Index::CARD_NONE;
}

bool SvrCard::isStraightSuit(const vector<int>& cards)
{
    if (cards.size() >= 3)
    {
        vector<int> vec = SvrCard::sortAsc(cards);
        for (int    i   = 0; i < vec.size() - 1; i++)
        {
            if (convertRank(vec.at(i)) + 1 != convertRank(vec.at(i + 1)))
                return false;
            if (convertSuit(vec.at(i)) != convertSuit(vec.at(i + 1)))
                return false;
        }
        return true;
    }
    return false;
}

int SvrCard::getPoints(int index)
{
    switch (convertRank(index))
    {
        case Rank::RANK_A:return Point::POINT_A;
        case Rank::RANK_2:return Point::POINT_2;
        case Rank::RANK_3:return Point::POINT_3;
        case Rank::RANK_4:return Point::POINT_4;
        case Rank::RANK_5:return Point::POINT_5;
        case Rank::RANK_6:return Point::POINT_6;
        case Rank::RANK_7:return Point::POINT_7;
        case Rank::RANK_8:return Point::POINT_8;
        case Rank::RANK_9:return Point::POINT_9;
        case Rank::RANK_10:return Point::POINT_10;
        case Rank::RANK_J:return Point::POINT_J;
        case Rank::RANK_Q:return Point::POINT_Q;
        case Rank::RANK_K:return Point::POINT_K;
        default:return Point::POINT_NONE;
    }
}

string SvrCard::getRankName(int rank)
{
    switch (rank)
    {
        case Rank::RANK_A:return " A";
        case Rank::RANK_2:return " 2";
        case Rank::RANK_3:return " 3";
        case Rank::RANK_4:return " 4";
        case Rank::RANK_5:return " 5";
        case Rank::RANK_6:return " 6";
        case Rank::RANK_7:return " 7";
        case Rank::RANK_8:return " 8";
        case Rank::RANK_9:return " 9";
        case Rank::RANK_10:return "10";
        case Rank::RANK_J:return " J";
        case Rank::RANK_Q:return " Q";
        case Rank::RANK_K:return " K";
        default:return "RANK NOT FOUND";
    }
}

string SvrCard::getSuitName(int suit)
{
    switch (suit)
    {
        case Suit::SUIT_SPADES:return u8"\u2660"; // return "♠";
        case Suit::SUIT_CLUBS:return u8"\u2663"; // return "♣";
        case Suit::SUIT_DIAMONDS:return u8"\u2666"; // return "♦";
        case Suit::SUIT_HEARTS:return u8"\u2665"; // return "♥";
        default:return "SUIT NOT FOUND";
    }
}

vector<int> SvrCard::initAllCards()
{
    vector<int> cards;

    for (int card = Common::MAX_CARD - 1; card >= 0; card--)
        cards.push_back(card);
    shuffleCards(cards);

    return cards;
}

vector<int> SvrCard::initAllCardsWithExcludes(const vector<int>& excludeCards)
{
    vector<int> cards;

    for (int card = Common::MAX_CARD - 1; card >= 0; card--)
        if (!containsCard(card, excludeCards))
            cards.push_back(card);
    shuffleCards(cards);

    return cards;
}
