#ifndef __BOT_OPPONENT_HAND_ESTIMATION_H__
#define __BOT_OPPONENT_HAND_ESTIMATION_H__

#include <vector>
#include <string>
#include <algorithm>
#include <map>

class BotOpponentHandEstimation
{
public:
    enum State
    {
        STATE_IN_PLAYER_HAND                       = -2, // if the card is in the player hand.
        STATE_SAFE_DISCARD_OR_NOT_IN_OPPONENT_HAND = -1, // if the card is a safe discard or is not in the opponent hand.
        STATE_UNKNOWN_CARD                         = 0, // if the card is unknown.
        STATE_IN_OPPONENT_HAND                     = 1 //  if the card is in the opponent hand or likely to be in the opponent meld.
    };

    BotOpponentHandEstimation(const std::vector<int>& playerHandCards,
                              const std::vector<int>& playerDiscards,
                              const std::vector<int>& opponentTakeCards,
                              const std::vector<int>& opponentDiscards,
                              const std::vector<int>& pileDiscards);
    std::vector<int> getSafeCards();
    std::vector<int> getNotSafeCards();
    std::vector<std::vector<int>> getPossibleMelds();

private:
    std::vector<int> _playerHandCards;
    std::vector<int> _playerDiscards;
    std::vector<int> _opponentTakeCards;
    std::vector<int> _opponentDiscards;
    std::vector<int> _discardPile;

    std::vector<std::vector<int>> _possibleMelds;

    std::vector<State>                _indexesByState;
    std::vector<std::map<State, int>> _ranksByState;
    std::vector<int>                  _suits;

    void initIndexesByState();
    void initRanksByState();
    void initSuits();

    void resetIndexes();
    void resetRanks();
    void resetSuits();

    int getCountRankOfSafeDiscards(int rank);

    void estimateCardOpponentCare(const std::vector<int>& takeCards);
    bool updateState(State state, int card);
    void updateState(State state, const std::vector<int>& cards);
    void estimateCardOpponentNotCare(State sta, bool loop);
    void logConsoleTable();
};

#endif // __BOT_OPPONENT_HAND_ESTIMATION_H__