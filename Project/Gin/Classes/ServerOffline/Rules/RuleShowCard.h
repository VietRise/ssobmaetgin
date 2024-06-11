#ifndef __RULE_SHOW_CARD_H__
#define __RULE_SHOW_CARD_H__

#include <vector>

class RuleShowCard
{
public:
    static std::vector<std::vector<int>> suggest(const std::vector<int>& cards, const std::vector<int>& takeCards);
};

#endif // __RULE_SHOW_CARD_H__