#ifndef __SUGGEST_UPDATE_POPUP_H__
#define __SUGGEST_UPDATE_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>

class SuggestUpdatePopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Text  * _textContent;
    cocos2d::ui::Button* _buttonUpdate;
    cocos2d::ui::Button* _buttonLater;

    void onButtonClick(cocos2d::Ref* sender);
    virtual void changeLanguage(Language language) override;
protected:
public:
    SuggestUpdatePopup();
    ~SuggestUpdatePopup();

    virtual bool init() override;
    void initScreen();
    void initValue(const std::string& content);

    CREATE_FUNC(SuggestUpdatePopup);
};

#endif //__SUGGEST_UPDATE_POPUP_H__