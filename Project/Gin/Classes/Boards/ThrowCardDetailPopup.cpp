#include "ThrowCardDetailPopup.h"
#include "Helper/GameUtils.h"
#include "Define/GameDefine.h"
#include "Scenes/Gin/GinCard.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

ThrowCardDetailPopup::~ThrowCardDetailPopup()
{
    CC_SAFE_RELEASE(_layoutCardTemp);
}

ThrowCardDetailPopup::ThrowCardDetailPopup()
{
}

bool ThrowCardDetailPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void ThrowCardDetailPopup::initScreen()
{
    this->setContentSize(_screenSize);
    _applyBackKey = true;
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/ThrowCardDetailPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _buttonClose = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutRoot, "ButtonClose"));
    _buttonClose->addClickEventListener(CC_CALLBACK_1(ThrowCardDetailPopup::onButtonClick, this));

    auto     layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "PanelContent"));
    for (int i             = 1; i <= MAXIMUM_NUMBER_SUIT; i++)
    {
        auto layoutSuit         = static_cast<Layout*>(Helper::seekWidgetByName(layoutContent, StringUtils::format("Panel%d", i)));
        auto listViewCardNumber = static_cast<ListView*>(Helper::seekWidgetByName(layoutSuit, "ListViewCardNumber"));
        listViewCardNumber->setScrollBarEnabled(false);
        _listListViewNumberCard.push_back(listViewCardNumber);
    }
    _layoutCardTemp = static_cast<Layout*>(node->getChildByName("PanelCard"));
    _layoutCardTemp->retain();
}

void ThrowCardDetailPopup::loadData(std::vector<int> listThrowCardIndex)
{
    std::sort(listThrowCardIndex.begin(), listThrowCardIndex.end(), [=](const int& a, const int& b) { return a > b; });
    for (int i = 0; i < _listListViewNumberCard.size(); i++)
        _listListViewNumberCard.at(i)->removeAllItems();
    for (int i = 0; i < listThrowCardIndex.size(); i++)
    {
        int indexCard = listThrowCardIndex.at(i);
        int suit      = indexCard % MAXIMUM_NUMBER_SUIT;
        Layout     * layoutCard = (Layout*) _layoutCardTemp->clone();
        // Create card
        GinCard* card       = GinCard::create(nullptr);
        card->setCardIndex(indexCard, true);
        card->setCardValue(indexCard);
        card->setBack(1);
        card->setScale(0.44f);
        card->open();
        card->setPosition(Vec2(layoutCard->getContentSize().width * 0.5f, layoutCard->getContentSize().height * 0.5f - 1.0f));
        layoutCard->addChild(card);
        _listListViewNumberCard.at(MAXIMUM_NUMBER_SUIT - 1 - suit)->pushBackCustomItem(layoutCard);
    }
}

void ThrowCardDetailPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        close();
}

void ThrowCardDetailPopup::open()
{
    float scale = 1.0f;
    if (s_resolution_ratio <= RESOLUTION_1_4)
        scale = RESOLUTION_1_4_SCALE;
    else if (s_resolution_ratio <= RESOLUTION_1_6)
        scale = RESOLUTION_1_6_SCALE;
    _layoutRoot->setScale(scale - 0.25f);
    _state = State::OPENED;
    _layoutRoot->stopAllActions();
    _layoutRoot->runAction(Sequence::create(ScaleTo::create(0.1f, scale), nullptr));
}

void ThrowCardDetailPopup::close(bool forceClose /*= false*/)
{
    if (forceClose == false)
    {
        _state = State::CLOSED;
        _layoutRoot->stopAllActions();
        CallFunc* endAnimFunc = CallFunc::create([=] {
            _state = State::CLOSED;
            this->removeFromParent();
        });
        _layoutRoot->runAction(Sequence::create(ScaleBy::create(0.1f, 0.95f), endAnimFunc, nullptr));
    }
    else
    {
        _state = State::CLOSED;
        _layoutRoot->stopAllActions();
        this->removeFromParent();
    }
}