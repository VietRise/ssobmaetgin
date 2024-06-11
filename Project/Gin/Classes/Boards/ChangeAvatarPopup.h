#ifndef __CHANGE_AVATAR_POPUP_H__
#define __CHANGE_AVATAR_POPUP_H__

#include "InterfacePopupBoard.h"
#include "Component/Avatar.h"

class AvatarItem;

typedef std::function<void(AvatarItem* item)> AvatarActionCallback;

class ChangeAvatarPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout* _layoutContent;

    cocos2d::ui::Layout   * _layoutAvatar;
    cocos2d::ui::Button   * _buttonSave;
    cocos2d::ui::TextField* _textfieldName;
    cocos2d::ui::Text     * _textTitle;
    cocos2d::ui::Button   * _buttonClose;
    cocos2d::ui::Button   * _buttonDevice;

    std::vector<std::string>     _listAvatar;
    cocos2d::Vector<AvatarItem*> _listAvatarItem;

    cocos2d::ui::ListView* _listViewContent;
    cocos2d::ui::Layout  * _layoutRowTemplate;
    cocos2d::ui::Layout  * _layoutAvatarTemplate;

    std::string _changeName;
    std::string _imgPath;

    virtual void changeLanguage(Language language) override;
public:

    ~ChangeAvatarPopup();
    ChangeAvatarPopup();

    virtual bool init() override;
    void initScreen();
    void initValue();

    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

    void onButtonClick(cocos2d::Ref* sender);
    void onAvatarAction(AvatarItem* item);
    void initListAvatar();
    void changeAvatar(const std::string& image);
    void selectAvatar(AvatarItem* item);
    void saveChange();

    CREATE_FUNC(ChangeAvatarPopup);
};

#endif // ! __CHANGE_AVATAR_POPUP_H__

#ifndef __AVATAR_ITEM_H__
#define __AVATAR_ITEM_H__

class AvatarItem : public cocos2d::Ref
{
private:
    cocos2d::ui::Layout  * _layout;
    cocos2d::ui::CheckBox* _checkBox;
    float                _scale;
    AvatarActionCallback _callback;
public:
    std::string _image;

    ~AvatarItem();
    AvatarItem(cocos2d::ui::Layout* layout);
    void initValue(const AvatarActionCallback& callback);
    void setChecked(bool checked);
    void setAvatar(const std::string& image);
    void onClick(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
};

#endif // __AVATAR_ITEM_H__