#ifndef __ACTION_CONFIRM_POPUP_H__
#define __ACTION_CONFIRM_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

enum class ActionConfirmResult
{
    CONFIRM_ACTION,
    CONFIRM_OK
};

#define ACTION_CONFIRM_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this, _1)
typedef std::function<void(ActionConfirmResult result)> ActionConfirmCallback;

class ActionConfirmPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonOk;
    cocos2d::ui::Button* _buttonAction;

    ActionConfirmCallback _callback;

    void onActionButtonClick(Ref* sender);
    void onOkButtonClick(Ref* sender);
protected:
public:
    ActionConfirmPopup();
    ~ActionConfirmPopup();

    virtual bool init() override;
    void initScreen();
    void setSize(float width, float height);
    void initValue(const std::string& content, const std::string& strOk, const std::string& strAction, const ActionConfirmCallback& callback);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    CREATE_FUNC(ActionConfirmPopup);
};

#endif //__ACTION_CONFIRM_POPUP_H__