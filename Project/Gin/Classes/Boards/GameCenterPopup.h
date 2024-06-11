#ifndef __GAME_CENTER_POPUP_H__
#define __GAME_CENTER_POPUP_H__

#include "InterfacePopupBoard.h"

enum GameCenterItemState
{
    NotInstall,
    ComingSoon,
    Installed,
    Playing
};

typedef struct
{
    std::string _name;
    std::string _packageID;
    std::string _avatarUrl;
    int         _versionCode;
    double      _price;
    long long   _releaseTime;
    int         _isHighlight;
    std::string _link;
    int         _state;
} GameCenterData;

typedef std::function<void(const GameCenterData& gameCenterData)> GameCenterItemClickCallback;

class GameCenterItem;

class GameCenterPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text    * _textTitle;
    cocos2d::ui::Button  * _buttonClose;
    cocos2d::ui::ListView* _listViewGames;

    cocos2d::Vector<GameCenterItem*> _listGameItems;

    void onCloseButtonClick(Ref* sender);
protected:
public:
    static std::string                 _dataGameCenterJson;
    static int                         _versionGameCenter;
    static std::vector<GameCenterData> _listGameItemsData;

    ~GameCenterPopup();
    GameCenterPopup();

    virtual bool init() override;
    virtual void onEnter() override;

    void initScreen();
    void loadItems();
    void onGameCenterItemClick(const GameCenterData& gameCenterData);

    static void loadItemsData();
    static bool isNew();
    static GameCenterData getGameCenterData(const std::string& id);

    CREATE_FUNC(GameCenterPopup);
};

// ------------- Game Center Item -----------------
class GameCenterItem : public cocos2d::Ref
{
private:
    cocos2d::ui::Layout   * _layoutRoot;
    cocos2d::ui::Layout   * _layoutAvatar;
    cocos2d::Sprite       * _spriteAvatar;
    cocos2d::ui::Text     * _textName;
    cocos2d::ui::Text     * _textPrice;
    cocos2d::ui::Button   * _btnStatus;
    cocos2d::ui::Text     * _textStatus;
    cocos2d::ui::ImageView* _imageViewNew;

    GameCenterData      _data;
    GameCenterItemState _state;
public:
CC_SYNTHESIZE(GameCenterItemClickCallback, _callback, Callback);
    ~GameCenterItem();
    GameCenterItem(const GameCenterData& data);
    void initLayout(cocos2d::ui::Layout* layoutRoot);
    void onButtonClick(cocos2d::Ref* pSender);
    static GameCenterItem* create(cocos2d::ui::Layout* layoutRoot, const GameCenterData& data);
};

#endif /* __GAME_CENTER_POPUP_H__ */
