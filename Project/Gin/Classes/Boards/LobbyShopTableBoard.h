#ifndef __LOBBY_SHOP_TABLE_BOARD_H__
#define __LOBBY_SHOP_TABLE_BOARD_H__

#include "InterfacePopupBoard.h"
#include "Component/LobbyTableNew.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "Helper/JsonHelper.h"
#include <vector>

class LobbyScene;

class LobbyShopTableBoard : public InterfacePopupBoard
{
private:
    LobbyScene           * _lobbyScene;
    cocos2d::ui::Text    * _textBMTitle;
    cocos2d::ui::ListView* _listViewContent;
    cocos2d::ui::Button  * _buttonClose;
    cocos2d::Vector<LobbyTableNew*> _listTable;
public:
    virtual void changeLanguage(Language language) override;
    ~LobbyShopTableBoard();
    LobbyShopTableBoard();

    virtual bool init() override;
    void initScreen();
    void initValue(const ValueType& listDataTable);
    void setLobbyScene(LobbyScene* lobbyScene);

    virtual void onExit() override;

    void selectedItemListViewEvent(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType type);
    void onButtonClick(cocos2d::Ref* sender);

    CREATE_FUNC(LobbyShopTableBoard);
};

#endif //__LOBBY_SHOP_TABLE_BOARD_H__
