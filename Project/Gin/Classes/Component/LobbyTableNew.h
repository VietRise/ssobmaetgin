#ifndef __LOBBY_TABLE_NEW_H__
#define __LOBBY_TABLE_NEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "Manager/TextManager.h"

class LobbyScene;

class LobbyShopTableBoard;

class LobbyTableNew : public cocos2d::Ref
{
public:
    enum class State
    {
        NORMAL             = 1,
        SELECTED           = 2,
        LOCK_CAN_UNLOCK    = 0,
        LOCK_CANNOT_UNLOCK = -1,
    };
private:
    cocos2d::ui::Layout    * _layoutTable;
    cocos2d::ui::ImageView * _imageViewTable;
    cocos2d::ui::Text      * _textBMLuckyPercent;
    cocos2d::ui::ImageView * _imageViewCheckBox;
    cocos2d::ui::ImageView * _imageViewCheck;
    cocos2d::ui::Button    * _buttonBuy;
    cocos2d::ui::Button    * _buttonListView;
    cocos2d::ui::TextBMFont* _textBMPrice;
    cocos2d::ui::Text      * _textBMDescrible;
    cocos2d::ui::Text      * _textBMConditionOpenPreviousTable;

    int                  _id;
    std::string          _name;
    long                 _price;
    LobbyTableNew::State _state;
    float                _percentLucky;
public:
CC_SYNTHESIZE(LobbyScene
                      *, _lobbyScene, LobbyScene);
CC_SYNTHESIZE(LobbyShopTableBoard
                      *, _lobbyShopTableBoard, LobbyShopTableBoard);

    static cocos2d::Vector<LobbyTableNew*> _listTable;

    ~LobbyTableNew();
    LobbyTableNew(LobbyScene* lobbyScene, LobbyShopTableBoard* lobbyShopTableBoard, cocos2d::ui::Layout* layoutTable);
    void initValue(int id, const std::string& name, long price, LobbyTableNew::State state, float percentLucky);
    static void setListTable(const cocos2d::Vector<LobbyTableNew*>& listTable);

    int getId();
    const std::string& getName();
    void setName(const std::string& name);
    void setPrice(long price);
    State getState();
    void setState(State state);
    void setPercentLucky(float percentLucky);
    void setTextDescrible();
    void showBuyNotify(bool show);
    void select();
    void unlock();
    void updateDataFromUnlockCityOrTable(const rapidjson::Value& data);
    void changeLanguage(Language language);
    static LobbyTableNew* getLobbyTableByID(int id);
};

#endif // __LOBBY_TABLE_NEW_H__