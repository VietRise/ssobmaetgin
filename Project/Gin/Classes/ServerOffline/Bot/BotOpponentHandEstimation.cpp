#include "BotOpponentHandEstimation.h"
#include "Helper/ConsoleTable.h"
#include "Helper/DebugHelper.h"
#include "Helper/LogHelper.h"
#include "ServerOffline/SvrCard.h"
#include "ServerOffline/Rules/RuleConstant.h"

using namespace std;

BotOpponentHandEstimation::BotOpponentHandEstimation(const std::vector<int>& playerHandCards,
                                                     const std::vector<int>& playerDiscards,
                                                     const std::vector<int>& opponentTakeCards,
                                                     const std::vector<int>& opponentDiscards,
                                                     const std::vector<int>& pileDiscards)
{
#if DEBUG_AI_BOT
    LogHelper::botArgs("", __FUNCTION__, D_GET_VAR(playerHandCards, playerDiscards, opponentTakeCards, opponentDiscards, pileDiscards));
#endif
    this->_playerHandCards   = playerHandCards;
    this->_playerDiscards    = playerDiscards;
    this->_opponentTakeCards = opponentTakeCards;
    this->_opponentDiscards  = opponentDiscards;
    this->_discardPile       = pileDiscards;

    vector<int> pileDiscardsNotInOpponentDiscards;
    for (const int& discard : pileDiscards)
        if (!SvrCard::containsCard(discard, opponentDiscards))
            pileDiscardsNotInOpponentDiscards.push_back(discard);

    initIndexesByState();
    initRanksByState();
    initSuits();

    updateState(STATE_IN_PLAYER_HAND, playerHandCards);
    updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, pileDiscards);
    updateState(STATE_IN_OPPONENT_HAND, opponentTakeCards);

    estimateCardOpponentNotCare(STATE_UNKNOWN_CARD, true);
    estimateCardOpponentNotCare(STATE_IN_PLAYER_HAND, false);
    estimateCardOpponentCare(opponentTakeCards);
#if DEBUG_AI_BOT
    logConsoleTable();
#endif
}

vector<int> BotOpponentHandEstimation::getSafeCards()
{
    vector<int> safeCards;
    for (int    card = 0; card < SvrCard::Common::MAX_CARD; card++)
        if (this->_indexesByState.at(card) == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && !SvrCard::containsCard(card, this->_discardPile))
            safeCards.push_back(card);
    return safeCards;
}

vector<int> BotOpponentHandEstimation::getNotSafeCards()
{
    vector<int> notSafeCards;
    for (const vector<int>& meld : this->_possibleMelds)
        for (const int& card : meld)
            if (!SvrCard::containsCard(card, notSafeCards))
                notSafeCards.push_back(card);
    return notSafeCards;
}

vector<vector<int>> BotOpponentHandEstimation::getPossibleMelds()
{
    return this->_possibleMelds;
}

void BotOpponentHandEstimation::initIndexesByState()
{
    for (int index = 0; index < SvrCard::Common::MAX_CARD; index++)
        this->_indexesByState.push_back(State::STATE_UNKNOWN_CARD);
}

void BotOpponentHandEstimation::initRanksByState()
{
    map<State, int> mapRanksByState;
    mapRanksByState[STATE_IN_PLAYER_HAND]                       = 0;
    mapRanksByState[STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND] = 0;
    mapRanksByState[STATE_UNKNOWN_CARD]                         = 0;
    mapRanksByState[STATE_IN_OPPONENT_HAND]                     = 0;

    for (int rank = 0; rank < SvrCard::Common::MAX_RANK; rank++)
        this->_ranksByState.push_back(mapRanksByState);
}

void BotOpponentHandEstimation::initSuits()
{
    for (int suit = 0; suit < SvrCard::Common::MAX_SUIT; suit++)
        this->_suits.push_back(0);
}

void BotOpponentHandEstimation::resetIndexes()
{
    this->_indexesByState.clear();
    initIndexesByState();
}

void BotOpponentHandEstimation::resetRanks()
{
    this->_ranksByState.clear();
    initRanksByState();
}

void BotOpponentHandEstimation::resetSuits()
{
    this->_suits.clear();
    initSuits();
}

int BotOpponentHandEstimation::getCountRankOfSafeDiscards(int rank)
{
    return this->_ranksByState.at(rank).at(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND);
}

void BotOpponentHandEstimation::estimateCardOpponentCare(const vector<int>& takeCards)
{
    this->_possibleMelds.clear(); // reset;

    for (const int takeCard : takeCards)
    {
        if (!SvrCard::isValidIndex(takeCard))
            continue;

        const State state = this->_indexesByState.at(takeCard);
        if (state == STATE_IN_OPPONENT_HAND)
        {
            const int rank = SvrCard::convertRank(takeCard);
            const int suit = SvrCard::convertSuit(takeCard);

            const map<State, int>& mapRanks = this->_ranksByState.at(rank);
            if (mapRanks.at(STATE_IN_PLAYER_HAND) + mapRanks.at(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND) >= 2)
            {
                vector<vector<int>> melds;

                for (int r = rank - 2; r <= rank; r++)
                {
                    if (SvrCard::isValidRank(r))
                    {
                        vector<int> meld;
                        for (int    newRank = r; newRank <= rank + 2; newRank++)
                        {
                            if (!SvrCard::isValidRank(newRank))
                                break;

                            const int newIndex = SvrCard::convertIndex(newRank, suit);
                            if (this->_indexesByState.at(newIndex) >= STATE_UNKNOWN_CARD)
                                meld.push_back(newIndex);
                            else
                                break;
                        }
                        if (meld.size() >= RuleConstant::Common::MIN_NUM_CARD_ON_MELD)
                        {
                            melds.push_back(meld);
                            break;
                        }
                    }
                }
                for (const vector<int>& possibleMeld : melds)
                {
                    for (const int& index : possibleMeld)
                        if (this->_indexesByState.at(index) == STATE_UNKNOWN_CARD)
                            updateState(state, index);
                    this->_possibleMelds.push_back(possibleMeld);
                }
            }
            else
            {
                vector<vector<int>> melds;

                // ranks
                for (int r = rank - 2; r <= rank; r++)
                {
                    if (!SvrCard::isValidRank(r))
                        continue;

                    vector<int> meld;
                    for (int    newRank = r; newRank <= rank + 2; newRank++)
                    {
                        if (!SvrCard::isValidRank(newRank))
                            break;

                        const int newIndex = SvrCard::convertIndex(newRank, suit);
                        if (this->_indexesByState.at(newIndex) >= STATE_UNKNOWN_CARD)
                            meld.push_back(newIndex);
                        else
                            break;
                    }
                    if (meld.size() >= RuleConstant::Common::MIN_NUM_CARD_ON_MELD)
                    {
                        melds.push_back(meld);
                        break;
                    }
                }

                // suits
                vector<int> meld;
                for (int    s = 0; s < SvrCard::Common::MAX_SUIT; s++)
                {
                    const int newIndex = SvrCard::convertIndex(rank, s);
                    if (this->_indexesByState.at(newIndex) >= STATE_UNKNOWN_CARD)
                        meld.push_back(newIndex);
                }
                if (meld.size() >= RuleConstant::Common::MIN_NUM_CARD_ON_MELD)
                    melds.push_back(meld);

                for (const vector<int>& possibleMeld : melds)
                {
                    for (const int& index : possibleMeld)
                        if (this->_indexesByState.at(index) == STATE_UNKNOWN_CARD)
                            updateState(state, index);
                    this->_possibleMelds.push_back(possibleMeld);
                }
            }
        }
    }
}

bool BotOpponentHandEstimation::updateState(State state, int card)
{
    State curState = this->_indexesByState.at(card);
    if (curState == state)
        return false;

    this->_indexesByState.at(card) = state;

    resetRanks();
    resetSuits();

    for (int i = 0; i < this->_indexesByState.size(); i++)
    {
        const int r = SvrCard::convertRank(i);
        const int s = SvrCard::convertSuit(i);

        this->_suits.at(s) = this->_suits.at(s) + 1;

        map<State, int> mapCounts = this->_ranksByState.at(r);

        const State f     = this->_indexesByState.at(i);
        int         count = (mapCounts.find(f) != mapCounts.end()) ? mapCounts.at(f) : 0;
        mapCounts.at(f)           = count + 1;
        this->_ranksByState.at(r) = mapCounts;
    }

    return true;
}

void BotOpponentHandEstimation::updateState(State state, const vector<int>& cards)
{
    for (const int card : cards)
        if (SvrCard::isValidIndex(card))
            updateState(state, card);
}

void BotOpponentHandEstimation::estimateCardOpponentNotCare(State sta, bool loop)
{
    bool hasChanged = false;

    for (int card = 0; card < SvrCard::Common::MAX_CARD; card++)
    {
        const int   rank  = SvrCard::convertRank(card);
        const int   suit  = SvrCard::convertSuit(card);
        const State state = this->_indexesByState.at(card);

        if (state != sta)
            continue; // skip

        const map<State, int>& mapStates = this->_ranksByState.at(rank);
        const int countInPlayerHand = mapStates.at(STATE_IN_PLAYER_HAND);
        const int countSafeCards    = mapStates.at(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND);
        if (countSafeCards >= 2 || (countSafeCards + countInPlayerHand >= 3))
        {
            if (rank == SvrCard::Rank::RANK_A || rank == SvrCard::Rank::RANK_K)
            {
                int rank1, rank2;
                if (rank == SvrCard::Rank::RANK_A)
                {
                    rank1 = rank + 1;
                    rank2 = rank + 2;
                }
                else
                {
                    rank1 = rank - 2;
                    rank2 = rank - 1;
                }

                const int index1 = SvrCard::convertIndex(rank1, suit);
                const int index2 = SvrCard::convertIndex(rank2, suit);

                const State state1 = this->_indexesByState.at(index1);
                const State state2 = this->_indexesByState.at(index2);

                bool condition = (state1 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND) || (state2 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND) || (state1 == STATE_IN_PLAYER_HAND && state2 == STATE_IN_PLAYER_HAND);
                if (condition && updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, card) && state == sta)
                    hasChanged = true;
            }
            else // if (rank >= SvrCard::Rank::RANK_2 && rank <= SvrCard::Rank::RANK_Q)
            {
                const int leftRank  = rank - 1;
                const int rightRank = rank + 1;

                const int leftIndex  = SvrCard::convertIndex(leftRank, suit);
                const int rightIndex = SvrCard::convertIndex(rightRank, suit);

                const State leftState  = this->_indexesByState.at(leftIndex);
                const State rightState = this->_indexesByState.at(rightIndex);

                bool condition = (STATE_IN_PLAYER_HAND <= leftState && leftState <= STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && STATE_IN_PLAYER_HAND <= rightState && rightState <= STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND);

                if (condition && updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, card) && state == sta) // ... | -1 | 0 | -1 | ...
                    hasChanged = true;
                else
                {
                    // ... | -1 | 0 | 0 | -1 | ...
                    if (rank == SvrCard::Rank::RANK_2)
                    {
                        const int rank1 = rank - 1;
                        const int rank4 = rank + 2;

                        const int index1 = SvrCard::convertIndex(rank1, suit);
                        const int index4 = SvrCard::convertIndex(rank4, suit);

                        const State state1 = this->_indexesByState.at(index1);
                        const State state4 = this->_indexesByState.at(index4);

                        condition = (state1 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && state4 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND);

                        if (condition && updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, card) && state == sta)
                            hasChanged = true;
                    }
                    else if (rank == SvrCard::Rank::RANK_Q)
                    {
                        const int rank1 = rank - 2;
                        const int rank4 = rank + 1;

                        const int index1 = SvrCard::convertIndex(rank1, suit);
                        const int index4 = SvrCard::convertIndex(rank4, suit);

                        const State state1 = this->_indexesByState.at(index1);
                        const State state4 = this->_indexesByState.at(index4);

                        condition = (state1 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && state4 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND);

                        if (condition && updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, card) && state == sta)
                            hasChanged = true;
                    }
                    else
                    {
                        const int rank1 = rank - 2;
                        const int rank2 = rank - 1;
                        const int rank4 = rank + 1;
                        const int rank5 = rank + 2;

                        const int index1 = SvrCard::convertIndex(rank1, suit);
                        const int index2 = SvrCard::convertIndex(rank2, suit);
                        const int index4 = SvrCard::convertIndex(rank4, suit);
                        const int index5 = SvrCard::convertIndex(rank5, suit);

                        const State state1 = this->_indexesByState.at(index1);
                        const State state2 = this->_indexesByState.at(index2);
                        const State state4 = this->_indexesByState.at(index4);
                        const State state5 = this->_indexesByState.at(index5);

                        condition = ((state1 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && state4 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND) ||
                                     (state2 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND && state5 == STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND));

                        if (condition && updateState(STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND, card) && state == sta)
                            hasChanged = true;
                    }
                }
            }
        }
    }

    if (loop && hasChanged)
        estimateCardOpponentNotCare(sta, loop);
}

void BotOpponentHandEstimation::logConsoleTable()
{
#if DEBUG_AI_BOT
    vector<string> headers = { "Suits \\ Ranks" };
    for (int       r       = 0; r < SvrCard::Common::MAX_RANK; r++)
    {
        string s;
        if (r < 10)
            s.append(" ");
        s.append(to_string(r)).append(" (").append(DebugHelper::ranks2Str(r)).append(")");
        headers.push_back(s);
    }
    headers.emplace_back("suits");

    vector<vector<string>> rows;
    for (int               suit = 0; suit < SvrCard::Common::MAX_SUIT; suit++)
        rows.push_back({ to_string(suit) });

    for (int suit = 0; suit < SvrCard::Common::MAX_SUIT; suit++)
    {
        for (int  rank  = 0; rank < SvrCard::Common::MAX_RANK; rank++)
        {
            const int   index = SvrCard::convertIndex(rank, suit);
            const State state = this->_indexesByState.at(index);
            string      s;
            if (state >= 0 && state < 10)
                s.append(" ");
            if (state == STATE_UNKNOWN_CARD)
                s.append("-");
            else
                s.append(to_string(state));
            rows.at(suit).push_back(s);
        }
        const int count = this->_suits.at(suit);
        string    s;
        if (count >= 0 && count < 10)
            s.append(" ");
        s.append(to_string(count));
        rows.at(suit).push_back(s);
    }

    for (int f = STATE_IN_PLAYER_HAND; f <= STATE_IN_OPPONENT_HAND; f++)
    {
        string fStr = "ranks: ";
        if (f < 10)
            fStr.append(" ");
        fStr.append(to_string(f));
        vector<string> row{ fStr };
        for (int       rank = 0; rank < SvrCard::Common::MAX_RANK; rank++)
        {
            string    s;
            const int c = this->_ranksByState.at(rank).at((State) f);
            if (c >= 0 && c < 10)
                s.append(" ");
            s.append(to_string(c));
            row.push_back(s);
        }
        row.emplace_back("");
        rows.push_back(row);
    }

    ConsoleTable consoleTable(headers);
    for (const vector<string>& r : rows)
        consoleTable.addRow(r);
    LogHelper::debugConsoleTable(consoleTable);


    map<State, int> countStates;
    for (int        index = 0; index < SvrCard::Common::MAX_CARD; index++)
    {
        const State state = this->_indexesByState.at(index);
        if (countStates.find(state) != countStates.end())
            countStates[state]++;
        else
            countStates[state] = 1;
    }

    headers.clear();
    rows.clear();
    headers.emplace_back("State");
    headers.emplace_back("Count");
    for (const auto it : countStates)
    {
        vector<string> row{ to_string(it.first), to_string(it.second) };
        rows.push_back(row);
    }
    ConsoleTable    ct(headers);
    for (const vector<string>& r : rows)
        ct.addRow(r);
    LogHelper::debugConsoleTable(ct);
#endif
}
