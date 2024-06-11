#ifndef __THROW_CARD_DETAIL_POPUP_H__
#define __THROW_CARD_DETAIL_POPUP_H__

#include "InterfacePopupBoard.h"

class ThrowCardDetailPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout* _layoutCardTemp;
    cocos2d::ui::Button* _buttonClose;
    std::vector<cocos2d::ui::ListView*> _listListViewNumberCard;
public:
    ~ThrowCardDetailPopup();
    ThrowCardDetailPopup();

    virtual bool init() override;
    void initScreen();
    void loadData(std::vector<int> listThrowCardIndex);
    void onButtonClick(cocos2d::Ref* sender);
    virtual void open();
    virtual void close(bool forceClose = false);

    CREATE_FUNC(ThrowCardDetailPopup);
};

#endif // __THROW_CARD_DETAIL_POPUP_H__