#ifndef HandCard_hpp
#define HandCard_hpp

#include <vector>
#include "Rules/RuleLayoffCard.h"

class HandCard
{
private:
    std::vector<int> _takeCards;

public:
    int                           index;
    bool                          isUser;
    std::vector<int>              handCards; // included: _takeCards
    std::vector<int>              throwCards; // excluded: _takeCards
    std::vector<std::vector<int>> meldCards;
    std::vector<RuleLayoffCard>   layoffCards;

    ~HandCard();
    HandCard(const int& index);

    void reset();
    void setHandCards(const std::vector<int>& handCards);
    void addHandCard(const int& card);
    void removeHandCard(const int& card);
    void removeHandCards(const std::vector<int>& cards);
    void addThrowCard(const int& card);
    int getLastThrowCard();
    int getHandCardTotalPoints();
    void addMeldCards(const std::vector<int>& meldCards);
    void addLayoffCard(const RuleLayoffCard& layoffCard);
    void addTakeCard(const int& card);
    void removeTakeCard(const int& card);
    const std::vector<int>& getTakeCards();
};

#endif /* HandCard_hpp */
