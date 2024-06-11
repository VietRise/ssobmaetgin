#ifndef __RULE_LAYOFF_CARD_H__
#define __RULE_LAYOFF_CARD_H__

#include <map>
#include <vector>

class RuleLayoffCard
{
private:
    int              _senderIndex;
    int              _position;
    std::vector<int> _cards;
    int              _meldIndex;
public:
    ~RuleLayoffCard();
    RuleLayoffCard();
    RuleLayoffCard(int senderIndex, int position, int meldIndex, const std::vector<int>& cards);

    int getPosition() const;
    void addCard(const int& card);
    void addCard(const std::vector<int>& cards);
    void setCard(const int& card);
    void setCard(const std::vector<int>& cards);
    const std::vector<int>& getCards() const;
    int getMeldIndex() const;
    int getSenderIndex() const;

    static std::vector<RuleLayoffCard> suggest(const std::vector<int>& cards, const std::map<int, std::vector<std::vector<int>>>& mapMeldCards, int senderIndex);
};

#endif // __RULE_LAYOFF_CARD_H__