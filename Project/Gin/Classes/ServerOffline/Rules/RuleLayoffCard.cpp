#include "RuleLayoffCard.h"
#include "RuleCommon.h"
#include "ServerOffline/SvrCard.h"

using namespace std;

RuleLayoffCard::~RuleLayoffCard() = default;

RuleLayoffCard::RuleLayoffCard()
{
    this->_senderIndex = -1;
    this->_position    = -1;
    this->_meldIndex   = -1;
}

RuleLayoffCard::RuleLayoffCard(int senderIndex, int position, int meldIndex, const vector<int>& cards)
{
    this->_senderIndex = senderIndex;
    this->_position    = position;
    this->_meldIndex   = meldIndex;
    this->addCard(cards);
}

int RuleLayoffCard::getPosition() const
{
    return this->_position;
}

void RuleLayoffCard::addCard(const int& card)
{
    this->_cards.push_back(card);
}

void RuleLayoffCard::addCard(const vector<int>& cards)
{
    for (const int c : cards)
        this->_cards.push_back(c);
}

void RuleLayoffCard::setCard(const int& card)
{
    this->_cards.clear();
    this->_cards.push_back(card);
}

void RuleLayoffCard::setCard(const vector<int>& cards)
{
    this->_cards.clear();
    addCard(cards);
}

const vector<int>& RuleLayoffCard::getCards() const
{
    return this->_cards;
}

int RuleLayoffCard::getMeldIndex() const
{
    return this->_meldIndex;
}

int RuleLayoffCard::getSenderIndex() const
{
    return this->_senderIndex;
}

//==================================================================================================================
// STATIC
vector<RuleLayoffCard> RuleLayoffCard::suggest(const vector<int>& cards, const map<int, vector<vector<int>>>& mapMeldCards, int senderIndex)
{
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    //	CCLOG("RuleLayoffCard::suggest cards: %s", SvrCard::debugCards(cards).c_str());
    //#endif
    vector<int> helperCards = SvrCard::sortDesc(cards);
    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    //	CCLOG("RuleLayoffCard::suggest helperCards: %s", SvrCard::debugCards(helperCards).c_str());
    //#endif

    vector<vector<int>> allMeldCard;
    vector<int>         allPosition;
    vector<int>         allMeldIndex;

    auto it = mapMeldCards.begin();
    while (it != mapMeldCards.end())
    {
        for (int index = 0; index < it->second.size(); index++)
        {
            //#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            //			CCLOG("RuleLayoffCard::suggest position: %d combCards: %s", position, SvrCard::debugCards(it->second.at(index)).c_str());
            //#endif
            allMeldCard.push_back(it->second.at(index));
            allPosition.push_back(it->first);
            allMeldIndex.push_back(index);
        }
        it++;
    }

    vector<RuleLayoffCard> result;

    // Trường hợp: sám chi.
    vector<int> tempCards = SvrCard::cloneCards(helperCards);
    while (!tempCards.empty())
    {
        const int card = tempCards.at(0);
        SvrCard::removeCard(card, tempCards);

        for (int i = 0; i < allMeldCard.size(); i++)
        {
            vector<int> meldCheck = SvrCard::cloneCards(allMeldCard.at(i));
            if (RuleCommon::isSameRank(meldCheck) && !SvrCard::containsCard(card, meldCheck))
            {
                meldCheck.push_back(card);
                if (RuleCommon::isMeld(meldCheck))
                {
                    bool add = false;
                    for (auto& x : result)
                    {
                        if (x.getPosition() == allPosition.at(i) && x.getMeldIndex() == allMeldIndex.at(i))
                        {
                            x.addCard(card);
                            add = true;
                            break;
                        }
                    }
                    if (!add)
                        result.push_back(RuleLayoffCard(senderIndex, allPosition.at(i), allMeldIndex.at(i), { card }));
                }
            }
        }
    }

    // Trường hợp: sảnh.
    tempCards = SvrCard::cloneCards(helperCards);
    for (int i = 0; i < allMeldCard.size(); i++)
    {
        vector<int> meldCheck = SvrCard::cloneCards(allMeldCard.at(i));
        if (RuleCommon::isStraightSuit(meldCheck))
        {
            bool        add            = false;
            vector<int> cloneTempCards = SvrCard::cloneCards(tempCards);
            while (!cloneTempCards.empty())
            {
                const int card = cloneTempCards.at(0);
                SvrCard::removeCard(card, cloneTempCards);

                vector<int> cloneMeldCheck = SvrCard::cloneCards(meldCheck);
                if (!SvrCard::containsCard(card, cloneMeldCheck))
                {
                    cloneMeldCheck.push_back(card);
                    if (RuleCommon::isMeld(cloneMeldCheck))
                    {
                        meldCheck.push_back(card);

                        cloneTempCards = SvrCard::cloneCards(tempCards);
                        SvrCard::removeCard(card, cloneTempCards);

                        for (auto& x : result)
                        {
                            if (x.getPosition() == allPosition.at(i) && x.getMeldIndex() == allMeldIndex.at(i))
                            {
                                x.addCard(card);
                                add = true;
                                break;
                            }
                        }
                        if (!add)
                            result.push_back(RuleLayoffCard(senderIndex, allPosition.at(i), allMeldIndex.at(i), { card }));
                    }
                }
            }
        }
    }

    //#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    //	for (RuleLayoffCard rgc : result)
    //		CCLOG("RuleLayoffCard::suggest position: %d cards: %s", rgc.position, SvrCard::debugCards(rgc.cards).c_str());
    //#endif

    if (result.size() >= 2)
    {
        sort(result.begin(), result.end(), [](const RuleLayoffCard& l1, const RuleLayoffCard& l2) {
            return l1.getCards().size() > l2.getCards().size();
        });
    }

    // remove duplicates
    if (result.size() >= 2)
    {
        sort(result.begin(), result.end(), [](const RuleLayoffCard& l1, const RuleLayoffCard& l2) {
            return l1.getCards().size() > l2.getCards().size();
        });

        vector<RuleLayoffCard> newResult;
        do
        {
            RuleLayoffCard rlc = result.back();
            result.pop_back();

            bool hasDuplicates = false;

            for (int i = result.size() - 1; i >= 0; i--)
            {
                for (const int& c : rlc.getCards())
                {
                    if (SvrCard::containsCard(c, result.at(i).getCards()))
                    {
                        hasDuplicates = true;
                        break;
                    }
                }
                if (hasDuplicates)
                    break;
            }
            if (!hasDuplicates)
                newResult.push_back(rlc);
        } while (result.size() >= 2);

        if (result.size() == 1)
            newResult.push_back(result.at(0));

        if (newResult.size() >= 2)
        {
            sort(newResult.begin(), newResult.end(), [](const RuleLayoffCard& l1, const RuleLayoffCard& l2) {
                return l1.getCards().size() > l2.getCards().size();
            });
        }
        return newResult;
    }
    return result;
}
