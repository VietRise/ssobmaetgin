#ifndef __MAILBOX_POPUP_H__
#define __MAILBOX_POPUP_H__

#include "InterfacePopupBoard.h"
#include <functional>

class MailboxItem;

class LobbyScene;

class MailboxPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout  * _layoutMailboxItemTemplate;
    cocos2d::ui::ListView* _listViewContent;
    cocos2d::ui::Text    * _textTitle;
    cocos2d::ui::Text    * _textEmpty;

    LobbyScene* _lobbyScene;

    cocos2d::ui::Button* _buttonClose;
    cocos2d::Vector<MailboxItem*> _listMailboxItem;

    virtual void changeLanguage(Language language) override;
public:
    ~MailboxPopup();
    MailboxPopup();

    virtual bool init() override;
    void initScreen();
    void initValue(const rapidjson::Value& data, LobbyScene* _lobbyScene);
    void removeMailboxItems(const std::vector<std::pair<std::string, std::string>>& removeMailboxItems);
    bool isValidMailboxItem(const rapidjson::Value& data);

    virtual void onExit() override;
    void onButtonClick(cocos2d::Ref* sender);
    void onMailboxAction(MailboxItem* item);
    void setMailboxRead();

    CREATE_FUNC(MailboxPopup);
};

#endif // ! __MAILBOX_POPUP_H__