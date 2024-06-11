#ifndef __NOTIFY_POPUP_H__
#define __NOTIFY_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

typedef std::function<void()> NotifyOkCallback;

class NotifyPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Button* _buttonOk;
    cocos2d::ui::Text  * _textContent;

    NotifyOkCallback _okCallback;

    void onButtonClick(cocos2d::Ref* sender);
protected:
public:
    NotifyPopup();
    ~NotifyPopup();

    virtual bool init() override;
    void initScreen();
    void initValue(const std::string& content, const NotifyOkCallback& callback = nullptr);
    void setSize(float width, float height);
    void setButtonLable(const std::string& content);

    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

    CREATE_FUNC(NotifyPopup);
};

#endif //__NOTIFY_POPUP_H__