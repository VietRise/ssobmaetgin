#ifndef __LOBBY_SCENE_H__
#define __LOBBY_SCENE_H__

#include "InterfaceScene.h"
#include "Component/LobbyPlace.h"
#include "Component/LobbyTableNew.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#include "Define/InGameDefine.h"
#include "Define/ServerClientMessageDefine.h"

class LobbyScene : public InterfaceScene
{
private:
    cocos2d::ui::Layout    * _layoutRoot;
    cocos2d::ui::Button    * _buttonSetting;
    cocos2d::ui::Button    * _buttonRating;
    cocos2d::ui::Button    * _buttonFreeGold;
    cocos2d::ui::Button    * _buttonLeaderboard;
    cocos2d::ui::TextBMFont* _textBMFreeGold;
    cocos2d::ui::Button    * _buttonHelp;
    cocos2d::ui::Button    * _buttonGameCenter;
    cocos2d::ui::Button    * _buttonPromoteGame1;
    cocos2d::ui::Button    * _buttonPromoteGame2;
    cocos2d::ui::Button    * _buttonMailbox;
    cocos2d::ui::Button    * _buttonVideoBonusReward;
    cocos2d::ui::Text      * _textBMVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusReward;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow0;
    cocos2d::ui::ImageView * _imageVideoBonusRewardGlow1;
    cocos2d::ui::Button    * _buttonHourlyBonus;
    cocos2d::ui::Text      * _textHourlyBonusTitle;
    cocos2d::ui::TextBMFont* _textHourlyBonusMoney;
    cocos2d::ui::TextBMFont* _textHourlyBonusCountDown;
    cocos2d::ui::Text      * _textHourlyBonusGetNow;
    cocos2d::ui::Text      * _textGameCenter;
    cocos2d::ui::ImageView * _imgHourlyBonusGet;
    cocos2d::ui::Text      * _textVideoBonusHint;
    cocos2d::ui::Layout    * _layoutBottomBar;
    cocos2d::ui::Button    * _buttonShop;
    cocos2d::ui::TextBMFont* _textBMMoneyUser;
    cocos2d::ui::Layout    * _layoutInfoUser;
    cocos2d::ui::Layout    * _layoutAvatarUser;
    cocos2d::ui::Text      * _textBMNameUser;
    cocos2d::ui::TextBMFont* _textUserLevel;
    cocos2d::ui::Slider    * _sliderUserLevel;
    cocos2d::ui::Layout    * _layoutChoosePlace;
    cocos2d::ui::Layout    * _layoutEventDecor;
    std::vector<LobbyPlace*> _listPlace;

    std::string _listDataStr;
    std::string _listMailStr;

    float _imageVideoBonusRewardScaleX;
    float _imageVideoBonusRewardScaleY;

    double         _hourlyBonusCountDown;
    double         _hourlyBonusMoney;
    cocos2d::Point _posButtonHourlyBonus;

    int         _ratingState;
    std::string _ratingCondition;

    void changeLanguage(Language language);
    void setHourlyBonusButtonState(int state, double countdown, double money);
    cocos2d::Action* createMoveHourlyBonusButton(int state);
protected:
    void onProfileInit(const rapidjson::Document& data);
    void onUserData(const rapidjson::Document& data);
    void onUnlockCity(const rapidjson::Document& data);
    void onUnlockTable(const rapidjson::Document& data);
    void onChangeUserName(const rapidjson::Document& data);
    void onChangeUserAvatar(const rapidjson::Document& data);
    void onTicket(const rapidjson::Document& data);
    void onJoinGame(const rapidjson::Document& data);
    void onGoldBonus(const rapidjson::Document& data);
    void onSaviorGoldBonus(const rapidjson::Document& data);
    void onSaviorGoldBonusPopup(const rapidjson::Document& data);
    void onVideoAdsMoney(const rapidjson::Document& data);
    void onLogInBonus(const rapidjson::Document& data);
    void onNotify(void* sender, int id, const cocos2d::Value& data);
    void onHourlyBonusData(const rapidjson::Document& data);
    void onCheckRatingPopup(const rapidjson::Document& data);
    void onVideoRewardEnable(const rapidjson::Document& data);
    void onVideoRewardInfo(const rapidjson::Document& data);
    void onSuggestUpdate(const rapidjson::Document& data);
    void onMailbox(const rapidjson::Document& data);
    void onClaimMailboxReward(const rapidjson::Document& data);
    void onGetLocalNotificationData(const rapidjson::Document& data);
public:
    static bool _backFromGameScene;

CC_SYNTHESIZE(LobbyTableNew*, _lobbyTableSelected, LobbyTableSelected);

    ~LobbyScene();
    LobbyScene();
    static cocos2d::Scene* createScene();
    CREATE_FUNC(LobbyScene);
    virtual bool init() override;
    void initScreen();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onLoadResourcesSyncDone() override;
    virtual void onLoadResourcesAsyncDone() override;
    virtual SceneID getSceneID() override;

    void openShopTable();
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);
    void onButtonClick(cocos2d::Ref* sender);
    void onLayoutClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
    void onTouchLayoutPlaceListener(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);

    void showTutorial();
    void openGameCenter();
    void openLeaderboard();

    void showShopNotify(bool show);
    void showMailNotify(bool show);
    void checkNewMail();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void checkAndEnableEventDecorLayer();

    static bool hasNewMail(const std::string& jsonData);
};

#endif // ! __LOBBY_SCENE_H__
