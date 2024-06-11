#include "DebugLayer.h"
#include "Define/GameDefine.h"
#include "Native/CallToOtherLanguage.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace std;

static DebugLayer* _instance = nullptr;

DebugLayer::DebugLayer()
{
}

DebugLayer::~DebugLayer()
{
}

DebugLayer* DebugLayer::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = DebugLayer::create();
        _instance->retain();
    }
    return _instance;
}

void DebugLayer::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParent();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

bool DebugLayer::init()
{
    if (!Layer::init())
        return false;
    Size size = Director::getInstance()->getVisibleSize();

    Node* node  = CSLoader::createNode("ui/Common/DebugLayer.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();

    this->_layoutInterfaceRoot = static_cast<Layout*>(child);
    this->_layoutInterfaceRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutInterfaceRoot->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
    addChild(_layoutInterfaceRoot, kMiddleground);

    if (s_enableSafeArea)
    {
        float padding = SAFE_AREA_PADDING;

        LayoutComponent* layoutComponent = (LayoutComponent*) this->_layoutInterfaceRoot->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();
    }

    Helper::doLayout(this);

    _layoutDebug = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutDebug"));
    _buttonLog   = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonLog"));
    _buttonLog->addClickEventListener(CC_CALLBACK_1(DebugLayer::onOpenAndCloseClick, this));

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(DebugLayer::onOpenAndCloseClick, this));

    _buttonCopy = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonCopy"));
    _buttonCopy->addClickEventListener(CC_CALLBACK_1(DebugLayer::onCopyClick, this));

    _buttonClear = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonClear"));
    _buttonClear->addClickEventListener(CC_CALLBACK_1(DebugLayer::onClearClink, this));

    _listView = static_cast<ListView*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ListViewLog"));
    _listView->setSwallowTouches(false);
    _listView->setScrollBarAutoHideEnabled(false);
    onOpenAndCloseClick(nullptr);

    return true;
}

void DebugLayer::addText(const std::string& txt, const cocos2d::Color3B& color /*= Color3B(255, 255, 255)*/)
{
#if (USE_DEBUG_VIEW == 1)
    string text  = txt;
    string text2 = text;
    int    start = 0;
    int    step  = 180;
    while (step < text.length() - start)
    {
        start += step;
        text = text.insert(start, "\n");
    }

    string content = StringUtils::format("[%d]  %s", this->_listView->getItems().size() + 1, text.c_str());
    Size   size    = Director::getInstance()->getWinSize();

    Text* textBMFont = Text::create();
    //textBMFont->setFontName("");
    textBMFont->setFontSize(16);
    textBMFont->setColor(color);
    textBMFont->setString(content);
    textBMFont->setAnchorPoint(Vec2(0, 0));
    textBMFont->setTextAreaSize(Size(size.width, 0));

    this->_listView->pushBackCustomItem(textBMFont);
    this->_listView->jumpToBottom();

    _logString.push_back(text2);
#endif
}

void DebugLayer::onOpenAndCloseClick(Ref* sender)
{
    _layoutDebug->setVisible(!this->_layoutDebug->isVisible());
}

void DebugLayer::onClearClink(cocos2d::Ref* sender)
{
    this->_listView->removeAllItems();
    _logString.clear();
}

void DebugLayer::onCopyClick(cocos2d::Ref* sender)
{
    CallToOtherLanguage::getInstance()->copyToClipboard(getStringLog());
}

std::string DebugLayer::getStringLog()
{
    string   ret = "";
    for (int i   = 0; i < _logString.size(); i++)
    {
        ret += _logString[i];
        ret += '\n';
    }
    return ret;
}

void DebugLayer::attach(cocos2d::Node* node)
{
#if (USE_DEBUG_VIEW == 1)
    removeFromParent();
    node->addChild(this, 1000);
#endif
}
