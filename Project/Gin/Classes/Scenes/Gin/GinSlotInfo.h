#ifndef __GinSlotInfo__
#define __GinSlotInfo__

#include "cocos2d.h"
#include <string>
#include "UserData/UserData.h"

class GinSlotInfo: public cocos2d::Ref
{
private:
    std::string _userName;
    std::string _avatarLink;
    long long   _money;
    bool        _isReady;
    int         _slotIdx;
    bool        _isMain;
    bool        _isDone;
    bool        _needUpdate;

    int _level;
    int _levelCurrentExp;
    int _levelTotalExp;
protected:
public:
	GinSlotInfo();
	~GinSlotInfo();

    void setUserName(const std::string& userName);
    std::string getUserName();

    void setAvatarLink(const std::string& userName);
    std::string getAvatarLink();

    void setNeedUpdate(bool needUpdate);
    bool needUpdate();

    void setMoney(long long money);
    long long getMoney();

    void setSlotIdx(int slotIdx);
    int getSlotIdx();

    void setLevel(int level);
    int getLevel();

    void setMain(bool main);
    bool isMain();

    void setDone(bool done);
    bool isDone();

    void setReady(bool ready);
    bool isReady();

    void setLevelCurrentExp(int level);
    int getLevelCurrentExp();

    void setLevelTotalExp(int level);
    int getLevelTotalExp();

    void copyFrom(UserData* data);
};

#endif //__GinSlotInfo__
