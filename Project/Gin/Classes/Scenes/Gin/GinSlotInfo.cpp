#include "GinSlotInfo.h"

using namespace std;

GinSlotInfo::GinSlotInfo()
{
	this->_userName = "";
	this->_isReady = false;
	this->_slotIdx = -1;
	this->_isMain = false;
	this->_isDone = false;
	_money = 0;
}

GinSlotInfo::~GinSlotInfo()
{
}

void GinSlotInfo::setUserName(const std::string& userName)
{
    setNeedUpdate(true);
	this->_userName = userName;
}

std::string GinSlotInfo::getUserName()
{
	return this->_userName;
}

void GinSlotInfo::setAvatarLink(const std::string& avatarLink)
{
    setNeedUpdate(true);
	this->_avatarLink = avatarLink;
}

string GinSlotInfo::getAvatarLink()
{
	return this->_avatarLink;
}

void GinSlotInfo::setMoney(long long money)
{
    setNeedUpdate(true);
	this->_money = money;
}

long long GinSlotInfo::getMoney()
{
	return this->_money;
}

void GinSlotInfo::setSlotIdx(int slotIdx)
{
	this->_slotIdx = slotIdx;
}

int GinSlotInfo::getSlotIdx()
{
	return this->_slotIdx;
}

void GinSlotInfo::setLevel(int level)
{
	_level = level;
}

int GinSlotInfo::getLevel()
{
	return _level;
}

void GinSlotInfo::setMain(bool main)
{
	this->_isMain = main;
}

bool GinSlotInfo::isMain()
{
	return this->_isMain;
}

void GinSlotInfo::setDone(bool done)
{
	this->_isDone = done;
}

bool GinSlotInfo::isDone()
{
	return this->_isDone;
}

void GinSlotInfo::copyFrom(UserData* data)
{
    setUserName(data->getUserName());
    setAvatarLink(data->getAvatarLink());
    setMoney(data->getMoney());
    setLevel(data->getLevel());
    setLevelCurrentExp(data->getLevelExp());
    setLevelTotalExp(data->getNextLevelExp());
}

void GinSlotInfo::setNeedUpdate(bool needUpdate)
{
	this->_needUpdate = needUpdate;
}

bool GinSlotInfo::needUpdate()
{
	return this->_needUpdate;
}

void GinSlotInfo::setReady(bool ready)
{
	setNeedUpdate(true);
	this->_isReady = ready;
}

bool GinSlotInfo::isReady()
{
	return this->_isReady;
}

void GinSlotInfo::setLevelCurrentExp(int level)
{
    _levelCurrentExp = level;
}

int GinSlotInfo::getLevelCurrentExp()
{
    return _levelCurrentExp;
}

void GinSlotInfo::setLevelTotalExp(int level)
{
    _levelTotalExp = level;
}

int GinSlotInfo::getLevelTotalExp()
{
    return _levelTotalExp;
}
