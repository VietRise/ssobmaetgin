#ifndef __MAILBOX_ITEM_H__
#define __MAILBOX_ITEM_H__

#include "cocos2d.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include "Manager/TextManager.h"

class MailboxItem;

typedef std::function<void(MailboxItem* item)> MailboxActionCallback;

class MailboxItem : public cocos2d::Ref
{
private:
    cocos2d::ui::Button   * _buttonListView;
    cocos2d::ui::Layout   * _layout;
    cocos2d::ui::Button   * _buttonAction;
    cocos2d::ui::Button   * _buttonClaim;
    cocos2d::ui::ImageView* _imageNotify;

    MailboxActionCallback _actionCallback;
public:

    ~MailboxItem();
    MailboxItem(cocos2d::ui::Layout* layout);
    void initValue(const rapidjson::Value& data, const MailboxActionCallback& callback);
    void onButtonClick(cocos2d::Ref* sender);

    void setTitle(const std::string& title);
    void setContent(const std::string& content);
    void setFullContent(const std::string& content);
    void initIcon();
    void setRewards(const std::string& rewards);

    void setTimeStart(double startAt);
    void setTimeEnd(double endAt);

    const std::string& getId();
    const std::string& getType();
    cocos2d::ui::Layout* getLayout();

    std::string _id;
    std::string _type;
    std::string _data;
    std::string _title;
    std::string _content;
    std::string _fullContent;
};

#endif // __MAILBOX_ITEM_H__