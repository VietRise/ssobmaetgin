#include "BoardManager.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

static BoardManager* _instance = nullptr;

BoardManager::~BoardManager()
{
}

BoardManager::BoardManager()
{
}

BoardManager* BoardManager::getInstance()
{
    if (!_instance)
        _instance = new BoardManager();
    return _instance;
}

void BoardManager::destroyInstance()
{
    if (_instance)
        CC_SAFE_DELETE(_instance);
}

LobbyTicketFlightBoard* BoardManager::createLobbyTicketFlightBoard()
{
    return (LobbyTicketFlightBoard*) getInstance()->createBoard(BoardID::LOBBY_TICKETFLIGHT_BOARD);
}

YesNoConfirmPopup* BoardManager::createYesNoConfirmPopup()
{
    return (YesNoConfirmPopup*) getInstance()->createBoard(BoardID::YESNO_CONFIRM_POPUP);
}

LobbySettingBoard* BoardManager::createLobbySettingBoard()
{
    return (LobbySettingBoard*) getInstance()->createBoard(BoardID::LOBBY_SETTING_BOARD);
}

LobbyInfoUserBoard* BoardManager::createLobbyInfoUserBoard()
{
    return (LobbyInfoUserBoard*) getInstance()->createBoard(BoardID::LOBBY_INFOUSER_BOARD);
}

GetMoneyToPlayPopup* BoardManager::createGetMoneyToPlayPopupBoard()
{
    return (GetMoneyToPlayPopup*) getInstance()->createBoard(BoardID::GET_MONEY_TO_PLAY_BOARD);
}

ContinueToPlayConfirmPopup* BoardManager::createContinueToPlayConfirmPopup()
{
    return (ContinueToPlayConfirmPopup*) getInstance()->createBoard(BoardID::CONTINUE_TO_PLAY_CONFIRM_POPUP);
}

LobbyShopTableBoard* BoardManager::createLobbyShopTableBoard()
{
    return (LobbyShopTableBoard*) getInstance()->createBoard(BoardID::LOBBY_SHOP_TABLE_BOARD);
}

NotifyPopup* BoardManager::createNotifyPopup()
{
    return (NotifyPopup*) getInstance()->createBoard(BoardID::NOTIFY_POPUP);
}

LeaderboardPopup* BoardManager::createLeaderboardPopup()
{
    return (LeaderboardPopup*) getInstance()->createBoard(BoardID::LEADERBOARD_POPUP);
}

NewsPopup* BoardManager::createNewsPopup()
{
    return (NewsPopup*) getInstance()->createBoard(BoardID::NEWS_POPUP);
}

RewardPopup* BoardManager::createRewardPopup()
{
    return (RewardPopup*) getInstance()->createBoard(BoardID::REWARD_POPUP);
}

RatingPopup* BoardManager::createRatingPopup()
{
    return (RatingPopup*) getInstance()->createBoard(BoardID::RATING_POPUP);
}

RatingPopupThank* BoardManager::createRatingThankPopup()
{
    return (RatingPopupThank*) getInstance()->createBoard(BoardID::RATING_THANK_POPUP);
}

GameCenterPopup* BoardManager::createGameCenterPopup()
{
    return (GameCenterPopup*) getInstance()->createBoard(BoardID::GAME_CENTER_POPUP);
}

VideoRewardsPopup* BoardManager::createVideoRewardsPopup()
{
    return (VideoRewardsPopup*) getInstance()->createBoard(BoardID::VIDEO_REWARDS_POPUP);
}

SuggestUpdatePopup* BoardManager::createSuggestUpdatePopup()
{
    return (SuggestUpdatePopup*) getInstance()->createBoard(BoardID::SUGGEST_UPDATE_POPUP);
}

MailboxPopup* BoardManager::createMailboxPopup()
{
    return (MailboxPopup*) getInstance()->createBoard(BoardID::MAILBOX_POPUP);
}

MailboxContentPopup* BoardManager::createMailboxContentPopup()
{
    return (MailboxContentPopup*) getInstance()->createBoard(BoardID::MAILBOX_CONTENT_POPUP);
}

ThrowCardDetailPopup* BoardManager::createThrowCardDetailPopup()
{
    return (ThrowCardDetailPopup*) getInstance()->createBoard(BoardID::THROW_CARD_DETAIL_POPUP);
}

FightChallengerPopup* BoardManager::createFightChallengerPopup()
{
    return (FightChallengerPopup*) getInstance()->createBoard(BoardID::FIGHT_CHALLENGER_POPUP);
}

GameResultPopup* BoardManager::createGameResultPopup()
{
    return (GameResultPopup*) getInstance()->createBoard(BoardID::GAME_RESULT_POPUP);
}

RoundPointPopup* BoardManager::createRoundPointPopup()
{
    return (RoundPointPopup*) getInstance()->createBoard(BoardID::ROUND_POINT_POPUP);
}

ChangeAvatarPopup* BoardManager::createChangeAvatarPopup()
{
    return (ChangeAvatarPopup*) getInstance()->createBoard(BoardID::CHANGE_AVATAR_POPUP);
}

ActionConfirmPopup* BoardManager::createActionConfirmPopup()
{
    return (ActionConfirmPopup*) getInstance()->createBoard(BoardID::ACTION_CONFIRM_POPUP);
}

InterfacePopupBoard* BoardManager::createBoard(BoardID boardId)
{
    InterfacePopupBoard* popup = nullptr;
    switch (boardId)
    {
        case BoardID::LOBBY_TICKETFLIGHT_BOARD:
            popup = LobbyTicketFlightBoard::create();
            break;
        case BoardID::YESNO_CONFIRM_POPUP:
            popup = YesNoConfirmPopup::create();
            break;
        case BoardID::LOBBY_SETTING_BOARD:
            popup = LobbySettingBoard::create();
            break;
        case BoardID::LOBBY_INFOUSER_BOARD:
            popup = LobbyInfoUserBoard::create();
            break;
        case BoardID::GET_MONEY_TO_PLAY_BOARD:
            popup = GetMoneyToPlayPopup::create();
            break;
        case BoardID::CONTINUE_TO_PLAY_CONFIRM_POPUP:
            popup = ContinueToPlayConfirmPopup::create();
            break;
        case BoardID::LOBBY_SHOP_TABLE_BOARD:
            popup = LobbyShopTableBoard::create();
            break;
        case BoardID::NOTIFY_POPUP:
            popup = NotifyPopup::create();
            break;
        case BoardID::LEADERBOARD_POPUP:
            popup = LeaderboardPopup::create();
            break;
        case BoardID::NEWS_POPUP:
            popup = NewsPopup::create();
            break;
        case BoardID::RATING_POPUP:
            popup = RatingPopup::create();
            break;
        case BoardID::RATING_THANK_POPUP:
            popup = RatingPopupThank::create();
            break;
        case BoardID::REWARD_POPUP:
            popup = RewardPopup::create();
            break;
        case BoardID::GAME_CENTER_POPUP:
            popup = GameCenterPopup::create();
            break;
        case BoardID::VIDEO_REWARDS_POPUP:
            popup = VideoRewardsPopup::create();
            break;
        case BoardID::SUGGEST_UPDATE_POPUP:
            popup = SuggestUpdatePopup::create();
            break;
        case BoardID::MAILBOX_POPUP:
            popup = MailboxPopup::create();
            break;
        case BoardID::MAILBOX_CONTENT_POPUP:
            popup = MailboxContentPopup::create();
            break;
        case BoardID::THROW_CARD_DETAIL_POPUP:
            popup = ThrowCardDetailPopup::create();
            break;
        case BoardID::FIGHT_CHALLENGER_POPUP:
            popup = FightChallengerPopup::create();
            break;
        case BoardID::GAME_RESULT_POPUP:
            popup = GameResultPopup::create();
            break;
        case BoardID::ROUND_POINT_POPUP:
            popup = RoundPointPopup::create();
            break;
        case BoardID::CHANGE_AVATAR_POPUP:
            popup = ChangeAvatarPopup::create();
            break;
        case BoardID::ACTION_CONFIRM_POPUP:
            popup = ActionConfirmPopup::create();
            break;
        default:
            break;
    }
    popup->setTag((int) boardId);
    return popup;
}
