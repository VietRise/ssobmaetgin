#ifndef __LEADERBOARD_POPUP_H__
#define __LEADERBOARD_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"

class LeaderboardPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Button* _buttonClose;
    cocos2d::ui::Button* _buttonLeaderboard1;
    cocos2d::ui::Button* _buttonLeaderboard2;
    cocos2d::ui::Button* _buttonLeaderboard3;
    cocos2d::ui::Text  * _textHint;
    cocos2d::ui::Text  * _textTitle;

    void onButtonClick(cocos2d::Ref* sender);
protected:
public:
    LeaderboardPopup();
    ~LeaderboardPopup();

    virtual bool init() override;
    void initScreen();
    virtual void changeLanguage(Language language) override;
    CREATE_FUNC(LeaderboardPopup);
};

#endif //__LEADERBOARD_POPUP_H__