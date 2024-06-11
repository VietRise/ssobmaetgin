#ifndef __LOBBY_INFOUSER_BOARD_H__
#define __LOBBY_INFOUSER_BOARD_H__

#include "InterfacePopupBoard.h"
#include "Component/Avatar.h"
#include "Define/ServerClientMessageDefine.h"

class LobbyInfoUserBoard : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout    * _layoutContent;
    cocos2d::ui::Text      * _textUsername;
    cocos2d::ui::Layout    * _layoutAvatar;
    cocos2d::ui::Button    * _buttonConfirm;
    cocos2d::ui::Button    * _buttonSignOut;
    cocos2d::ui::Button    * _buttonSignIn;
    cocos2d::ui::Button    * _buttonEdit;
    cocos2d::ui::Text      * _textProfileTitle;
    cocos2d::ui::Text      * _textTotalTitle;
    cocos2d::ui::Text      * _textTotalWinTitle;
    cocos2d::ui::Text      * _textTotalWinRateTitle;
    cocos2d::ui::Text      * _textTotal;
    cocos2d::ui::Text      * _textTotalWin;
    cocos2d::ui::Text      * _textTotalWinRate;
    cocos2d::ui::TextBMFont* _textMoney;
    cocos2d::ui::TextBMFont* _textLevel;
    cocos2d::ui::Button    * _buttonClose;

    std::string _name;
    virtual void changeLanguage(Language language) override;
public:
    void onReceiveNotificationValue(void* sender, int id, const cocos2d::Value& data);

    ~LobbyInfoUserBoard();
    LobbyInfoUserBoard();

    virtual bool init() override;
    void initScreen();
    void initValue(const std::string& name, Avatar* avatar);

    virtual void onEnter() override;
    virtual void onExit() override;

    void onButtonClick(cocos2d::Ref* sender);
    void onLayoutClick(cocos2d::Ref* sender);

    void onChangeUserName(const rapidjson::Document& data);
    void onChangeUserAvatar(const rapidjson::Document& data);
    void onServerResponse(MessageID messageId, const rapidjson::Document& data);
    CREATE_FUNC(LobbyInfoUserBoard);
};

#endif // !__LOBBY_INFOUSER_BOARD_H__