#ifndef __DAILY_BONUS_POPUP_H__
#define __DAILY_BONUS_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

typedef std::function<void(double, double)> GetMoneyButtonCallback;

class DailyBonusPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::TextBMFont* _textContent;
    cocos2d::ui::Text      * _textSubContent;
    cocos2d::ui::ImageView * _imgTitle;
    cocos2d::ui::Layout    * _layoutButton;
    cocos2d::ui::Layout    * _layoutEventDecor;

    GetMoneyButtonCallback _buttonCallback;

    double _money;
    double _bonusMoney;

    cocos2d::FiniteTimeAction* createShowAction(float delay = 0);
    cocos2d::FiniteTimeAction* createHideAction(float delay = 0);

    virtual void changeLanguage(Language language) override;
    void onButtonTouch(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
protected:
public:
    DailyBonusPopup();
    ~DailyBonusPopup();

    virtual bool init() override;
    void initScreen();

    void show(cocos2d::Node* parent);
    void hide();

    void setContent(double money, double bonusMoney, double nextBonusMoney, const GetMoneyButtonCallback& callback);
    void checkAndEnableEventDecorLayer();
    CREATE_FUNC(DailyBonusPopup);
};

#endif //__DAILY_BONUS_POPUP_H__