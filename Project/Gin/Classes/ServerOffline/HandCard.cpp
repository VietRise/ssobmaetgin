#include "HandCard.h"
#include "InGameServerOfflineDefines.h"
#include "Rules/RuleCommon.h"
#include "SvrCard.h"

using namespace std;

HandCard::~HandCard()
{
}

HandCard::HandCard(const int& index)
{
    this->index  = index;
    this->isUser = (index == INDEX_PLAYER);
}

void HandCard::reset()
{
    handCards.clear();
    throwCards.clear();
    meldCards.clear();
    _takeCards.clear();
}

void HandCard::setHandCards(const vector<int>& handCard)
{
    this->handCards.clear();
    for (const int& card : handCard)
        this->handCards.push_back(card);
}

void HandCard::addHandCard(const int& card)
{
    this->handCards.push_back(card);
}

void HandCard::removeHandCard(const int& card)
{
    this->handCards.erase(remove(this->handCards.begin(), this->handCards.end(), card), this->handCards.end());
}

void HandCard::removeHandCards(const vector<int>& cards)
{
    for (const int& card : cards)
        this->removeHandCard(card);
}

void HandCard::addThrowCard(const int& card)
{
    this->throwCards.push_back(card);
}

int HandCard::getLastThrowCard()
{
    return !this->throwCards.empty() ? this->throwCards.back() : SvrCard::Index::CARD_NONE;
}

int HandCard::getHandCardTotalPoints()
{
    return RuleCommon::getPoints(this->handCards);
}

void HandCard::addMeldCards(const vector<int>& meldCards)
{
    this->meldCards.push_back(meldCards);
}

void HandCard::addLayoffCard(const RuleLayoffCard& layoffCard)
{
    this->layoffCards.push_back(layoffCard);
}

void HandCard::addTakeCard(const int& card)
{
    this->_takeCards.push_back(card);
}

void HandCard::removeTakeCard(const int& card)
{
    this->_takeCards.erase(remove(this->_takeCards.begin(), this->_takeCards.end(), card), this->_takeCards.end());
}

const std::vector<int>& HandCard::getTakeCards()
{
    return this->_takeCards;
}
