#include "BotHelper.h"
#include "BotConfig.h"
#include "BotLogger.h"
#include "Helper/DebugHelper.h"
#include "Helper/GameUtils.h"
#include "ServerOffline/Rules/RuleCommon.h"
#include "ServerOffline/Rules/RuleTakeCard.h"
#include "ServerOffline/Rules/RuleKnocking.h"
#include "ServerOffline/Rules/RuleLayoffCard.h"
#include "ServerOffline/SvrCard.h"

USING_NS_CC;
using namespace std;

#if DEBUG_AI_BOT == 0
    #define DLOG_BOT(...) do {} while (0)
#else
    #define _DLOG_BOT_(i, s) LogHelper::bot(DebugHelper::player2Str(i), __FUNCTION__, s);
    #define _DLOG_BOT_0()                                               _DLOG_BOT_(-1, "")
    #define _DLOG_BOT_1(i)                                              _DLOG_BOT_(i, "")
    #define _DLOG_BOT_2(i, a1)                                          _DLOG_BOT_(i, _D_GET_VAR_1(a1))
    #define _DLOG_BOT_3(i, a1, a2)                                      _DLOG_BOT_(i, _D_GET_VAR_2(a1, a2))
    #define _DLOG_BOT_4(i, a1, a2, a3)                                  _DLOG_BOT_(i, _D_GET_VAR_3(a1, a2, a3))
    #define _DLOG_BOT_5(i, a1, a2, a3, a4)                              _DLOG_BOT_(i, _D_GET_VAR_4(a1, a2, a3, a4))
    #define _DLOG_BOT_6(i, a1, a2, a3, a4, a5)                          _DLOG_BOT_(i, _D_GET_VAR_5(a1, a2, a3, a4, a5))
    #define _DLOG_BOT_7(i, a1, a2, a3, a4, a5, a6)                      _DLOG_BOT_(i, _D_GET_VAR_6(a1, a2, a3, a4, a5, a6))
    #define _DLOG_BOT_8(i, a1, a2, a3, a4, a5, a6, a7)                  _DLOG_BOT_(i, _D_GET_VAR_7(a1, a2, a3, a4, a5, a6, a7))
    #define _DLOG_BOT_9(i, a1, a2, a3, a4, a5, a6, a7, a8)              _DLOG_BOT_(i, _D_GET_VAR_8(a1, a2, a3, a4, a5, a6, a7, a8))
    #define _DLOG_BOT_10(i, a1, a2, a3, a4, a5, a6, a7, a8, a9)         _DLOG_BOT_(i, _D_GET_VAR_9(a1, a2, a3, a4, a5, a6, a7, a8, a9))
    #define _DLOG_BOT_11(i, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)    _DLOG_BOT_(i, _D_GET_VAR_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))
    #define _DLOG_BOT_MACROS(i, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, N, ...) N
    #define DLOG_BOT(...) _DLOG_BOT_MACROS( , ##__VA_ARGS__, _DLOG_BOT_11, _DLOG_BOT_10, _DLOG_BOT_9, _DLOG_BOT_8, _DLOG_BOT_7, _DLOG_BOT_6, _DLOG_BOT_5, _DLOG_BOT_4, _DLOG_BOT_3, _DLOG_BOT_2, _DLOG_BOT_1, _DLOG_BOT_0, UNUSED)(__VA_ARGS__)
#endif

int BotHelper::s_lastTakeCard = -1;

int BotHelper::getLevel(const double& userWinRate, const int& userMatchCount, const double& userMoney)
{
    DLOG_BOT(-1, userWinRate, userMatchCount, userMoney);

    BotConfig botConf = BotConfig::loadConf();
    if (botConf.enable && userMatchCount >= botConf.startMatch)
    {
        BotVersion botVersion = BotConfig::getVersion(botConf, userWinRate, userMatchCount, userMoney);

        vector<int> percents;
        for (const auto& wr : botVersion.winRates)
        {
            if (userWinRate >= wr.ratio)
                percents = wr.percent;
            else
                break;
        }
        if (!percents.empty())
        {
            int      percent = 0;
            int      random  = GameUtils::randomIntInRange(0, 100);
            for (int level   = 0; level < percents.size(); level++)
            {
                percent += percents[level];
                if (percent == 0)
                    continue;
                if (percent == 100 || random <= percent)
                    return level;
            }
        }
        return botVersion.levelDef;
    }
    return (int) BotConstant::Level::LEVEL_0;
}

int BotHelper::getVersion()
{
    return BotConfig::s_currentVersion;
}

#if USE_CHEAT_CARD
void BotHelper::cheatVersion(const int& version)
{
    BotConfig::s_currentVersion = version;
}
#endif

int BotHelper::pass_0(const int& upCard, const RDPlayer& usr, const RDPlayer& bot)
{
    DLOG_BOT(bot.index, upCard);
    return RuleTakeCard::canTake(upCard, bot.cards) ? upCard : SvrCard::Index::CARD_NONE;
}

int BotHelper::pass_1(const int& upCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, const bool& isBeginTurn)
{
    DLOG_BOT(bot.index, upCard, isBeginTurn, remainCards);

    if (RuleTakeCard::canTake(upCard, bot.cards))
        return upCard;
    else
    {
        const int remainCard = remainCards.back();
        if (isBeginTurn && RuleTakeCard::canTake(remainCard, bot.cards))
            return SvrCard::Index::CARD_NONE;

        const int pointRemainCard = SvrCard::getPoints(remainCard);
        const int pointUpCard     = SvrCard::getPoints(upCard);
        if (pointUpCard < pointRemainCard)
            return upCard;
        else if (pointRemainCard <= pointUpCard)
            return SvrCard::Index::CARD_NONE;
    }
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::pass_2(const int& upCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, const bool& isBeginTurn)
{
    DLOG_BOT(bot.index, upCard, isBeginTurn, remainCards);

    if (RuleTakeCard::canTake(upCard, bot.cards))
    {
        const int remainCard = remainCards.back();
        if (isBeginTurn && RuleTakeCard::canTake(remainCard, bot.cards))
        {
            vector<int> handContainRemainCard = SvrCard::cloneCards(bot.cards);
            handContainRemainCard.push_back(remainCard);

            vector<int> handContainUpCard = SvrCard::cloneCards(bot.cards);
            handContainUpCard.push_back(upCard);

            if (RuleCommon::getResultInfo(handContainRemainCard).points <= RuleCommon::getResultInfo(handContainUpCard).points)
                return remainCard;
        }
        return upCard;
    }
    else
    {
        const int remainCard = remainCards.back();
        if (isBeginTurn && RuleTakeCard::canTake(remainCard, bot.cards))
            return SvrCard::Index::CARD_NONE;

        const int pointRemainCard = SvrCard::getPoints(remainCard);
        const int pointUpCard     = SvrCard::getPoints(upCard);
        if (pointUpCard < pointRemainCard)
            return upCard;
        else if (pointRemainCard <= pointUpCard)
            return SvrCard::Index::CARD_NONE;
    }
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::takeCard_0(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, int shuffleRemainCards)
{
    DLOG_BOT(bot.index, takeCard, shuffleRemainCards, remainCards);

    if (RuleTakeCard::canTake(takeCard, bot.cards))
        return takeCard;
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::takeCard_1(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, int shuffleRemainCards)
{
    DLOG_BOT(bot.index, takeCard, shuffleRemainCards, remainCards);

    if (RuleTakeCard::canTake(takeCard, bot.cards))
        return takeCard;
    else
    {
        const int remainCard = remainCards.back();
        if (RuleTakeCard::canTake(remainCard, bot.cards))
            return SvrCard::Index::CARD_NONE;

        if (shuffleRemainCards > 0)
        {
            SvrCard::shuffleCards(remainCards);
            DLOG_BOT(bot.index, shuffleRemainCards, remainCards);
            return takeCard_1(takeCard, usr, bot, remainCards, --shuffleRemainCards);
        }
        else
        {
            const int pointRemainCard = SvrCard::getPoints(remainCard);
            const int pointTakeCard   = SvrCard::getPoints(takeCard);
            if (pointRemainCard <= pointTakeCard)
                return SvrCard::Index::CARD_NONE;

            const int pointDeadwoodCard = SvrCard::getPoints(RuleCommon::getResultInfo(bot.cards).getHighestDeadwood());
            if (pointTakeCard < pointRemainCard && pointTakeCard < pointDeadwoodCard && takeCard != s_lastTakeCard)
                return takeCard;
        }
    }
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::takeCard_2(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, int shuffleRemainCards)
{
    DLOG_BOT(bot.index, takeCard, shuffleRemainCards, remainCards);

    if (RuleTakeCard::canTake(takeCard, bot.cards))
    {
        const int remainCard = remainCards.back();
        if (RuleTakeCard::canTake(remainCard, bot.cards))
        {
            vector<int> handContainRemainCard = SvrCard::cloneCards(bot.cards);
            vector<int> handContainTakeCard   = SvrCard::cloneCards(bot.cards);

            handContainRemainCard.push_back(remainCard);
            handContainTakeCard.push_back(takeCard);

            if (RuleCommon::getResultInfo(handContainRemainCard).points < RuleCommon::getResultInfo(handContainTakeCard).points)
                return remainCard;
        }
        return takeCard;
    }

    const int remainCard = remainCards.back();
    if (RuleTakeCard::canTake(remainCard, bot.cards))
        return SvrCard::Index::CARD_NONE;

    if (shuffleRemainCards > 0)
    {
        SvrCard::shuffleCards(remainCards);
        DLOG_BOT(bot.index, shuffleRemainCards, remainCards);
        return takeCard_2(takeCard, usr, bot, remainCards, --shuffleRemainCards);
    }
    else
    {
        const int pointRemainCard = SvrCard::getPoints(remainCard);
        const int pointTakeCard   = SvrCard::getPoints(takeCard);
        if (pointRemainCard <= pointTakeCard)
            return SvrCard::Index::CARD_NONE;

        const int pointDeadwoodCard = SvrCard::getPoints(RuleCommon::getResultInfo(bot.cards).getHighestDeadwood());
        if (pointTakeCard < pointRemainCard && pointTakeCard < pointDeadwoodCard && takeCard != s_lastTakeCard)
            return takeCard;
    }
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::takeCard_3(const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, int shuffleRemainCards)
{
    DLOG_BOT(bot.index, takeCard, shuffleRemainCards, remainCards);

    if (RuleTakeCard::canTake(takeCard, bot.cards))
    {
        const int remainCard = remainCards.back();
        if (RuleTakeCard::canTake(remainCard, bot.cards))
        {
            vector<int> handContainRemainCard = SvrCard::cloneCards(bot.cards);
            vector<int> handContainTakeCard   = SvrCard::cloneCards(bot.cards);

            handContainRemainCard.push_back(remainCard);
            handContainTakeCard.push_back(takeCard);

            if (RuleCommon::getResultInfo(handContainRemainCard).points < RuleCommon::getResultInfo(handContainTakeCard).points)
                return remainCard;
        }
        return takeCard;
    }

    const int remainCard = remainCards.back();
    if (RuleTakeCard::canTake(remainCard, bot.cards))
        return SvrCard::Index::CARD_NONE;

    if (shuffleRemainCards > 0)
    {
        SvrCard::shuffleCards(remainCards);
        DLOG_BOT(bot.index, shuffleRemainCards, remainCards);
        return takeCard_3(takeCard, usr, bot, remainCards, --shuffleRemainCards);
    }
    else
    {
        const int pointRemainCard = SvrCard::getPoints(remainCard);
        const int pointTakeCard   = SvrCard::getPoints(takeCard);
        if (pointRemainCard <= pointTakeCard)
            return SvrCard::Index::CARD_NONE;

        const int pointDeadwoodCard = SvrCard::getPoints(RuleCommon::getResultInfo(bot.cards).getHighestDeadwood());
        if (pointTakeCard < pointRemainCard && pointTakeCard < pointDeadwoodCard && takeCard != s_lastTakeCard)
            return takeCard;
    }
    return SvrCard::Index::CARD_NONE;
}

int BotHelper::throwCard_0(const RDPlayer& usr, const RDPlayer& bot)
{
    DLOG_BOT(bot.index);

    int deadwoodCard = RuleCommon::getResultInfo(bot.cards).getHighestDeadwood();
    if (SvrCard::isValidIndex(deadwoodCard))
        return deadwoodCard;
    return SvrCard::getMaxIndex(bot.cards);
}

int BotHelper::throwCard_1(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule)
{
    DLOG_BOT(bot.index, (int) gameRule);

    if (gameRule != RuleConstant::GameRule::STRAIGHT_GIN)
    {
        map<int, vector<vector<int>>> mapMelds;
        mapMelds[usr.index] = RuleCommon::getResultInfo(usr.cards).melds;
        if (!mapMelds.empty())
        {
            vector<int> deadwoodCards = SvrCard::sortDesc(RuleCommon::getResultInfo(bot.cards).deadwood);
            vector<int> layoffCards;
            for (const auto& rlc : RuleLayoffCard::suggest(deadwoodCards, mapMelds, bot.index))
                for (const int& card : rlc.getCards())
                    if (!SvrCard::containsCard(card, layoffCards))
                        layoffCards.push_back(card);

            if (!layoffCards.empty())
            {
                for (const int& deadwood : deadwoodCards)
                    if (!SvrCard::containsCard(deadwood, layoffCards))
                        return deadwood;
            }
        }
    }

    int deadwoodCard = RuleCommon::getResultInfo(bot.cards).getHighestDeadwood();
    if (SvrCard::isValidIndex(deadwoodCard))
        return deadwoodCard;
    return SvrCard::getMaxIndex(bot.cards);
}

int BotHelper::throwCard_2(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const vector<int>& pileDiscards)
{
    DLOG_BOT(bot.index, (int) gameRule, pileDiscards);
    return throwCard_n(usr, bot, gameRule, pileDiscards);
}

int BotHelper::throwCard_n(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const vector<int>& pileDiscards)
{
    DLOG_BOT(bot.index, (int) gameRule, pileDiscards);

    vector<int> layoffCards;
    const auto& botResult = RuleCommon::getResultInfo(bot.cards);
    vector<int> deadwoodCards = SvrCard::sortDesc(botResult.deadwood);
    if (gameRule != RuleConstant::GameRule::STRAIGHT_GIN)
    {
        map<int, vector<vector<int>>> mapMelds;
        mapMelds[usr.index] = RuleCommon::getResultInfo(usr.cards).melds;
        if (!mapMelds.empty())
        {
            for (const auto& rlc : RuleLayoffCard::suggest(deadwoodCards, mapMelds, bot.index))
                for (const int& card : rlc.getCards())
                    if (!SvrCard::containsCard(card, layoffCards))
                        layoffCards.push_back(card);
        }
    }

    BotOpponentHandEstimation ohe = userHandEstimation(usr, bot, pileDiscards);
    const vector<int>& safeCards = ohe.getSafeCards();
    if (!safeCards.empty())
    {
        for (const int& deadwoodCard : deadwoodCards)
        {
            if (SvrCard::containsCard(deadwoodCard, safeCards))
            {
#if DEBUG_AI_BOT
                LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is safe card !!!");
#endif
                if (!SvrCard::containsCard(deadwoodCard, layoffCards))
                {
#if DEBUG_AI_BOT
                    LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is not layoff card !!!");
#endif
                    return deadwoodCard;
                }
            }
        }
    }

    const vector<int>& notSafeCards = ohe.getNotSafeCards();
    if (!notSafeCards.empty())
    {
        for (const int& deadwoodCard : deadwoodCards)
        {
            if (!SvrCard::containsCard(deadwoodCard, notSafeCards))
            {
                if (ohe.getPossibleMelds().empty())
                {
#if DEBUG_AI_BOT
                    LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is maybe safe card !!!");
#endif
                    if (!SvrCard::containsCard(deadwoodCard, layoffCards))
                    {
#if DEBUG_AI_BOT
                        LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is not layoff card !!!");
#endif
                        return deadwoodCard;
                    }
                }
                else
                {
                    bool notfound = true;
                    for (const auto& meld : ohe.getPossibleMelds())
                    {
                        if (!SvrCard::containsCard(deadwoodCard, meld) && RuleCommon::isMeld(SvrCard::mergeCard(meld, deadwoodCard)))
                        {
                            notfound = false;
                            break;
                        }
                    }
                    if (notfound)
                    {
#if DEBUG_AI_BOT
                        LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is maybe safe card !!!");
#endif
                        if (!SvrCard::containsCard(deadwoodCard, layoffCards))
                        {
#if DEBUG_AI_BOT
                            LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is not layoff card !!!");
#endif
                            return deadwoodCard;
                        }
                    }
                }
            }
#if DEBUG_AI_BOT
            else
                LogHelper::bot(__FUNCTION__, "Card " + DebugHelper::cards2Str({ deadwoodCard }) + " is not safe card !!!");
#endif
        }
    }

    if (!layoffCards.empty())
    {
        for (const int& deadwood : deadwoodCards)
            if (!SvrCard::containsCard(deadwood, layoffCards))
                return deadwood;
    }

    int deadwoodCard = botResult.getHighestDeadwood();
    if (!SvrCard::isValidIndex(deadwoodCard))
        deadwoodCard = SvrCard::getMaxIndex(bot.cards);
    return deadwoodCard;
}

int BotHelper::knock_0(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints)
{
    DLOG_BOT(bot.index, (int) gameRule, knockPoints);
    return knock_n(usr, bot, gameRule, knockPoints, BotConstant::Level::LEVEL_0);
}

int BotHelper::knock_1(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints)
{
    DLOG_BOT(bot.index, (int) gameRule, knockPoints);
    return knock_n(usr, bot, gameRule, knockPoints, BotConstant::Level::LEVEL_1);
}

int BotHelper::knock_2(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints)
{
    DLOG_BOT(bot.index, (int) gameRule, knockPoints);
    return knock_n(usr, bot, gameRule, knockPoints, BotConstant::Level::LEVEL_2);
}

int BotHelper::knock_3(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints)
{
    DLOG_BOT(bot.index, (int) gameRule, knockPoints);;
    return knock_n(usr, bot, gameRule, knockPoints, BotConstant::Level::LEVEL_3);
}

int BotHelper::knock_n(const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints, BotConstant::Level level)
{
    DLOG_BOT(bot.index, (int) gameRule, knockPoints, (int) level);

    if (gameRule == RuleConstant::GameRule::CLASSIC_GIN)
    {
        const auto& result = RuleCommon::getResultInfo(bot.cards);
        if (RuleKnocking::canKnock(knockPoints, result.knockPoint))
        {
#if USE_BOT_INSTANT_KNOCK == 0
            vector<int> classicKnock = BotConfig::getClassicKnock((int) level);
            if (result.knockPoint < classicKnock.size())
            {
                const int random  = GameUtils::randomIntInRange(0, 100);
                const int percent = classicKnock.at(result.knockPoint);

                DLOG_BOT(bot.index, random, percent);

                if (random <= percent)
                    return result.getHighestDeadwood();
            }
#else
            return result.getHighestDeadwoodCard();
#endif
        }
    }
    else if (gameRule == RuleConstant::GameRule::OKLAHOMA_GIN)
    {
        const auto& result = RuleCommon::getResultInfo(bot.cards);
        if (RuleKnocking::canKnock(knockPoints, result.knockPoint))
        {
#if USE_BOT_INSTANT_KNOCK == 0
            vector<vector<int>> oklahomaKnock = BotConfig::getOklahomaKnock((int) level);
            if (knockPoints < oklahomaKnock.size())
            {
                const vector<int>& v = oklahomaKnock.at(knockPoints);
                if (result.knockPoint < v.size())
                {
                    const int random  = GameUtils::randomIntInRange(0, 100);
                    const int percent = v.at(result.knockPoint);

                    DLOG_BOT(bot.index, random, percent);

                    if (random <= percent)
                        return result.getHighestDeadwood();
                }
            }
#else
            return result.getHighestDeadwoodCard();
#endif
        }
    }
    return SvrCard::Index::CARD_NONE;
}

BotOpponentHandEstimation BotHelper::userHandEstimation(const RDPlayer& user, const RDPlayer& bot, const vector<int>& pileDiscards)
{
    BotOpponentHandEstimation ohe = BotOpponentHandEstimation(bot.cards, bot.throwCards, user.takeCards, user.throwCards, pileDiscards);
    DLOG_BOT(bot.index, SvrCard::sortAsc(ohe.getSafeCards()), SvrCard::sortAsc(ohe.getNotSafeCards()));
    return ohe;
}

void BotHelper::resetData()
{
    s_lastTakeCard = SvrCard::Index::CARD_NONE;
}

int BotHelper::pass(const int& level, const int& upCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards, const bool& isBeginTurn)
{
    DLOG_BOT(bot.index, level, upCard, isBeginTurn, remainCards);

#if USE_BOT_INSTANT_PASS == 0
    switch ((BotConstant::Level) level)
    {
        case BotConstant::Level::LEVEL_1:return pass_1(upCard, usr, bot, remainCards, isBeginTurn);
        case BotConstant::Level::LEVEL_2:return pass_2(upCard, usr, bot, remainCards, isBeginTurn);
        default:return pass_0(upCard, usr, bot);
    }
#else
    return SvrCard::Index::CARD_NONE;
#endif
}

int BotHelper::takeCard(const int& level, const int& takeCard, const RDPlayer& usr, const RDPlayer& bot, vector<int>& remainCards)
{
    DLOG_BOT(bot.index, level, takeCard, remainCards);

    int result = SvrCard::Index::CARD_NONE;
#if USE_BOT_TAKE_CARD
    if (SvrCard::isValidIndex(takeCard))
    {
        switch ((BotConstant::Level) level)
        {
            case BotConstant::Level::LEVEL_1:result = takeCard_1(takeCard, usr, bot, remainCards, BotConfig::getShuffleRemainCards(level));
                break;
            case BotConstant::Level::LEVEL_2:result = takeCard_2(takeCard, usr, bot, remainCards, BotConfig::getShuffleRemainCards(level));
                break;
            case BotConstant::Level::LEVEL_3:result = takeCard_3(takeCard, usr, bot, remainCards, BotConfig::getShuffleRemainCards(level));
                break;
            default:result = takeCard_0(takeCard, usr, bot, remainCards, BotConfig::getShuffleRemainCards(level));
        }
        if (SvrCard::isValidIndex(result))
            s_lastTakeCard = result;
    }
#endif
    return result;
}

int BotHelper::throwCard(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const vector<int>& pileDiscards)
{
    DLOG_BOT(bot.index, level, (int) gameRule, pileDiscards);

    switch ((BotConstant::Level) level)
    {
        case BotConstant::Level::LEVEL_1:return throwCard_1(usr, bot, gameRule);
        case BotConstant::Level::LEVEL_2:return throwCard_2(usr, bot, gameRule, pileDiscards);
        default:return throwCard_0(usr, bot);
    }
}

int BotHelper::knock(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule, const int& knockPoints)
{
    DLOG_BOT(bot.index, level, (int) gameRule, knockPoints);

#if USE_BOT_KNOCK == 1
    int knockCard;
    switch ((BotConstant::Level) level)
    {
        case BotConstant::Level::LEVEL_1:knockCard = knock_1(usr, bot, gameRule, knockPoints);
            break;
        case BotConstant::Level::LEVEL_2:knockCard = knock_2(usr, bot, gameRule, knockPoints);
            break;
        case BotConstant::Level::LEVEL_3:knockCard = knock_3(usr, bot, gameRule, knockPoints);
            break;
        default:knockCard = knock_0(usr, bot, gameRule, knockPoints);
    }
#if DEBUG_LOG_BOT
    if (SvrCard::isValidIndex(knockCard))
    {
        BotLogger::logCount(BotLogger::Event::ACTION_KNOCK);
        BotLogger::logScore(BotLogger::Event::ACTION_KNOCK, RuleCommon::getResultInfo(bot.cards).knockPoint);
    }
#endif
    return knockCard;
#else
    return SvrCard::Index::CARD_NONE;
#endif
}

bool BotHelper::gin(const int& level, const RDPlayer& usr, const RDPlayer& bot, const RuleConstant::GameRule& gameRule)
{
    DLOG_BOT(bot.index, level, (int) gameRule);

#if USE_BOT_GIN
    #if DEBUG_LOG_BOT
    BotLogger::logCount(BotLogger::Event::ACTION_GIN);
    #endif
    return true;
#else
    return false;
#endif
}

bool BotHelper::bigGin(const int& level, const RDPlayer& usr, const RDPlayer& bot)
{
    DLOG_BOT(bot.index, level);

#if USE_BOT_BIG_GIN
    #if DEBUG_LOG_BOT
    BotLogger::logCount(BotLogger::Event::ACTION_BIG_GIN);
    #endif
    return true;
#else
    return false;
#endif
}