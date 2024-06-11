#ifndef __BOARD_MANAGER_H__
#define __BOARD_MANAGER_H__

#include "Boards/YesNoConfirmPopup.h"
#include "Boards/LobbySettingBoard.h"
#include "Boards/LobbyInfoUserBoard.h"
#include "Boards/LobbyTicketFlightBoard.h"
#include "Boards/GetMoneyToPlayPopup.h"
#include "Boards/ContinueToPlayConfirmPopup.h"
#include "Boards/LobbyShopTableBoard.h"
#include "Boards/NotifyPopup.h"
#include "Boards/LeaderboardPopup.h"
#include "Boards/NewsPopup.h"
#include "Boards/RatingPopup.h"
#include "Boards/RatingPopupThank.h"
#include "Boards/RewardPopup.h"
#include "Boards/GameCenterPopup.h"
#include "Boards/VideoRewardsPopup.h"
#include "Boards/SuggestUpdatePopup.h"
#include "Boards/MailboxPopup.h"
#include "Boards/MailboxContentPopup.h"
#include "Boards/ThrowCardDetailPopup.h"
#include "Boards/FightChallengerPopup.h"
#include "Boards/GameResultPopup.h"
#include "Boards/RoundPointPopup.h"
#include "Boards/ChangeAvatarPopup.h"
#include "Boards/ActionConfirmPopup.h"

enum class BoardID
{
    LOBBY_TICKETFLIGHT_BOARD       = 1011,
    YESNO_CONFIRM_POPUP            = 1018,
    LOBBY_SETTING_BOARD            = 1019,
    LOBBY_INFOUSER_BOARD           = 1020,
    GET_MONEY_TO_PLAY_BOARD        = 1024,
    CONTINUE_TO_PLAY_CONFIRM_POPUP = 1027,
    LOBBY_SHOP_TABLE_BOARD         = 1029,
    NOTIFY_POPUP                   = 1030,
    LEADERBOARD_POPUP              = 1031,
    NEWS_POPUP                     = 1032,
    RATING_POPUP                   = 1033,
    RATING_THANK_POPUP             = 1034,
    REWARD_POPUP                   = 1035,
    GAME_CENTER_POPUP              = 1036,
    VIDEO_REWARDS_POPUP            = 1037,
    SUGGEST_UPDATE_POPUP           = 1038,
    MAILBOX_POPUP                  = 1039,
    MAILBOX_CONTENT_POPUP          = 1040,
    THROW_CARD_DETAIL_POPUP        = 1060,
    FIGHT_CHALLENGER_POPUP         = 1061,
    GAME_RESULT_POPUP              = 1062,
    ROUND_POINT_POPUP              = 1063,
    CHANGE_AVATAR_POPUP            = 1064,
    ACTION_CONFIRM_POPUP           = 1067,
};


class BoardManager
{
private:
protected:
public:
    ~BoardManager();
    BoardManager();

    static BoardManager* getInstance();
    static void destroyInstance();

    static LobbyTicketFlightBoard* createLobbyTicketFlightBoard();
    static YesNoConfirmPopup* createYesNoConfirmPopup();
    static LobbySettingBoard* createLobbySettingBoard();
    static LobbyInfoUserBoard* createLobbyInfoUserBoard();
    static GetMoneyToPlayPopup* createGetMoneyToPlayPopupBoard();
    static ContinueToPlayConfirmPopup* createContinueToPlayConfirmPopup();
    static LobbyShopTableBoard* createLobbyShopTableBoard();
    static NotifyPopup* createNotifyPopup();
    static LeaderboardPopup* createLeaderboardPopup();
    static NewsPopup* createNewsPopup();
    static RewardPopup* createRewardPopup();
    static RatingPopup* createRatingPopup();
    static RatingPopupThank* createRatingThankPopup();
    static GameCenterPopup* createGameCenterPopup();
    static VideoRewardsPopup* createVideoRewardsPopup();
    static SuggestUpdatePopup* createSuggestUpdatePopup();
    static MailboxPopup* createMailboxPopup();
    static MailboxContentPopup* createMailboxContentPopup();
    static ThrowCardDetailPopup* createThrowCardDetailPopup();
    static FightChallengerPopup* createFightChallengerPopup();
    static GameResultPopup* createGameResultPopup();
    static RoundPointPopup* createRoundPointPopup();
    static ChangeAvatarPopup* createChangeAvatarPopup();
    static ActionConfirmPopup* createActionConfirmPopup();

    InterfacePopupBoard* createBoard(BoardID boardId);
};

#endif // ! __BOARD_MANAGER_H__
