#ifndef __RATING_POPUP_THANK_H__
#define __RATING_POPUP_THANK_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"

typedef std::function<void()> RatingPopupThankOkCallback;

class RatingPopupThank : public InterfacePopupBoard
{
public:
    enum Type
    {
        THANK_LOW_STAR,
        THANK_HIGH_STAR
    };

private:
    cocos2d::ui::TextBMFont* _textTitle;
    cocos2d::ui::Text      * _textContent;
    cocos2d::ui::Button    * _buttonOK;
    Type _type;
    void onOKButtonClick(Ref* sender);
protected:
public:
CC_SYNTHESIZE(RatingPopupThankOkCallback, _callbackOK, CallbackOK);
    ~RatingPopupThank();
    RatingPopupThank();

    void setType(RatingPopupThank::Type type);
    virtual bool init() override;
    void initScreen();
    CREATE_FUNC(RatingPopupThank);
};

#endif /* __RATING_POPUP_THANK_H__ */
