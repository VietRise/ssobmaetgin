#ifndef __ROUND_POINT_POPUP_H__
#define __ROUND_POINT_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include "Define/InGameDefine.h"

class RoundPointPopup : public InterfacePopupBoard
{
private:
    cocos2d::Size _originalLayoutRootSize;
    cocos2d::ui::Button  * _buttonClose;
    cocos2d::ui::Button  * _buttonOk;
    cocos2d::ui::Button  * _buttonTutorial;
    cocos2d::ui::Layout  * _layoutPlayerTemplate;
    cocos2d::ui::Layout  * _layoutEachRowTemplate;
    cocos2d::ui::ListView* _listViewEachRound;
    cocos2d::ui::Layout  * _layoutRowHitpot;

    std::vector<cocos2d::ui::Layout*> _listLayoutPlayer;
    std::vector<cocos2d::ui::Layout*> _listLayoutDataRow;

    void onButtonClick(cocos2d::Ref* sender);
    void showTutorial();
protected:
public:
    static std::vector<PlayerResultData> _listUserDataCached;

    ~RoundPointPopup();
    RoundPointPopup();

    virtual bool init() override;
    void initScreen();
    void setRoomInfo(const std::string& roomName, double bet);
    void setPlayerInfo(const PlayerResultData& playerInfoData, int order);
    void loadData(const std::vector<PlayerResultData>& listUserData);

    CREATE_FUNC(RoundPointPopup);
};

#endif /* __ROUND_POINT_POPUP_H__ */

