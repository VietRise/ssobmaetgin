#ifndef __MAILBOX_CONTENT_POPUP_H__
#define __MAILBOX_CONTENT_POPUP_H__

#include "InterfacePopupBoard.h"
#include <functional>

class MailboxContentPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text  * _textTitle;
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonClose;
public:
    ~MailboxContentPopup();
    MailboxContentPopup();

    virtual bool init() override;
    void initScreen();
    void initValue(const std::string& title, const std::string& content);
    void onButtonClick(cocos2d::Ref* sender);

    CREATE_FUNC(MailboxContentPopup);
};

#endif // ! __MAILBOX_CONTENT_POPUP_H__