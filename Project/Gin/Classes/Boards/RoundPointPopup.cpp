#include "RoundPointPopup.h"
#include "Manager/TextManager.h"
#include "Define/GameDefine.h"
#include "Helper/fmt/format.h"
#include "Component/Avatar.h"
#include "Helper/GameUtils.h"
#include "Component/ScoreBoxNode.h"
#include "Component/TutorialView.h"
#include "Component/RuleScore.h"
#include "Component/RuleDetails.h"

#define INDEX_RESULT_PROFIT             0
#define INDEX_RESULT_HITPOT             1
#define INDEX_RESULT_NORMAL_WIN         2
#define INDEX_RESULT_GIN                3
#define INDEX_RESULT_SECRET_MELDS       4
#define INDEX_RESULT_BONUS_CARD         5
#define INDEX_RESULT_BURNED_PLAYER      6
#define INDEX_RESULT_CHALLENGER         7
#define INDEX_RESULT_BONUS_WIN          8
#define MAX_RESULT_SIZE                 9

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

std::vector<PlayerResultData> RoundPointPopup::_listUserDataCached;

RoundPointPopup::~RoundPointPopup()
{
    CC_SAFE_RELEASE_NULL(this->_layoutPlayerTemplate);
    CC_SAFE_RELEASE_NULL(this->_layoutEachRowTemplate);
    CC_SAFE_RELEASE_NULL(this->_layoutRowHitpot);
}

RoundPointPopup::RoundPointPopup()
{
    this->_layoutPlayerTemplate  = nullptr;
    this->_layoutEachRowTemplate = nullptr;
}

bool RoundPointPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void RoundPointPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/RoundPointPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    _originalLayoutRootSize = this->_layoutRoot->getContentSize();

    this->_buttonClose = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonClose"));
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(RoundPointPopup::onButtonClick, this));

    this->_buttonOk = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonOK"));
    this->_buttonOk->addClickEventListener(CC_CALLBACK_1(RoundPointPopup::onButtonClick, this));

    this->_buttonTutorial = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonTutorial"));
    this->_buttonTutorial->addClickEventListener(CC_CALLBACK_1(RoundPointPopup::onButtonClick, this));

    this->_layoutPlayerTemplate = (Layout*) node->getChildByName("LayoutPlayer");
    this->_layoutPlayerTemplate->removeFromParent();
    this->_layoutPlayerTemplate->retain();

    this->_layoutEachRowTemplate = (Layout*) node->getChildByName("LayoutData");
    this->_layoutEachRowTemplate->removeFromParent();
    this->_layoutEachRowTemplate->retain();

    this->_listViewEachRound = static_cast<ListView*>(Helper::seekWidgetByName(_layoutRoot, "ListViewContent"));
    this->_listViewEachRound->removeAllItems();

    // List layout players
    auto     layoutPlayers = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "PanelPlayer"));
    for (int i             = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        auto layoutPlayer     = static_cast<Layout*>(Helper::seekWidgetByName(layoutPlayers, fmt::format("LayoutSlot{0}", i + 1)));
        auto layoutPlayerData = (Layout*) _layoutPlayerTemplate->clone();
        layoutPlayerData->setPosition(Vec2(layoutPlayer->getContentSize().width * 0.5f, layoutPlayer->getContentSize().height * 0.5f));
        layoutPlayer->addChild(layoutPlayerData);
        _listLayoutPlayer.push_back(layoutPlayerData);
    }

    // List layout data
    auto layoutTotal = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutTotal"));
    _listLayoutDataRow.push_back(layoutTotal);
    for (int i = 0; i < MAX_RESULT_SIZE - 1; i++)
    {
        auto layoutDataRow = (Layout*) _layoutEachRowTemplate->clone();
        layoutDataRow->setCascadeColorEnabled(true);
        _listViewEachRound->pushBackCustomItem(layoutDataRow);
        _listLayoutDataRow.push_back(layoutDataRow);
        if (i == INDEX_RESULT_HITPOT - 1)
        {
            _layoutRowHitpot = layoutDataRow;
            _layoutRowHitpot->retain();
        }
    }

    auto textBetTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextBetTitle"));
    textBetTitle->setString(GET_TEXT_BY_ID(TXT_BET_TITLE));

    auto textRoomTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextRoomTitle"));
    textRoomTitle->setString(GET_TEXT_BY_ID(TXT_ROOM_TITLE));
}

void RoundPointPopup::onButtonClick(cocos2d::Ref* sender)
{
    if (_buttonClose == sender || _buttonOk == sender)
        this->close();
    else if (_buttonTutorial == sender)
        showTutorial();
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
}

void RoundPointPopup::setRoomInfo(const std::string& roomName, double bet)
{
    auto textBet = static_cast<TextBMFont*>(Helper::seekWidgetByName(_layoutRoot, "TextBet"));
    textBet->setString(GameUtils::MoneyFormatShortCut(bet, "$"));

    auto textRoom = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextRoom"));
    textRoom->setString(roomName);
}

void RoundPointPopup::setPlayerInfo(const PlayerResultData& playerInfoData, int order)
{
    Layout* layoutPlayer     = _listLayoutPlayer.at(order);
    Layout* layoutPlayerInfo = static_cast<Layout*>(Helper::seekWidgetByName(layoutPlayer, "LayoutInfo"));
    if (playerInfoData._playerIndex >= 0)
    {
        layoutPlayerInfo->setVisible(true);
        // Set avatar
        auto layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(layoutPlayerInfo, "LayoutAvatar"));
        layoutAvatar->removeChildByTag(1);
        auto avatar = Avatar::create();
        avatar->setTag(1);
        avatar->setPosition(layoutAvatar->getContentSize() / 2);
        layoutAvatar->addChild(avatar, -1);
        avatar->setFrameUrl(playerInfoData._avatarLink, layoutAvatar->getContentSize());
        // Set text status
        // Result text
        auto win  = static_cast<Layout*>(Helper::seekWidgetByName(layoutPlayerInfo, "LayoutWinner"));
        auto lose = static_cast<Layout*>(Helper::seekWidgetByName(layoutPlayerInfo, "LayoutLose"));

        win->setVisible(false);
        lose->setVisible(false);

        auto winGin       = static_cast<ImageView*>(Helper::seekWidgetByName(win, "tongits"));
        auto winFight     = static_cast<ImageView*>(Helper::seekWidgetByName(win, "fight"));
        auto winChallenge = static_cast<ImageView*>(Helper::seekWidgetByName(win, "challenge"));
        auto winScore     = static_cast<ImageView*>(Helper::seekWidgetByName(win, "winner"));

        winGin->setVisible(false);
        winFight->setVisible(false);
        winChallenge->setVisible(false);
        winScore->setVisible(false);

        if (playerInfoData._type == GinResultType::GIN_RESULT_TYPE_WIN_KNOCK || playerInfoData._type == GinResultType::GIN_RESULT_TYPE_WIN_GIN ||
            playerInfoData._type == GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN)
        {
            winGin->setVisible(true);
            win->setVisible(true);
        }
        else if (playerInfoData._type == GinResultType::GIN_RESULT_TYPE_SCORE && playerInfoData._winOrder == 0)
        {
            winScore->setVisible(true);
            win->setVisible(true);
        }

        auto textUserName = static_cast<Text*>(Helper::seekWidgetByName(layoutPlayerInfo, "TextUsername"));
        textUserName->setString(playerInfoData._userName);
        auto layoutScore = static_cast<Text*>(Helper::seekWidgetByName(layoutPlayerInfo, "LayoutScore"));
        layoutScore->removeAllChildren();
        if (playerInfoData._type != GinResultType::GIN_RESULT_TYPE_WIN_GIN)
        {
            int value = playerInfoData._resultDetail._handPoints;
            int type  = playerInfoData._type;

            if (playerInfoData._type == GinResultType::GIN_RESULT_TYPE_SCORE && playerInfoData._winOrder != 0)
                value *= -1;

            ScoreBoxNode* scoreBoxNode = ScoreBoxNode::create();
            scoreBoxNode->show(true, value, (GinResultType) type);
            scoreBoxNode->setScore(playerInfoData._resultDetail._handPoints);
            scoreBoxNode->setPosition(layoutScore->getContentSize() / 2);
            layoutScore->addChild(scoreBoxNode);
        }
    }
    else
    {
        layoutPlayerInfo->setVisible(false);
    }
}


void RoundPointPopup::loadData(const std::vector<PlayerResultData>& listUserData)
{
    //std::sort(listUserData.begin(), listUserData.end(), [=](const PlayerResultData& a, const PlayerResultData& b) {return a._playerIndex < b._playerIndex; });
    // Load data for player
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        if (i < listUserData.size())
        {
            this->setPlayerInfo(listUserData.at(i), i);
        }
        else
        {
            PlayerResultData noPlayerData;
            noPlayerData._playerIndex = -1;
            this->setPlayerInfo(noPlayerData, i);
        }
    }
    // Insert layout Hitpot
    if (_listViewEachRound->getItems().size() < MAX_RESULT_SIZE - 1)
        _listViewEachRound->insertCustomItem(_layoutRowHitpot, INDEX_RESULT_HITPOT - 1);
    std::vector<std::pair<std::string, std::vector<double>>> listDataRow;
    bool                                                     haveWinJackpot = false;
    vector<int>                                              properties;
    properties.push_back(INDEX_RESULT_PROFIT);
    properties.push_back(INDEX_RESULT_HITPOT);
    properties.push_back(INDEX_RESULT_NORMAL_WIN);
    properties.push_back(INDEX_RESULT_GIN);
    properties.push_back(INDEX_RESULT_CHALLENGER);
    properties.push_back(INDEX_RESULT_BURNED_PLAYER);
    properties.push_back(INDEX_RESULT_SECRET_MELDS);
    properties.push_back(INDEX_RESULT_BONUS_CARD);
    properties.push_back(INDEX_RESULT_BONUS_WIN);

    for (int pro : properties)
    {
        std::string         title = "";
        std::vector<double> dataRow;

        switch (pro)
        {
            case INDEX_RESULT_PROFIT:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_PROFIT);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_HITPOT:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_HITPOT);
                double   amount = 0;
                for (int j      = 0; j < listUserData.size(); j++)
                {
                    amount += 0;
                    dataRow.push_back(0);
                }
                if (amount > 0)
                {
                    haveWinJackpot = true;
                }
            }
                break;
            case INDEX_RESULT_NORMAL_WIN:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_NORMAL_WIN);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(listUserData.at(j)._resultDetail._normalWinMoney);
                }
            }
                break;
            case INDEX_RESULT_GIN:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_GIN);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_SECRET_MELDS:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_SECRET_MELDS);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_BONUS_CARD:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_BONUS_CARD);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_BURNED_PLAYER:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_BURNED_PLAYER);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_CHALLENGER:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_CHALLENGER);
                for (int j = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
            }
                break;
            case INDEX_RESULT_BONUS_WIN:
            {
                title = GET_TEXT_BY_ID(TXT_GAME_RESULT_BONUS_WIN);
                double   amount = 0;
                for (int j      = 0; j < listUserData.size(); j++)
                {
                    dataRow.push_back(0);
                }
                if (amount > 0)
                {
                    title = StringUtils::format("%s (x%.0f)", GET_TEXT_BY_ID(TXT_GAME_RESULT_BONUS_WIN), 1.0f);
                }
            }
                break;
            default:
                break;
        }
        listDataRow.push_back({ title, dataRow });
    }

    for (int i = 0; i < _listLayoutDataRow.size(); i++)
    {
        auto        layoutDataRow = _listLayoutDataRow.at(i);
        bool        emptyRow      = true;
        for (double money : listDataRow.at(i).second)
            if (money != 0)
                emptyRow = false;
        layoutDataRow->setColor(Color3B::WHITE);
        if (emptyRow)
            layoutDataRow->setColor(Color3B(170, 170, 170));

        auto textTitle = static_cast<Text*>(Helper::seekWidgetByName(layoutDataRow, "TextTitle"));
        textTitle->setString(listDataRow.at(i).first);
        if (i == INDEX_RESULT_PROFIT)
        {
            for (int j = 0; j < MAXIMUM_NUMBER_SLOT_GAME; j++)
            {
                auto   textValue  = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutDataRow, fmt::format("TextPoint{0}", j + 1)));
                double number     = 0;
                bool   isNoPlayer = true;
                if (j < listDataRow.at(i).second.size())
                {
                    isNoPlayer = false;
                    number     = listDataRow.at(i).second.at(j);
                }
                if (number > 0)
                {
                    textValue->setColor(COLOR_POSITIVE_NUMBER_3);
                    textValue->setString(GameUtils::MoneyFormatShortCut(number, "+"));
                }
                else if (number < 0)
                {
                    textValue->setColor(COLOR_NAGATIVE_NUMBER_3);
                    textValue->setString(GameUtils::MoneyFormatShortCut(number));
                }
                else
                {
                    textValue->setColor(COLOR_MONEY);
                    if (isNoPlayer)
                        textValue->setString("-");
                    else
                        textValue->setString("0");
                }
            }
        }
        else
        {
            for (int j = 0; j < MAXIMUM_NUMBER_SLOT_GAME; j++)
            {
                auto   textValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(layoutDataRow, fmt::format("TextPoint{0}", j + 1)));
                double number    = 0;
                if (j < listDataRow.at(i).second.size())
                    number = listDataRow.at(i).second.at(j);
                if (number > 0)
                {
                    textValue->setColor(Color3B(49, 236, 49));
                    textValue->setString(GameUtils::MoneyFormatShortCut(number, "+"));
                }
                else if (number < 0)
                {
                    textValue->setColor(Color3B(255, 54, 54));
                    textValue->setString(GameUtils::MoneyFormatShortCut(number));
                }
                else
                {
                    textValue->setColor(Color3B::GRAY);
                    textValue->setString("-");
                }
            }
        }
    }
    // Check whether show hitpot row
    if (haveWinJackpot)
    {
        this->_layoutRoot->setContentSize(_originalLayoutRootSize);
        Helper::doLayout(this);
    }
    else
    {
        _listViewEachRound->removeItem(INDEX_RESULT_HITPOT - 1);
        this->_layoutRoot->setContentSize(_originalLayoutRootSize - Size(0, _layoutEachRowTemplate->getContentSize().height));
        Helper::doLayout(this);
    }
}

void RoundPointPopup::showTutorial()
{
    auto tutorialView = TutorialView::create();
    tutorialView->setContent({ RuleDetails::create(), RuleScore::create() });
    addChild(tutorialView, kAboveForeground);
    tutorialView->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([=]() {
        tutorialView->selectButton(1);
    }), nullptr));
}