#include "GPGinSlotManager.h"
#include "../../ServerOffline/SvrCard.h"
#include "ServerOffline/Rules/RuleConstant.h"

USING_NS_CC;
using namespace std;

static GPGinSlotManager* _instance = nullptr;

GPGinSlotManager::GPGinSlotManager()
{
    _numSlot = 0;
}

GPGinSlotManager::~GPGinSlotManager()
{
}

GPGinSlotManager* GPGinSlotManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new GPGinSlotManager();
    return _instance;
}

void GPGinSlotManager::destroyInstance()
{
    CC_SAFE_DELETE(_instance);
}

void GPGinSlotManager::init()
{
}

void GPGinSlotManager::clearAllSlot()
{
    _listSlotInfo.clear();
}

void GPGinSlotManager::removeSlot(int slotIdx)
{
    for (int i = 0; i < this->_listSlotInfo.size(); i++)
    {
        if (_listSlotInfo.at(i)->getSlotIdx() == slotIdx)
        {
            _listSlotInfo.erase(i);
            break;
        }
    }
}

void GPGinSlotManager::setNumSlot(int slotNum)
{
    _numSlot = slotNum;
}

int GPGinSlotManager::getNumberSlot()
{
    return _numSlot;
}

int GPGinSlotManager::getNumberRemainCardsAfterDealCardFinish()
{
    return MAXIMUM_NUMBER_CARD - (_numSlot * RuleConstant::Common::MAX_HAND_CARD) - 1;
}

Vector<GinSlotInfo*> GPGinSlotManager::getSlotInfo()
{
    return this->_listSlotInfo;
}

GinSlotInfo* GPGinSlotManager::getSlotInfo(int slotIdx)
{
    for (int i = 0; i < this->_listSlotInfo.size(); i++)
        if (_listSlotInfo.at(i)->getSlotIdx() == slotIdx)
            return _listSlotInfo.at(i);
    return nullptr;
}

GinSlotInfo* GPGinSlotManager::getSlotInfoByOrderIdx(int orderIdx)
{
    for (int i = 0; i < this->_listSlotInfo.size(); i++)
        if (getSlotOrderIdx(_listSlotInfo.at(i)->getSlotIdx()) == orderIdx)
            return _listSlotInfo.at(i);
    return nullptr;
}

int GPGinSlotManager::getSlotIdx(int orderIdx)
{
    GinSlotInfo* ginSlotInfo = this->getSlotInfoByOrderIdx(orderIdx);
    if (ginSlotInfo != nullptr)
        return ginSlotInfo->getSlotIdx();
    return -1;
}

vector<int> GPGinSlotManager::getListReadySlotIdx()
{
    vector<int> listSlotIdx;
    for (int    i = 0; i < this->_listSlotInfo.size(); i++)
    {
        GinSlotInfo* slotInfo = this->_listSlotInfo.at(i);
        if (slotInfo->isReady())
            listSlotIdx.push_back(slotInfo->getSlotIdx());
    }
    return listSlotIdx;
}

vector<int> GPGinSlotManager::getListReadySlotOrderIdx(const std::vector<int>& listSlotIdx)
{
    vector<int> listOrderIdx;

    for (int i = 0; i < listSlotIdx.size(); i++)
    {
        for (GinSlotInfo* slotInfo : this->_listSlotInfo)
        {
            if (slotInfo->getSlotIdx() == listSlotIdx[i])
            {
                listOrderIdx.push_back(getSlotOrderIdx(slotInfo->getSlotIdx()));
                break;
            }
        }
    }
    return listOrderIdx;
}

std::vector<int> GPGinSlotManager::getListReadySlotOrderIdx()
{
    return getListReadySlotOrderIdx(getListReadySlotIdx());
}

int GPGinSlotManager::getSlotOrderIdx(int slotIdx)
{
    vector<int> arr;
    switch (_numSlot - 1)
    {
        case 0:
            arr = vector<int>{ -1, -1 };
            break;
        case 1:
            arr = vector<int>{ 0, 1 };
            break;
        default:
            break;
    }
    if (slotIdx >= 0 && slotIdx < arr.size())
        return arr[slotIdx];
    return 0;
}

void GPGinSlotManager::addSlot(GinSlotInfo slotInfo)
{
    int        slotIdx = slotInfo.getSlotIdx();
    for (int   i       = 0; i < this->_listSlotInfo.size(); i++)
    {
        GinSlotInfo* slot = this->_listSlotInfo.at(i);
        if (slot->getSlotIdx() == slotIdx)
        {
            GinSlotInfo* slot2 = new GinSlotInfo(slotInfo);
            slot2->autorelease();
            this->_listSlotInfo.replace(i, slot2);
            return;
        }
    }
    GinSlotInfo* slot3 = new GinSlotInfo(slotInfo);
    slot3->autorelease();
    this->_listSlotInfo.pushBack(slot3);
}

bool GPGinSlotManager::isSlotValid(int slotIdx)
{
    for (GinSlotInfo* slot : _listSlotInfo)
        if (slot->getSlotIdx() == slotIdx)
            return true;
    return false;
}

int GPGinSlotManager::getPreviousSlotIdx(int currentSlotIdx)
{
    int previousSlotIdx = currentSlotIdx + 1;
    previousSlotIdx = previousSlotIdx % _numSlot;
    return previousSlotIdx;
}

int GPGinSlotManager::getNextSlotIdx(int currentSlotIdx)
{
    int nextSlotIdx = currentSlotIdx - 1;
    if (nextSlotIdx < 0)
        nextSlotIdx = _numSlot - 1;
    return nextSlotIdx;
}
