#ifndef __MONEY_BOX_NODE_H__
#define __MONEY_BOX_NODE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MoneyBoxNode : public cocos2d::Node
{
private:
    cocos2d::ui::Layout* _rootLayout;
    bool _minus;
protected:
public:
    MoneyBoxNode();
    ~MoneyBoxNode();

    void setMoney(double money);
    void show(bool show);
    void reset();
    void playBonusRate(float bonusRate);
    virtual bool init();
    void setMinus(bool minus);

    CREATE_FUNC(MoneyBoxNode);
};

#endif //__MONEY_BOX_NODE_H__