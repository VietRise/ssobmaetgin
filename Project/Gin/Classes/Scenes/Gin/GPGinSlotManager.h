#ifndef __GPGinSlotManager__
#define __GPGinSlotManager__

#include <string>
#include <vector>
#include "cocos2d.h"
#include "GinSlotInfo.h"
#include "GPGinUserSlotNode.h"
#include "Define/GameDefine.h"

class GPGinSlotManager
{
private:
    int                               _numSlot;
	cocos2d::Vector<GinSlotInfo*> _listSlotInfo;
protected:
public:
    GPGinSlotManager();
    ~GPGinSlotManager();

    void init();
    static GPGinSlotManager* getInstance();
    static void destroyInstance();

    void setNumSlot(int slotNum);
    int getNumberSlot();
    int getNumberRemainCardsAfterDealCardFinish();
    cocos2d::Vector<GinSlotInfo*> getSlotInfo();
    GinSlotInfo* getSlotInfo(int slotIdx);
    GinSlotInfo* getSlotInfoByOrderIdx(int orderIdx);
    std::vector<int> getListReadySlotIdx();
    std::vector<int> getListReadySlotOrderIdx(const std::vector<int>& listSlotIdx);
    std::vector<int> getListReadySlotOrderIdx();
    int getSlotOrderIdx(int slotIdx);
    int getSlotIdx(int orderIdx);
    void clearAllSlot();
    void removeSlot(int slotIdx);
    void addSlot(GinSlotInfo slotInfo);
    bool isSlotValid(int slotIdx);
    int getPreviousSlotIdx(int currentIdx);
    int getNextSlotIdx(int currentIdx);
    cocos2d::Vector<GPGinUserSlotNode*> createSlotNode(cocos2d::ui::Layout* layout);
};

#endif //__GPGinSlotManager__
