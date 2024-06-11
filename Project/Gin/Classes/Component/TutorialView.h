#ifndef __TUTORIAL_VIEW_H__
#define __TUTORIAL_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class TutorialView : public cocos2d::Layer
{
private:
    cocos2d::Size screenSize;
    cocos2d::ui::Layout  * _layoutRoot;
    cocos2d::ui::Button  * _buttonClose;
    cocos2d::ui::PageView* _pageviewContent;
    cocos2d::ui::Button  * _buttonMoveLeft;
    cocos2d::ui::Button  * _buttonMoveRight;
    std::vector<cocos2d::ui::Button*> _listButton;

    int _currentPageIdx;

    virtual bool onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    void onButtonMovePage(Ref* sender);
    void updateButtonMove();
protected:
public:
    TutorialView();
    ~TutorialView();

    virtual bool init() override;
    void initLayout();
    virtual void onEnter() override;

    void onCloseButtonClick(Ref* sender);
    void onPageViewEvent(cocos2d::Ref*, cocos2d::ui::PageView::EventType eventType);
    void setContent(const cocos2d::Vector<cocos2d::ui::Layout*>& listContent);
    virtual void update(float delta) override;
    void updateButtons();
    void onButtonClick(Ref* sender);
    void selectButton(int index);

    void open();
    void close();

    CREATE_FUNC(TutorialView);
};

#endif //__TUTORIAL_VIEW_H__
