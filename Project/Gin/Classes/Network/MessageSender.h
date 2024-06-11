#ifndef __MESSAGE_SENDER_H__
#define __MESSAGE_SENDER_H__

#include <string>
#include <vector>
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"
#include "Define/ServerClientMessageDefine.h"

class MessageSender
{
private:
protected:
public:
    static void requestStartGame();
    static void requestQuitInGame();
    static void requestDealCardFinish();
    static void requestBeginTurnFinish(int playerIndex);
    static void requestThrowCard(int cardValue, int actionType, const std::vector<std::vector<int>>& melds);
    static void requestThrowCardDone(int playerIndex, const std::vector<std::vector<int>>& melds);
    static void requestGetCard(int playerIndex = -1);
    static void requestGetCardDone(int playerIndex);
    static void requestPass(int playerIndex, int upCardValue);
    static void requestPassDone(int playerIndex);
    static void requestCustomMelds(int playerIndex, const std::vector<std::vector<int>>& melds);

    static void requestUserData();
    static void requestUserProfile(const std::string& where, int versionCode);
    static void requestShowGoldBonusPopup();
    static void requestGoldBonus();
    static void requestUnlockTable(int id);
    static void requestUnlockCity(int id);
    static void requestOpenTicket(int cityId, int tableId);
    static void requestFlightNow(int cityId, int tableId, double betPrice, int targetScore);
    static void requestChangeName(const std::string& name);
    static void requestChangeProfilePicture(const std::string& path);
    static void requestBonusMoneyWatchVideo();
    static void requestVideoAdsMoney();
    static void requestLogInBonus();
    static void requestLoadSavedMatch();
    static void requestHourlyBonus();
    static void requestCheckShowRatingPopup(int where);
    static void requestRatingSubmit(int star);
    static void requestCheatCards(int slotIdx, CheatCardType cheatCardType, int cardValue);

    static void requestVideoRewardEnable();
    static void requestVideoRewardInfo();
    static void requestVideoRewardSkip();
    static void requestVideoRewardClaim(bool isSkippedWaitingPhase);

    static void requestCheckSuggestUpdate();
    static void requestMailbox();
    static void requestClaimMailboxReward(const std::string& type, const std::string& id);
    static void requestLocalNotificationData();
};

#endif //__MESSAGE_SENDER_H__
