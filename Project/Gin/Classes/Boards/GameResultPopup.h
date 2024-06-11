#ifndef __GAME_RESULT_POPUP_H__
#define __GAME_RESULT_POPUP_H__

#include "InterfacePopupBoard.h"
#include "Component/Avatar.h"
#include "Define/GameDefine.h"
#include "Define/InGameDefine.h"

class GameResultItem;

class GameResultPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout* _layoutContent;
    cocos2d::ui::Layout* _layoutGameResultTemp;
    std::vector<cocos2d::ui::Text*>   _listTitle;
    std::vector<cocos2d::ui::Layout*> _listGameResultLayout;
    cocos2d::Vector<GameResultItem*>  _listGameResultItem;
    cocos2d::ui::Button* _buttonOK;
public:
    ~GameResultPopup();
    GameResultPopup();

    virtual bool init() override;
    void initScreen();
    void loadData(std::vector<PlayerResultData> listUserData);
    void onButtonClick(cocos2d::Ref* sender);

    CREATE_FUNC(GameResultPopup);

    static std::map<int, int> s_mapResultValue;
};

class GameResultItem : public cocos2d::Ref
{
private:
    cocos2d::ui::Layout    * _layoutRoot;
    cocos2d::ui::Layout    * _layoutAvatar;
    Avatar                 * _avatar;
    cocos2d::ui::Text      * _textUsername;
    cocos2d::ui::ImageView * _imageResult;
    cocos2d::ui::TextBMFont* _textResult;
    std::vector<cocos2d::ui::TextBMFont*> _listTextValue;
    PlayerResultData                      _data;
public:
    ~GameResultItem();
    GameResultItem();

    static GameResultItem* create();
    void initLayout(cocos2d::ui::Layout* layoutResultTemp);
    void loadData(PlayerResultData data);
    cocos2d::ui::Layout* getLayoutRoot();
};

#endif // __GAME_RESULT_POPUP_H__