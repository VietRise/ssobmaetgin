#ifndef __RULE_KNOCKING_H__
#define __RULE_KNOCKING_H__

#include <vector>

class RuleKnocking
{
public:
    static bool canKnock(const int& knockPoints, const int& handPoints);
    static bool canKnock(const int& knockPoints, const std::vector<int>& cards);
};

#endif // __RULE_KNOCKING_H__