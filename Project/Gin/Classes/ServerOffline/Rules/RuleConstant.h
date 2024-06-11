#ifndef __RULE_CONSTANT_H__
#define __RULE_CONSTANT_H__

#include <vector>

class RuleConstant
{
public:
    enum class GameRule
    {
        CLASSIC_GIN  = 1,
        STRAIGHT_GIN = 2,
        OKLAHOMA_GIN = 3
    };

    class Common
    {
    public:
        static const int MAX_HAND_CARD        = 10;
        static const int MIN_NUM_CARD_ON_MELD = 3;
        static const int KNOCK_POINTS         = 10;
    };

    enum class ResultType
    {
        NONE          = -1,
        POINT         = 0,
        WIN_KNOCK     = 1,
        WIN_GIN       = 2,
        WIN_BIG_GIN   = 3,
        LOSE_KNOCK    = 4,
        LOSE_GIN      = 5,
        LOSE_BIG_GIN  = 6,
        DRAW          = 7,
        WIN_UNDERCUT  = 8,
        LOSE_UNDERCUT = 9
    };

    enum class ActionType
    {
        NONE                = -1,
        NOT_ENOUGH_TO_KNOCK = 0,
        KNOCK               = 1,
        GIN                 = 2,
        BIG_GIN             = 3,
        UNDERCUT            = 4
    };
};

#endif // __RULE_CONSTANT_H__