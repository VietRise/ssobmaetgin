#ifndef __INGAME_DEFINE_H__
#define __INGAME_DEFINE_H__

#include "cocos2d.h"
#include <vector>
#include "Define/GameDefine.h"
#include "ServerOffline/Rules/RuleConstant.h"

#if BUILD_RELEASE // Release
    #define DEBUG_LOG_BOT 0
    #define DEBUG_DEAL_CARD 0
    #define DEBUG_AI_BOT 0

    #define USE_BOT_TAKE_CARD 1            // 0 is OFF. Default is 1.
    #define USE_BOT_KNOCK 1                // 0 is OFF. Default is 1.
    #define USE_BOT_GIN 1                // 0 is OFF. Default is 1.
    #define USE_BOT_BIG_GIN 1            // 0 is OFF. Default is 1.
    #define USE_BOT_INSTANT_PASS 0        // 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_KNOCK 0        // 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_GIN 0        // 0 is OFF. Default is 0.

#else // Debug
    #define DEBUG_LOG_BOT 0
    #define DEBUG_DEAL_CARD 0
    #define DEBUG_AI_BOT 0

    #define USE_BOT_TAKE_CARD 1			// 0 is OFF. Default is 1.
    #define USE_BOT_KNOCK 1				// 0 is OFF. Default is 1.
    #define USE_BOT_GIN 1				// 0 is OFF. Default is 1.
    #define USE_BOT_BIG_GIN 1			// 0 is OFF. Default is 1.
    #define USE_BOT_INSTANT_PASS 0	    // 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_KNOCK 0		// 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_GIN 0		// 0 is OFF. Default is 0.

#endif

#define DEFAULT_PACKAGE_NAME "com.emagssob.gin"

#define INDEX_NONE -1
#define INDEX_PLAYER 0

typedef struct PointGiveServerDefine
{
    std::vector<int> card;
    int              slot;
    int              phomOrder;
} PointGiveServerDefine;

enum class Suit
{
    SUIT_SPADES   = 0,
    SUIT_HEARTS   = 1,
    SUIT_CLUBS    = 2,
    SUIT_DIAMONDS = 3
};

enum class Rank
{
    RANK_A  = 0,
    RANK_2  = 1,
    RANK_3  = 2,
    RANK_4  = 3,
    RANK_5  = 4,
    RANK_6  = 5,
    RANK_7  = 6,
    RANK_8  = 7,
    RANK_9  = 8,
    RANK_10 = 9,
    RANK_J  = 10,
    RANK_Q  = 11,
    RANK_K  = 12
};

enum class FightStatus
{
    NOT_ALLOW    = 0,
    ALLOW        = 1,
    WAIT_CONFIRM = 2,
    KNOCK        = 3,
    UNDERCUT     = 4,
    PASS         = 5
};

typedef enum
{
    MATCH_STATUS_READY          = 1,
    GIN_MATCH_STATUS_DEAL_CARD  = 2,
    GIN_MATCH_STATUS_PASS_CARD  = 3,
    GIN_MATCH_STATUS_THROW_CARD = 4,
    GIN_MATCH_STATUS_TAKE_CARD  = 5,
    GIN_MATCH_STATUS_DRAW_CARD  = 6,
    GIN_MATCH_STATUS_FINISH     = 10
} GinMatchStatus;

typedef enum
{
    GIN_RESULT_TYPE_NONE          = -1,
    GIN_RESULT_TYPE_SCORE         = 0,
    GIN_RESULT_TYPE_WIN_KNOCK     = 1,
    GIN_RESULT_TYPE_WIN_GIN       = 2,
    GIN_RESULT_TYPE_WIN_BIG_GIN   = 3,
    GIN_RESULT_TYPE_LOSE_KNOCK    = 4,
    GIN_RESULT_TYPE_LOSE_GIN      = 5,
    GIN_RESULT_TYPE_LOSE_BIG_GIN  = 6,
    GIN_RESULT_TYPE_DRAW          = 7,
    GIN_RESULT_TYPE_WIN_UNDERCUT  = 8,
    GIN_RESULT_TYPE_LOSE_UNDERCUT = 9
} GinResultType;

typedef enum
{
    GIN_MATCH_END_STATUS_NONE          = -1,
    GIN_MATCH_END_STATUS_KNOCK         = 0,
    GIN_MATCH_END_STATUS_UNDERCUT      = 1,
    GIN_MATCH_END_STATUS_GIN           = 2,
    GIN_MATCH_END_STATUS_BIGGIN        = 3,
    GIN_MATCH_END_STATUS_DRAW          = 4,
    GIN_MATCH_END_STATUS_SPADES_DOUBLE = 5,
    GIN_MATCH_END_STATUS_GIN_25        = 6,
    GIN_MATCH_END_STATUS_BIGGIN_50     = 7,
} GinMatchEndStatus;

typedef enum
{
    K_NONE        = -1,
    K_NOT_ALLOWED = 0,
    K_KNOCK       = 1,
    K_GIN         = 2,
    K_BIG_GIN     = 3
} ActionType;

typedef struct CardInHand
{
    std::vector<std::vector<int>> _phom;
    std::vector<int>              _trash;

} CardInHand;

typedef struct ListOverlap
{
    std::vector<std::vector<int>> _phom;
    bool                          _overlap;

} ListOverlap;

typedef struct
{
    int    _handPoints;
    int    _targetPoints;
    int    _winPoints;
    double _normalWinMoney;
    int    _scoreGin;
    int    _scoreBigGin;
    int    _scoreUnderCut;
    int    _scoreSpadesDouble;
    double _jackpotWinMoney;
    double _bonusWinMoney;
} PlayerResultDetailData;

typedef struct
{
    int                           _playerIndex;
    bool                          _isUser;
    double                        _money;
    std::vector<int>              _handCards;
    std::vector<int>              _throwCards;
    std::vector<int>              _takeCards;
    std::vector<std::vector<int>> _listCombinationPhom;
    int                           _level;
    int                           _levelExp;
    int                           _nextExp;
} PlayerCardData;

typedef struct
{
    int                                _playerIndex;
    std::string                        _userName;
    std::string                        _avatarLink;
    bool                               _isUser;
    double                             _money;
    int                                _level;
    int                                _levelExp;
    int                                _levelExpNext;
    double                             _rewardMoney;
    int                                _winOrder;
    int                                _type;
    std::vector<std::vector<int>>      _listPhom;
    std::vector<int>                   _remainCards;
    std::vector<PointGiveServerDefine> _listGiveCards;
    PlayerResultDetailData             _resultDetail;
} PlayerResultData;

typedef struct
{
    int  _slotIdx;
    bool _isUser;
    int  _score;
    int  _bet;
} BetData;

typedef struct
{
    int                  _targetScore;
    float                _jackpotBetMultiply;
    std::map<int, float> _multiplyDataMap;
} TargetScoreData;

enum class HintDropPlaceData
{
    VISIBLE,
    LISTCARD,
    SLOT,
    MELD
};

enum class CityType
{
    CLASSIC_GIN  = (int) RuleConstant::GameRule::CLASSIC_GIN,
    STRAIGHT_GIN = (int) RuleConstant::GameRule::STRAIGHT_GIN,
    OKLAHOMA_GIN = (int) RuleConstant::GameRule::OKLAHOMA_GIN
};

enum class TableType
{
    TABLE_1 = 1,
    TABLE_2 = 2,
    TABLE_3 = 3,
    TABLE_4 = 4,
    TABLE_5 = 5,
    TABLE_6 = 6,
    TABLE_7 = 7,
    TABLE_8 = 8,
    TABLE_9 = 9
};

enum class HintDropPlaceType
{
    HintDropPlaceNone             = -1,
    HintDropPlaceAtThrowCardsArea = 11
};

typedef struct
{
    std::string _name;
    double      _min;
    double      _max;
} MoneyRangeDefine;

class SvrMultiplyBonus
{
public:
    float ratio;
    float chance;
};

class GameMatchBundle
{
public:
    int                           cityId;
    int                           tableId;
    double                        price;
    int                           numPlayer;
    int                           numBot;
    std::vector<SvrMultiplyBonus> bonus;
    float                         jackpotRatioBet;
    int                           targetScore;
};

#define USER_MINIMUM_MONEY 1000
#define MAXIMUM_NUMBER_CARD 52
#define MAXIMUM_NUMBER_SUIT 4
#define MAXIMUM_NUMBER_SLOT_GAME 2
#define SECRET_MELD_SIZE 3

#define MAIN_USER_SLOT_INDEX 0
#define BOT_SLOT_INDEX 1
#define HINT_DISTANCE 18

#define RESOLUTION_1_6_SCALE 0.83f
#define RESOLUTION_1_4_SCALE 0.74f

#define COLOR_MONEY cocos2d::Color3B(255, 200, 103)
#define COLOR_WIN_3 cocos2d::Color3B(255, 193, 0)
#define COLOR_LOSE_3 cocos2d::Color3B(210, 210, 210)
#define COLOR_POSITIVE_NUMBER_3 cocos2d::Color3B(255, 211, 30)
#define COLOR_NAGATIVE_NUMBER_3 cocos2d::Color3B(229, 229, 229)
#define TIME_MOVE_ANIMATION_PER_PIXEL 1.5f/1000

extern int                                                                 s_winHitpotNumber;
extern std::map<int, std::map<int, std::map<double, std::vector<double>>>> s_betRange;
extern std::vector<std::string>                                            s_strNum;
extern std::vector<MoneyRangeDefine>                                       s_moneyRangeName;
extern std::map<int, std::map<int, std::vector<TargetScoreData>>>          s_targetScoreDataRange;

#define NUMBER_SEPERATE "."

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    #define DEBUG_LOG_BOT 1
    #define DEBUG_DEAL_CARD 1
    #define DEBUG_AI_BOT 0
    #define USE_BOT_TAKE_CARD 1     // 0 is OFF. Default is 1.
    #define USE_BOT_KNOCK 1         // 0 is OFF. Default is 1.
    #define USE_BOT_GIN 1           // 0 is OFF. Default is 1.
    #define USE_BOT_BIG_GIN 1       // 0 is OFF. Default is 1.
    #define USE_BOT_INSTANT_PASS 0  // 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_KNOCK 0 // 0 is OFF. Default is 0.
    #define USE_BOT_INSTANT_GIN 0   // 0 is OFF. Default is 0.
#endif

#if STRESS_TEST == 1
    #define DEBUG_LOG_BOT 0
#endif

#if DEBUG_LOG_BOT == 0
    #define DEBUG_AI_BOT 0
#endif

#if USE_AUTO_PLAY_CARD == 1
    #define DEBUG_AI_BOT 0
#endif

#if DEBUG_LOG_BOT || DEBUG_DEAL_CARD
// START: D_GET_CARD
    #define _D_GET_CARD_1(a1) StringHelper::joinArgsDelimiter(":", #a1, SvrCard::debugCards(a1))
    #define _D_GET_CARD_2(a1, a2)                                    _D_CONCAT_VAR(_D_GET_CARD_1(a1),                                  _D_GET_CARD_1(a2))
    #define _D_GET_CARD_3(a1, a2, a3)                                _D_CONCAT_VAR(_D_GET_CARD_2(a1, a2),                              _D_GET_CARD_1(a3))
    #define _D_GET_CARD_4(a1, a2, a3, a4)                            _D_CONCAT_VAR(_D_GET_CARD_3(a1, a2, a3),                          _D_GET_CARD_1(a4))
    #define _D_GET_CARD_5(a1, a2, a3, a4, a5)                        _D_CONCAT_VAR(_D_GET_CARD_4(a1, a2, a3, a4),                      _D_GET_CARD_1(a5))
    #define _D_GET_CARD_6(a1, a2, a3, a4, a5, a6)                    _D_CONCAT_VAR(_D_GET_CARD_5(a1, a2, a3, a4, a5),                  _D_GET_CARD_1(a6))
    #define _D_GET_CARD_7(a1, a2, a3, a4, a5, a6, a7)                _D_CONCAT_VAR(_D_GET_CARD_6(a1, a2, a3, a4, a5, a6),              _D_GET_CARD_1(a7))
    #define _D_GET_CARD_8(a1, a2, a3, a4, a5, a6, a7, a8)            _D_CONCAT_VAR(_D_GET_CARD_7(a1, a2, a3, a4, a5, a6, a7),          _D_GET_CARD_1(a8))
    #define _D_GET_CARD_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)        _D_CONCAT_VAR(_D_GET_CARD_8(a1, a2, a3, a4, a5, a6, a7, a8),      _D_GET_CARD_1(a9))
    #define _D_GET_CARD_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)  _D_CONCAT_VAR(_D_GET_CARD_9(a1, a2, a3, a4, a5, a6, a7, a8, a9),  _D_GET_CARD_1(a10))
    #define _D_GET_CARD_MACROS(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, N, ...) N
    #define D_GET_CARD(...) _D_GET_CARD_MACROS(__VA_ARGS__, _D_GET_CARD_10, _D_GET_CARD_9, _D_GET_CARD_8, _D_GET_CARD_7, _D_GET_CARD_6, _D_GET_CARD_5, _D_GET_CARD_4, _D_GET_CARD_3, _D_GET_CARD_2, _D_GET_CARD_1, UNUSED)(__VA_ARGS__)
// END: D_GET_CARD
#endif

#endif //__INGAME_DEFINE_H__
