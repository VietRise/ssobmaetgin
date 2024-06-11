#include "TutorialView.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Manager/SoundManager.h"
#include "Define/InGameDefine.h"

USING_NS_CC;
using namespace ui;

TutorialView::TutorialView()
{
    _currentPageIdx = 0;
}

TutorialView::~TutorialView()
{
}

bool TutorialView::init()
{
    if (!Layer::init())
        return false;
    initLayout();
    return true;
}

void TutorialView::initLayout()
{
    setKeypadEnabled(true);
    screenSize = Director::getInstance()->getWinSize();
    setContentSize(screenSize);
    Node* node  = CSLoader::createNode("ui/Common/TutorialLayout.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();

    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setContentSize(screenSize);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));
    this->addChild(_layoutRoot);
    ui::Helper::doLayout(_layoutRoot);
	 if (s_enableSafeArea)
    {
        float padding = SAFE_AREA_PADDING;

        LayoutComponent* layoutComponent = (LayoutComponent*) static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutContent"))->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + padding);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + padding);
        layoutComponent->refreshLayout();

    }
    ui::Helper::doLayout(this);
	
    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        float scale = 1.0f;
        if (s_resolution_ratio <= RESOLUTION_1_4)
            scale = RESOLUTION_1_4_SCALE;
        else if (s_resolution_ratio <= RESOLUTION_1_6)
            scale = RESOLUTION_1_6_SCALE;

        auto layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutContent"));
        layoutContent->setScale(scale);

        Size size = screenSize / scale;

        float paddingWidth  = -1 * (size.width - layoutContent->getContentSize().width) / 2;
        float paddingHeight = -1 * (size.height - layoutContent->getContentSize().height) / 2;

        LayoutComponent* layoutComponent = (LayoutComponent*) layoutContent->getComponent(__LAYOUT_COMPONENT_NAME);
        layoutComponent->setLeftMargin(layoutComponent->getLeftMargin() + paddingWidth);
        layoutComponent->setRightMargin(layoutComponent->getRightMargin() + paddingWidth);
        layoutComponent->setTopMargin(layoutComponent->getTopMargin() + paddingHeight);
        layoutComponent->setBottomMargin(layoutComponent->getBottomMargin() + paddingHeight);
        layoutComponent->refreshLayout();

        ui::Helper::doLayout(layoutContent);
    }
    ui::Helper::doLayout(_layoutRoot);

    this->_pageviewContent = static_cast<PageView*>(Helper::seekWidgetByName(_layoutRoot, "PageViewContent"));
    this->_pageviewContent->enableDpadNavigation(true);
    this->_pageviewContent->setSwallowTouches(true);
    this->_pageviewContent->addEventListener(CC_CALLBACK_2(TutorialView::onPageViewEvent, this));
    ui::Helper::doLayout(this->_pageviewContent);
    this->_buttonClose = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonClose"));
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(TutorialView::onCloseButtonClick, this));

    this->_buttonMoveLeft = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonMoveLeft"));
    this->_buttonMoveLeft->addClickEventListener(CC_CALLBACK_1(TutorialView::onButtonMovePage, this));

    this->_buttonMoveRight = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonMoveRight"));
    this->_buttonMoveRight->addClickEventListener(CC_CALLBACK_1(TutorialView::onButtonMovePage, this));

    auto layoutButtons = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelButton"));
    auto button        = static_cast<Button*>(Helper::seekWidgetByName(layoutButtons, "Button"));
    button->setVisible(false);
    ui::Helper::doLayout(this);
}

void TutorialView::onEnter()
{
    Layer::onEnter();
    scheduleUpdate();
    updateButtonMove();
    open();
}

void TutorialView::onCloseButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    close();
}

void TutorialView::onButtonMovePage(Ref* sender)
{
    int currentIndex = this->_pageviewContent->getCurrentPageIndex();
    if (sender == this->_buttonMoveLeft)
    {
        int pre = currentIndex - 1;
        if (pre >= 0)
        {
            this->_pageviewContent->scrollToPage(pre);
            updateButtonMove();
            selectButton(pre);
        }
    }
    else if (sender == this->_buttonMoveRight)
    {
        int nex = currentIndex + 1;
        if (nex < this->_pageviewContent->getItems().size())
        {
            this->_pageviewContent->scrollToPage(nex);
            updateButtonMove();
            selectButton(nex);
        }

    }
    updateButtonMove();
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void TutorialView::updateButtonMove()
{
    int currentIndex = this->_pageviewContent->getCurrentPageIndex();
    int pre          = currentIndex - 1;
    int nex          = currentIndex + 1;
    _currentPageIdx = currentIndex;
    if (pre < 0)
        this->_buttonMoveLeft->setVisible(false);
    else
        this->_buttonMoveLeft->setVisible(true);

    if (nex >= this->_pageviewContent->getItems().size())
        this->_buttonMoveRight->setVisible(false);
    else
        this->_buttonMoveRight->setVisible(true);
}

void TutorialView::onPageViewEvent(cocos2d::Ref*, cocos2d::ui::PageView::EventType eventType)
{
    switch (eventType)
    {
        case PageView::EventType::TURNING:
            updateButtonMove();
            selectButton(this->_pageviewContent->getCurrentPageIndex());
            break;
        default:
            break;
    }
}

void TutorialView::setContent(const cocos2d::Vector<cocos2d::ui::Layout*>& listContent)
{
    this->_pageviewContent->removeAllPages();
    for (Button* layout : _listButton)
        layout->removeFromParent();
    _listButton.clear();
    auto layoutButtons = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelButton"));
    auto button        = static_cast<Button*>(Helper::seekWidgetByName(layoutButtons, "Button"));
    for (Layout* layout : listContent)
    {
        this->_pageviewContent->addPage(layout);
        auto button1 = (Button*) button->clone();
        button1->setTag(this->_pageviewContent->getItems().size() - 1);
        auto label = static_cast<Text*>(Helper::seekWidgetByName(button1, "Label"));
        label->setString(layout->getName());
        layoutButtons->addChild(button1);
        _listButton.push_back(button1);
        button1->addClickEventListener(CC_CALLBACK_1(TutorialView::onButtonClick, this));
    }
    selectButton(this->_pageviewContent->getCurrentPageIndex());
    updateButtons();
    ui::Helper::doLayout(this);
}

void TutorialView::update(float delta)
{
    if (_currentPageIdx != this->_pageviewContent->getCurrentPageIndex())
        updateButtonMove();
}

bool TutorialView::onTouchBegan(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
    return false;
}

void TutorialView::onTouchMoved(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

void TutorialView::onTouchEnded(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

void TutorialView::onTouchCancelled(cocos2d::Touch* ptouch, cocos2d::Event* pEvent)
{
}

void TutorialView::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
        {
            onCloseButtonClick(nullptr);
            event->stopPropagation();
            break;
        }
        default:
            break;
    }
}

void TutorialView::updateButtons()
{
    int  lenght        = _listButton.size();
    int  padding       = 15;
    auto layoutButtons = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelButton"));
    auto button        = static_cast<Button*>(Helper::seekWidgetByName(layoutButtons, "Button"));
    int  width         = lenght * button->getContentSize().width + (lenght - 1) * padding;
    layoutButtons->setContentSize(Size(width, layoutButtons->getContentSize().height));
    Helper::doLayout(layoutButtons);
    for (int i = 0; i < _listButton.size(); i++)
    {
        auto button2 = _listButton.at(i);
        button2->setPositionX(i * (button2->getContentSize().width) + i * padding + button2->getContentSize().width / 2);
        button2->setVisible(true);
    }
}

void TutorialView::onButtonClick(Ref* sender)
{
    if (sender != nullptr)
    {
        Button* button = (Button*) sender;
        int index = button->getTag();
        selectButton(index);
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    }
}

void TutorialView::selectButton(int index)
{
    this->_pageviewContent->scrollToPage(index);
    for (int i = 0; i < _listButton.size(); i++)
    {
        auto bt = _listButton.at(i);
        if (bt->getTag() == index)
            bt->setEnabled(false);
        else
            bt->setEnabled(true);
    }
}

void TutorialView::open()
{
    _layoutRoot->setScale(1.0f - 0.25f);
    _layoutRoot->stopAllActions();
    _layoutRoot->setOpacity(0);
    _layoutRoot->runAction(Sequence::create(Spawn::create(FadeIn::create(0.2f), ScaleTo::create(0.1f, 1.0f), nullptr), nullptr));
}

void TutorialView::close()
{
    _layoutRoot->stopAllActions();
    CallFunc* endAnimFunc = CallFunc::create([=] {
        this->removeFromParent();
    });
    _layoutRoot->runAction(Sequence::create(Spawn::create(ScaleBy::create(0.1f, 0.95f), FadeOut::create(0.1f), nullptr), endAnimFunc, nullptr));
}
