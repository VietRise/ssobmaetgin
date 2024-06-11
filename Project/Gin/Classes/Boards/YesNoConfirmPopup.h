#ifndef __YESNO_CONFIRM_POPUP_H__
#define __YESNO_CONFIRM_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

enum class YesNoConfirmResult
{
    CONFIRM_YES,
    CONFIRM_NO
};

typedef std::function<void(YesNoConfirmResult result)> YesNoCallback;

class YesNoConfirmPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonYes;
    cocos2d::ui::Button* _buttonNo;

    YesNoCallback _callback;

    void onYesButtonClick(Ref* sender);
    void onNoButtonClick(Ref* sender);
protected:
public:
    YesNoConfirmPopup();
    ~YesNoConfirmPopup();

    virtual bool init() override;
    void initScreen();

    void setCallback(const YesNoCallback& callback);
    void setContent(const std::string& content);
    void setSize(float width, float height);
    cocos2d::Size getSize();
    void setButtonLabel(const std::string& yesStr, const std::string& noStr);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;

    CREATE_FUNC(YesNoConfirmPopup);
};

#endif //__YESNO_CONFIRM_POPUP_H__