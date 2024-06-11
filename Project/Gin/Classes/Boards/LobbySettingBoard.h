#ifndef __LOBBY_SETTING_BOARD_H__
#define __LOBBY_SETTING_BOARD_H__

#include "InterfacePopupBoard.h"
#include <functional>

class LobbySettingBoard : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout* _layoutContent;
    cocos2d::ui::Text  * _textSettingsTitle;
    cocos2d::ui::Text  * _textLanguageTitle;
    cocos2d::ui::Text  * _textLanguageEN;
    cocos2d::ui::Text  * _textLanguageID;
    cocos2d::ui::Text  * _textSoundTitle;
    cocos2d::ui::Text  * _textSoundEffect;
    cocos2d::ui::Text  * _textSoundBackground;

    cocos2d::ui::CheckBox* _cbLanguageEN;
    cocos2d::ui::CheckBox* _cbLanguageID;
    cocos2d::ui::CheckBox* _cbSoundEffect;
    cocos2d::ui::CheckBox* _cbSoundBackground;

    cocos2d::ui::Button* _buttonClose;
    cocos2d::ui::Button* _buttonMoreGame;
    cocos2d::ui::Button* _buttonPolicy;
    cocos2d::ui::Button* _buttonShare;
    cocos2d::ui::Button* _buttonRate;
    cocos2d::ui::Button* _buttonExit;

    virtual void changeLanguage(Language language) override;
    void notifyChangeLanguage(Language language);
    void playChangeLanguageEffect();

    void selectLanguage(Language language, bool init = false);
    void selectSoundEffect(bool state, bool init = false);
    void selectSoundBackground(bool state, bool init = false);
public:
    ~LobbySettingBoard();
    LobbySettingBoard();

    virtual bool init() override;
    void initScreen();
    void initValue(int type = 0 /*0: lobby, 1: ingame*/);

    void onButtonClick(cocos2d::Ref* sender);
    void onCheckBoxClick(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType type);

    CREATE_FUNC(LobbySettingBoard);
};

#endif // !__LOBBY_SETTING_BOARD_H__
