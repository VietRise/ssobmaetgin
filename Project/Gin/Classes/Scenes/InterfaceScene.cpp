#include "InterfaceScene.h"
#include "Boards/InterfacePopupBoard.h"

USING_NS_CC;
using namespace ui;

InterfaceScene::~InterfaceScene()
{
    ResourcesManager::getInstance()->unloadUnusedResources();
}

InterfaceScene::InterfaceScene()
{
    _screenSize = Director::getInstance()->getWinSize();
}

bool InterfaceScene::init()
{
    if (!Layer::init())
        return false;
    return true;
}

void InterfaceScene::onEnter()
{
    Layer::onEnter();
    ResourcesManager::getInstance()->loadResources(CC_CALLBACK_0(InterfaceScene::onLoadResourcesSyncDone, this), CC_CALLBACK_0(InterfaceScene::onLoadResourcesAsyncDone, this));
}

void InterfaceScene::onExit()
{
    Layer::onExit();
}

void InterfaceScene::removePopup(int boardId)
{
    Node* node = getChildByTag((int) boardId);
    if (node != nullptr && dynamic_cast<InterfacePopupBoard*>(node) != nullptr)
    {
        InterfacePopupBoard* popup = (InterfacePopupBoard*) node;
        if (popup->getParent() != nullptr)
            popup->close(true);
    }
}
