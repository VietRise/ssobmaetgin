#include "CheatCardLayer.h"
#include "Define/GameDefine.h"
#include "Component/GinCardStore.h"
#include "Network/MessageSender.h"
#include "Helper/JsonHelper.h"
#include "Helper/CardIndexConverter.h"

USING_NS_CC;
using namespace ui;
using namespace std;

static CheatCardLayer* _instance = nullptr;

CheatCardLayer::CheatCardLayer()
{
    _userId = 0;
}

CheatCardLayer::~CheatCardLayer()
{
    CC_SAFE_RELEASE_NULL(_cardButton);
}

CheatCardLayer* CheatCardLayer::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = CheatCardLayer::create();
        _instance->retain();
    }
    return _instance;
}

void CheatCardLayer::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParent();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}

bool CheatCardLayer::init()
{
    if (!Layer::init())
        return false;
    Size screenSize = Director::getInstance()->getVisibleSize();

    Node* node  = CSLoader::createNode("ui/Common/CheatCardLayer.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();

    this->_layoutInterfaceRoot = static_cast<Layout*>(child);
    this->_layoutInterfaceRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutInterfaceRoot->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));
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

    _closeButton = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonClose"));
    _closeButton->setPressedActionEnabled(true);
    _closeButton->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onCloseButton, this));
    _closeButton->setVisible(false);

    _cardButton = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonCard"));
    _cardButton->setPressedActionEnabled(true);
    _cardButton->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onCardButtonClick, this));
    _cardButton->retain();
    _cardButton->removeFromParent();

    _cbFirstMove = static_cast<CheckBox*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "CheckBoxFirstMove"));
    _cbFirstMove->setVisible(false);
    _cbFirstMove->addEventListener(CC_CALLBACK_2(CheatCardLayer::onCheckBoxClick, this));

    Button* buttonClearCheat = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonClearCheat"));
    buttonClearCheat->setPressedActionEnabled(true);
    buttonClearCheat->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onClearCheatCardButton, this));

    _buttonCheatCard = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonCheatCard"));
    _buttonCheatCard->setPressedActionEnabled(true);
    _buttonCheatCard->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onCheatCardButton, this));

    _layoutCheatCards = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutCheatCard"));
    _layoutCheatCards->setVisible(false);

    _layoutCheatCombine = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "LayoutCheatCombine"));
    _layoutCheatCombine->setVisible(false);

    _textCheat = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "TextCheat"));
    _textCheat->setVisible(false);

    _textUserId = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "TextUserId"));
    _textUserId->setString("");

    _textField = static_cast<TextField*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "TextField"));
    _textField->setString("");

    _buttonApply = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutInterfaceRoot, "ButtonApply"));
    _buttonApply->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onApplyButton, this));
    initCardsButton();
    return true;
}

void CheatCardLayer::initCardsButton()
{
    Point startPos = Point(35, 200);

    for (int i = 0; i < 52; i++)
    {
        Button* buttonCard = (Button*) _cardButton->clone();
        buttonCard->setCascadeColorEnabled(true);
        Sprite* front = GinCardStore::getInstance()->getCardFront(i);
        front->setScale((buttonCard->getContentSize().width - 5) / front->getContentSize().width);
        front->setPosition(buttonCard->getContentSize() / 2);
        front->setCascadeColorEnabled(true);
        buttonCard->addChild(front, -1);

        Point pos = startPos + Point((i / 4) * (buttonCard->getContentSize().width + 3), 1 * ((i % 4) * (buttonCard->getContentSize().height + 3)));
        buttonCard->setPosition(pos);
        buttonCard->setTag(i);
        buttonCard->addClickEventListener(CC_CALLBACK_1(CheatCardLayer::onCardButtonClick, this));
        _layoutCheatCards->addChild(buttonCard);
        Text* textIndex = (Text*) Helper::seekWidgetByName(buttonCard, "TextIndex");
        textIndex->setString(cocos2d::StringUtils::format("%d", i));
        _arrCardButtonStruct[i] = CardButtonStruct(CardButtonState::STATE_NORMAL, buttonCard);
    }
}

void CheatCardLayer::onCardButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    int tag = button->getTag();

    if (_arrCardButtonStruct[tag]._state == CardButtonState::STATE_NORMAL)
        requestCheatCard(tag);
    else if (_arrCardButtonStruct[tag]._state == CardButtonState::STATE_CHEATED)
        requestCancelCheatCard(tag);
    else if (_arrCardButtonStruct[tag]._state == CardButtonState::STATE_DISABLE)
    {
        //do nothing
    }
}

void CheatCardLayer::onApplyButton(cocos2d::Ref* sender)
{
    string data = "[" + _textField->getString() + "]";
    if (data != "")
    {
        DocumentType document = JsonHelper::parseStringToDocumentType(data);
        if (document.IsArray() == true)
        {
            for (int i = 0; i < document.Size(); i++)
            {
                int index = document[i].GetInt();
                requestCheatCard(index);
            }
        }
    }
}

void CheatCardLayer::onCheatCardResponse(const rapidjson::Document& cheatCardsResponse)
{
    vector<int> listCheatedCardIdx;
    vector<int> listRemainCardIdx;

    int slotIdx = JsonHelper::getInt(cheatCardsResponse, KEY_CHEAT_CARD_USER_ID, -2);
    const rapidjson::Value& cheatcards  = JsonHelper::getValue(cheatCardsResponse, KEY_CHEAT_CARD_CHEATED_CARDS);
    const rapidjson::Value& remaincards = JsonHelper::getValue(cheatCardsResponse, KEY_CHEAT_CARD_REMAIN_CARDS);
    int firstMove = JsonHelper::getInt(cheatCardsResponse, KEY_CHEAT_CARD_FIRST_MOVE, -1);

    for (int i = 0; i < cheatcards.Size(); i++)
        listCheatedCardIdx.push_back(cheatcards[i].GetInt());
    for (int i = 0; i < remaincards.Size(); i++)
        listRemainCardIdx.push_back(remaincards[i].GetInt());

    setCheatedCard(slotIdx, listCheatedCardIdx, listRemainCardIdx);

    if (listCheatedCardIdx.size() > 0 || listRemainCardIdx.size() < 52 || firstMove != -1)
        _textCheat->setVisible(true);
    else
        _textCheat->setVisible(false);

    _cbFirstMove->setSelected(false);
    if (firstMove == _userId)
        _cbFirstMove->setSelected(true);
}

void CheatCardLayer::setCardButtonState(int userid, CardButtonStruct* cardButtonStruct, CardButtonState state, int order /*= -1*/)
{
    Button* cardButton = cardButtonStruct->_cardButton;
    cardButtonStruct->_state = state;
    if (state == CardButtonState::STATE_DISABLE) //disable
    {
        cardButton->setEnabled(false);
        cardButton->setColor(Color3B(100, 100, 100));

        if (userid == -2)
        {
            if (order == 0)
                cardButton->setColor(Color3B(126, 0, 4));
            else if (order == 1)
                cardButton->setColor(Color3B(126, 74, 0));
            else if (order == 2)
                cardButton->setColor(Color3B(124, 126, 0));
            else if (order == 3)
                cardButton->setColor(Color3B(14, 109, 0));
            else if (order == 4)
                cardButton->setColor(Color3B(0, 45, 109));
        }
    }
    else if (state == CardButtonState::STATE_NORMAL) //uncheat
    {
        cardButton->setEnabled(true);
        cardButton->setColor(Color3B::WHITE);
    }
    else if (state == CardButtonState::STATE_CHEATED) //cheated
    {
        cardButton->setEnabled(true);
        cardButton->setColor(Color3B::GREEN);
        if (userid == -2)
        {
            if (order == 0)
                cardButton->setColor(Color3B(255, 88, 94));
            else if (order == 1)
                cardButton->setColor(Color3B(255, 145, 88));
            else if (order == 2)
                cardButton->setColor(Color3B(255, 226, 88));
            else if (order == 3)
                cardButton->setColor(Color3B(114, 255, 88));
            else if (order == 4)
                cardButton->setColor(Color3B(88, 153, 255));
        }
    }
}

void CheatCardLayer::onCloseButton(cocos2d::Ref* sender)
{
    _layoutCheatCombine->setVisible(false);
    _layoutCheatCards->setVisible(false);
    _closeButton->setVisible(false);
    _cbFirstMove->setVisible(false);
}

void CheatCardLayer::onCheatCardButton(cocos2d::Ref* sender)
{
    _userId = 0;
    _textUserId->setString(StringUtils::format("UserId: %d", _userId).c_str());
    _layoutCheatCombine->setVisible(false);
    _layoutCheatCards->setVisible(true);
    _closeButton->setVisible(true);
    _cbFirstMove->setVisible(true);
    CheatCardLayer::getInstance()->requestCheatCard(-1);
}

void CheatCardLayer::setCheatedCard(int userId, std::vector<int> listCheatedCardIndex, std::vector<int> listRemainCardIndex)
{
    for (int i = 0; i < 52; i++)
        setCardButtonState(userId, &_arrCardButtonStruct[i], CardButtonState::STATE_DISABLE, i);
    for (int i = 0; i < listCheatedCardIndex.size(); i++)
        setCardButtonState(userId, &_arrCardButtonStruct[listCheatedCardIndex[i]], CardButtonState::STATE_CHEATED, i);
    for (int i = 0; i < listRemainCardIndex.size(); i++)
        setCardButtonState(userId, &_arrCardButtonStruct[listRemainCardIndex[i]], CardButtonState::STATE_NORMAL);
}

void CheatCardLayer::attach(cocos2d::Node* node, int sceneId)
{
    removeFromParent();
    node->addChild(this, kAboveForeground);
    for (int i = 0; i < 52; i++)
        setCardButtonState(-2, &_arrCardButtonStruct[i], CardButtonState::STATE_NORMAL);

    _layoutCheatCards->setVisible(false);
    _layoutCheatCombine->setVisible(false);
    _textCheat->setVisible(false);
    _buttonCheatCard->setVisible(true);
}

void CheatCardLayer::onClearCheatCardButton(cocos2d::Ref* sender)
{
    requestClearCheatCard();
}

void CheatCardLayer::requestCheatCard(int cardIdx)
{
    if (cardIdx != -1)
        MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_ADD, cardIdx);
    else
        MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_ADD, cardIdx);
}

void CheatCardLayer::requestClearCheatCard()
{
    MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_CLEAR, -1);
}

void CheatCardLayer::requestCancelCheatCard(int cardIdx)
{
    MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_REMOVE, cardIdx);
}

void CheatCardLayer::showCheatLayer(int userId)
{
    _layoutCheatCombine->setVisible(false);
    _layoutCheatCards->setVisible(true);
    _closeButton->setVisible(true);
    _cbFirstMove->setVisible(true);

    _userId = userId;
    _textUserId->setString(StringUtils::format("UserId: %d", _userId).c_str());
}

void CheatCardLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (_layoutCheatCards->isVisible())
        event->stopPropagation();
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_BACK:
            onCloseButton(nullptr);
            break;
        default:
            break;
    }
}

void CheatCardLayer::onCheckBoxClick(cocos2d::Ref* ref, cocos2d::ui::CheckBox::EventType type)
{
    if (type == cocos2d::ui::CheckBox::EventType::SELECTED)
        MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_FIRST_MOVE, 1);
    else if (type == cocos2d::ui::CheckBox::EventType::UNSELECTED)
        MessageSender::requestCheatCards(_userId, CheatCardType::CHEAT_CARD_TYPE_FIRST_MOVE, 0);
}
