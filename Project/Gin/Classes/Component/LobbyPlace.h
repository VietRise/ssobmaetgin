#ifndef __LOBBY_PLACE_H__
#define __LOBBY_PLACE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Define/GameDefine.h"
#include <vector>
#include "Manager/TextManager.h"

class LobbyScene;

class LobbyPlace : public cocos2d::Ref
{
public:
    enum class State
    {
        NORMAL             = 1,
        LOCK_CAN_UNLOCK    = 0,
        LOCK_CANNOT_UNLOCK = -1,
    };
private:
    LobbyScene         * _lobbyScene;
    cocos2d::ui::Layout* _layoutEntireCity;
    cocos2d::ui::Layout* _layoutCity;

    cocos2d::ui::Layout* _panelUnlock;
    cocos2d::ui::Layout* _panelLock;

    int               _id;
    LobbyPlace::State _state;
    std::string       _name;
    long              _price;
    float             _betJackpot;
    double            _requiredMoney;

    void changeLanguage(Language language);
    void onNotify(void* sender, int id, const cocos2d::Value& data);
public:
    static std::vector<LobbyPlace*> _listPlace;

    ~LobbyPlace();
    LobbyPlace(LobbyScene* lobbyScene, cocos2d::ui::Layout* layoutEntireCity);
    void initValue(int id, LobbyPlace::State state, const std::string& name, long price, float jackpotRatioBet, double requiredMoney);
    static void setListPlace(const std::vector<LobbyPlace*>& listPlace);

    int getID();
    void setID(int id);
    State getState();
    void setState(State state);
    const std::string& getName();
    void setName(const std::string& name);
    long getPrice();
    void setPrice(long price);
    void showBuyNotify(bool show);

    void select();
    void unlock();
    void touchBegin();
    void touchEnd();

    void reopenTicket();
    bool checkTouchCity(cocos2d::ui::Layout* layout);
    void fadeOut();
    void fadeIn();
    void playActionTouchBegin();
    void playActionTouchEnd();
    void playActionVibration();
    void stopActionVibration();

    static LobbyPlace* getLobbyPlaceByID(int id);
};

#endif // __LOBBY_PLACE_H__