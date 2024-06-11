#include "MyActionsManager.h"
#include "Define/GameDefine.h"
#include "Helper/GameUtils.h"

USING_NS_CC;
using namespace std;

static MyActionsManager* _instance = nullptr;

MyActionsManager::~MyActionsManager()
{
    _mapMoneyAction.clear();
}

MyActionsManager::MyActionsManager()
{
}

MyActionsManager* MyActionsManager::getInstance()
{
    if (!_instance)
        _instance = new MyActionsManager();
    return _instance;
}

void MyActionsManager::destroyInstance()
{
    if (_instance)
        CC_SAFE_DELETE(_instance);
}

Action* MyActionsManager::createActionMoneyChange(cocos2d::ui::TextBMFont* moneyLabel, float duration, int step, double moneyFrom, double moneyTo, std::string preFix, std::string posFix)
{
    setNodeMoneyAction(moneyLabel, 0);
    double stepMoney = (moneyTo - moneyFrom) / step;
    float  stepdelay = duration / step;
    CallFunc* preCallback = CallFunc::create([preFix, posFix, moneyLabel, moneyFrom, this]() { moneyLabel->setString(StringUtils::format("%s%.0lf%s", preFix.c_str(), moneyFrom, posFix.c_str())); });
    CallFunc* posCallback = CallFunc::create([preFix, posFix, moneyLabel, moneyTo, this]() {
        moneyLabel->setString(GameUtils::MoneyFormat(moneyTo, preFix.c_str()));
        removeNodeAction(moneyLabel);
    });
    CallFunc* onCallback  = CallFunc::create([preFix, posFix, moneyLabel, stepMoney, moneyTo, moneyFrom, this]() {
        setNodeMoneyAction(moneyLabel, getNodeMoneyAction(moneyLabel) + 1);
        double newMoneyFrom = getNodeMoneyAction(moneyLabel) * stepMoney + moneyFrom;

        if ((moneyFrom <= moneyTo && newMoneyFrom <= moneyTo) || (moneyFrom >= moneyTo && newMoneyFrom >= moneyTo))
            moneyLabel->setString(StringUtils::format("%s%s%s", preFix.c_str(), GameUtils::MoneyFormat(newMoneyFrom).c_str(), posFix.c_str()));
    });
    moneyLabel->stopActionByTag(1244);
    Repeat  * repeatAction = Repeat::create(Sequence::create(DelayTime::create(stepdelay), onCallback, nullptr), step);
    Sequence* seqAction    = Sequence::create(preCallback, repeatAction, posCallback, nullptr);
    seqAction->setTag(1244);
    moneyLabel->runAction(seqAction);
    return seqAction;
}

double MyActionsManager::getNodeMoneyAction(cocos2d::Node* node)
{
    for (const std::pair<Node*, int>& pair : _mapMoneyAction)
        if (pair.first == node)
            return pair.second;
    return 0;
}

void MyActionsManager::setNodeMoneyAction(cocos2d::Node* node, double money)
{
    Node                            * nodeMoney = nullptr;
    for (const std::pair<Node*, int>& pair : _mapMoneyAction)
        if (pair.first == node)
            nodeMoney = node;

    if (nodeMoney == nullptr)
        _mapMoneyAction.push_back(make_pair(node, money));
    else
    {
        for (std::pair<Node*, int>& pair : _mapMoneyAction)
            if (pair.first == nodeMoney)
                pair.second = money;
    }
}

void MyActionsManager::removeNodeAction(cocos2d::Node* node)
{
    for (auto iter = this->_mapMoneyAction.begin(); iter != this->_mapMoneyAction.end();)
    {
        if (iter->first == node)
            iter = this->_mapMoneyAction.erase(iter);
        else
            ++iter;
    }
}