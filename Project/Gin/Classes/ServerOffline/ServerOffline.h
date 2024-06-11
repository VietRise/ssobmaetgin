#ifndef __SERVER_OFFLINE_H__
#define __SERVER_OFFLINE_H__

#include "json/rapidjson.h"
#include "json/document.h"
#include "GinGameMatchManager.h"
#include "Define/ServerClientMessageDefine.h"

class ServerOffline : public cocos2d::Node
{
private:
protected:
    static ServerOffline* _serverOfflineInstance;
public:
    ServerOffline();
    ~ServerOffline();
    virtual bool init() override;

    CREATE_FUNC(ServerOffline);

    void onRequestBuyNewCity(int id);
    void onRequestBuyNewTable(int id);
    void onRequestTicket(int cityId, int tableId);
    void onRequestJoinCity(const rapidjson::Document& data);
    void onRequestUserData();
    void onRequestUserProfileData(const rapidjson::Document& data);
    void onRequestChangeName(const std::string& name);
    void onRequestChangeAvatar(const std::string& path);
    void onRequestBonus();
    void onRequestSaviorBonus();
    void onRequestQuitMatch();
    void onRequestSaviorPopup();
    void onVideoAdsMoney();
    void onLogInBonusMoney();
    void onLoadSavedMatch();
    void onRequestGetHourlyBonus();
    void onRequestCheckShowRatingPopup(int where);
    void onRequestRatingSumbit(int star);
    void sendHourlyBonusData(int result = 0);
    void resetUserData();
    void onRequestIsVideoRewardFunctionEnable();
    void onRequestVideoRewardInfo();
    void onRequestVideoRewardSkip();
    void onRequestVideoRewardClaim(bool isSkipWaitingPhase);

    void onSuggestUpdate();
    void onMailbox();
    void onClaimRewardMailbox(const std::string& type, const std::string& id);
    void onGetLocalNotificationData();

    static ServerOffline* getInstance();
    static void destroyInstance();

    void onMessage(MessageID messageId, const rapidjson::Document& data);

    static bool  _getBonusGold;
    static int   _consecutiveLose;
    static int   _consecutiveWin;
    static float _winBetMultiple;
    static int   _matchCountPerOneTimeEnterRoom;
    static int   _winScoreGapMultiply;
    static int  _winJackpot;
    static int   _gameCountPerOneTimeEnterRoom;
    static int   _winGameRoundCount;
    static int   _winGame;

	GinGameMatchManager* _currentGameManager;
};

#endif
