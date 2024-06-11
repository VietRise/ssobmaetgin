#ifndef __FIGHT_CHALLENGER_POPUP_H__
#define __FIGHT_CHALLENGER_POPUP_H__

#include "InterfacePopupBoard.h"
#include "Scenes/Gin/GinSlotInfo.h"

#define TAG_BUTTON_FOLD         1
#define TAG_BUTTON_CHALLENGE    2
#define TAG_BUTTON_FIGHT        3

typedef std::function<void(int)> FightChallengerCallback;

class GPGinControlCardLayer;

class FightChallengerPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout* _layoutChallenge;
    cocos2d::ui::Layout* _layoutFight;
    cocos2d::ui::Button* _buttonFold;
    cocos2d::ui::Button* _buttonChallenge;
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonCancel;
    cocos2d::ui::Button* _buttonFight;

    bool _isRequested;
public:
CC_SYNTHESIZE(FightChallengerCallback, _fightChallengerCallback, FightChallengerCallback);
CC_SYNTHESIZE(GPGinControlCardLayer*, _gpGinControlCardLayerCallback, GPGinControlCardLayer);
CC_SYNTHESIZE_READONLY(bool, _isChallenge, IsChallenge);

    ~FightChallengerPopup();
    FightChallengerPopup();

    virtual bool init() override;
    void initScreen();
    void setChallenge(int currentScore);
    void setFight(int currentScore);
    virtual void changeLanguage(Language language) override;
    void onButtonClick(cocos2d::Ref* sender);
    void hideParticle();
    virtual void close(bool forceClose = false) override;
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;

    CREATE_FUNC(FightChallengerPopup);
};

#endif // __FIGHT_CHALLENGER_POPUP_H__