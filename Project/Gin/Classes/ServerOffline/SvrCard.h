#ifndef __SVR_CARD_H__
#define __SVR_CARD_H__

#include <vector>
#include <string>
#include "Define/InGameDefine.h"

class SvrCard
{
public:
    class Common
    {
    public:
        static const int MAX_CARD = 52;
        static const int MAX_RANK = 13;
        static const int MAX_SUIT = 4;
    };

    class Rank
    {
    public:
        static const int RANK_NONE = -1;

        static const int RANK_A  = 0;
        static const int RANK_2  = 1;
        static const int RANK_3  = 2;
        static const int RANK_4  = 3;
        static const int RANK_5  = 4;
        static const int RANK_6  = 5;
        static const int RANK_7  = 6;
        static const int RANK_8  = 7;
        static const int RANK_9  = 8;
        static const int RANK_10 = 9;
        static const int RANK_J  = 10;
        static const int RANK_Q  = 11;
        static const int RANK_K  = 12;
    };

    class Suit
    {
    public:
        static const int SUIT_NONE = -1;

        static const int SUIT_DIAMONDS = 0;
        static const int SUIT_CLUBS    = 1;
        static const int SUIT_HEARTS   = 2;
        static const int SUIT_SPADES   = 3;
    };

    class Index
    {
    public:
        static const int CARD_NONE = -1;
    };

    class Point
    {
    public:
        static const int POINT_NONE = 0;

        static const int POINT_A  = 1;
        static const int POINT_2  = 2;
        static const int POINT_3  = 3;
        static const int POINT_4  = 4;
        static const int POINT_5  = 5;
        static const int POINT_6  = 6;
        static const int POINT_7  = 7;
        static const int POINT_8  = 8;
        static const int POINT_9  = 9;
        static const int POINT_10 = 10;
        static const int POINT_J  = 10;
        static const int POINT_Q  = 10;
        static const int POINT_K  = 10;
    };

    static int convertIndex(int rank, int suit);
    static int convertRank(int index);
    static int convertSuit(int index);

    static void sortAsc(std::vector<int>& cards);
    static void sortDesc(std::vector<int>& cards);
    static std::vector<int> sortAsc(const std::vector<int>& cards);
    static std::vector<int> sortDesc(const std::vector<int>& cards);
    static void sortAsc(std::vector<std::vector<int>>& cards);
    static void sortDesc(std::vector<std::vector<int>>& cards);
    static std::vector<std::vector<int>> sortAsc(const std::vector<std::vector<int>>& cards);
    static std::vector<std::vector<int>> sortDesc(const std::vector<std::vector<int>>& cards);

    static std::vector<int> cloneCards(const std::vector<int>& cards);

    static void removeCard(int remove, std::vector<int>& cards);
    static void removeCard(const std::vector<int>& listRemove, std::vector<int>& cards);
    static void removeCard(const std::vector<std::vector<int>>& listRemove, std::vector<int>& cards);

    static std::vector<int> mergeCard(const std::vector<int>& cards, int mergeCard);
    static std::vector<int> mergeCard(const std::vector<int>& cards, const std::vector<int>& mergeCards);

    static bool isRank(int index, int rank);
    static bool isSuit(int index, int suit);

    static bool isSameRank(const std::vector<int>& cards, int rank);
    static bool isSameRank(const std::vector<int>& cards);
    static bool isSameSuit(const std::vector<int>& cards, int suit);
    static bool isSameSuit(const std::vector<int>& cards);

    static bool containsCard(int card, const std::vector<int>& cards);

    static void shuffleCards(std::vector<int>& cards);
    static bool hasDuplicates(const std::vector<int>& cards);

    static bool isValidIndex(int index);
    static bool isValidRank(int rank);
    static bool isValidSuit(int suit);

    static int getMaxIndex(const std::vector<int>& cards);

    static bool isStraightSuit(const std::vector<int>& cards);

    static int getPoints(int index);

    static std::string getRankName(int rank);
    static std::string getSuitName(int suit);

    static std::vector<int> initAllCards();
    static std::vector<int> initAllCardsWithExcludes(const std::vector<int>& excludeCards);
};

#endif // __SVR_CARD_H__