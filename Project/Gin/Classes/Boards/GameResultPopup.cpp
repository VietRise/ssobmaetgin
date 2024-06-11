#include "GameResultPopup.h"
#include "Helper/GameUtils.h"
#include "Manager/TextManager.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define INDEX_RESULT_SCORE              0
#define INDEX_RESULT_PROFIT             1
#define INDEX_RESULT_NORMAL_WIN         2
#define INDEX_RESULT_GIN            3
#define INDEX_RESULT_SECRET_MELDS       4
#define INDEX_RESULT_BONUS_CARD         5
#define INDEX_RESULT_BURNED_PLAYER      6
#define INDEX_RESULT_CHALLENGER         7
#define INDEX_RESULT_HITPOT             8
#define INDEX_RESULT_BONUS_WIN          9

#define MAX_RESULT_SIZE                 10
#define SCALE_TEXT_TITLE_IPAD           0.8f

std::map<int, int> GameResultPopup::s_mapResultValue;

GameResultPopup::~GameResultPopup()
{
    CC_SAFE_RELEASE(_layoutGameResultTemp);
    _listGameResultItem.clear();
}

GameResultPopup::GameResultPopup()
{
    _layoutContent        = nullptr;
    _layoutGameResultTemp = nullptr;
    _buttonOK             = nullptr;

    s_mapResultValue = {{ INDEX_RESULT_SCORE,         TXT_GAME_RESULT_SCORE },
                        { INDEX_RESULT_PROFIT,        TXT_GAME_RESULT_PROFIT },
                        { INDEX_RESULT_NORMAL_WIN,    TXT_GAME_RESULT_NORMAL_WIN },
                        { INDEX_RESULT_GIN,           TXT_GAME_RESULT_GIN },
                        { INDEX_RESULT_SECRET_MELDS,  TXT_GAME_RESULT_SECRET_MELDS },
                        { INDEX_RESULT_BONUS_CARD,    TXT_GAME_RESULT_BONUS_CARD },
                        { INDEX_RESULT_BURNED_PLAYER, TXT_GAME_RESULT_BURNED_PLAYER },
                        { INDEX_RESULT_CHALLENGER,    TXT_GAME_RESULT_CHALLENGER },
                        { INDEX_RESULT_HITPOT,        TXT_GAME_RESULT_HITPOT },
                        { INDEX_RESULT_BONUS_WIN,     TXT_GAME_RESULT_BONUS_WIN }};
}

bool GameResultPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void GameResultPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node  = CSLoader::createNode("ui/Common/GameResultPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    if (s_resolution_ratio <= RESOLUTION_1_6)
    {
        Size sizeLayoutRoot = Size(this->_layoutRoot->getContentSize().width, this->_layoutRoot->getContentSize().height);
        sizeLayoutRoot.width += (_screenSize.width - sizeLayoutRoot.width) * 0.55f;
        this->_layoutRoot->setContentSize(sizeLayoutRoot);
    }
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_layoutGameResultTemp = (Layout*) node->getChildByName("UserResultInfo");
    this->_layoutGameResultTemp->setContentSize(Helper::seekWidgetByName(this->_layoutRoot, "PanelSlot1")->getContentSize());
    ui::Helper::doLayout(this->_layoutGameResultTemp);
    this->_layoutGameResultTemp->retain();

    this->_layoutContent = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "PanelContent"));
    for (int i = 0; i < MAX_RESULT_SIZE; i++)
    {
        auto textTitle = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutContent, StringUtils::format("Text%d", i + 1)));
        textTitle->setString(GET_TEXT_BY_ID(s_mapResultValue.at(i)));
        if (s_resolution_ratio <= RESOLUTION_1_6)
            textTitle->setScale(textTitle->getScale() * SCALE_TEXT_TITLE_IPAD);
        _listTitle.push_back(textTitle);
    }
    for (int i = 0; i < MAXIMUM_NUMBER_SLOT_GAME; i++)
    {
        auto layoutSlot = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutContent, StringUtils::format("PanelSlot%d", i + 1)));
        _listGameResultLayout.push_back(layoutSlot);
        auto layoutInfo = (Layout*) layoutSlot->getChildByName("PanelInfo");
        GameResultItem* gameResultItem = GameResultItem::create();
        gameResultItem->initLayout(this->_layoutGameResultTemp);
        Layout* layoutGameResultItem = gameResultItem->getLayoutRoot();
        layoutGameResultItem->setContentSize(layoutInfo->getContentSize());
        layoutGameResultItem->setPosition(Vec2(layoutInfo->getContentSize().width * 0.5f, layoutInfo->getContentSize().height * 0.5f));
        layoutInfo->addChild(layoutGameResultItem);
        _listGameResultItem.pushBack(gameResultItem);
    }
    this->_buttonOK = static_cast<Button*>(Helper::seekWidgetByName(this->_layoutContent, "ButtonOK"));
    this->_buttonOK->addClickEventListener(CC_CALLBACK_1(GameResultPopup::onButtonClick, this));
}

void GameResultPopup::loadData(std::vector<PlayerResultData> listUserData)
{
    for (int i = 0; i < _listGameResultItem.size(); i++)
    {
        Layout* layoutInfo   = (Layout*) _listGameResultLayout.at(i)->getChildByName("PanelInfo");
        Text  * textNoPlayer = (Text*) _listGameResultLayout.at(i)->getChildByName("TextNoPlayer");
        if (i < listUserData.size())
        {
            layoutInfo->setVisible(true);
            textNoPlayer->setVisible(false);
            _listGameResultItem.at(i)->loadData(listUserData.at(i));
        }
        else
        {
            layoutInfo->setVisible(false);
            textNoPlayer->setVisible(true);
        }
    }
}

void GameResultPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonOK)
        this->close();
}


GameResultItem::~GameResultItem()
{
}

GameResultItem::GameResultItem()
{
    _layoutRoot   = nullptr;
    _layoutAvatar = nullptr;
    _avatar       = nullptr;
    _textUsername = nullptr;
    _imageResult  = nullptr;
    _textResult   = nullptr;
}

GameResultItem* GameResultItem::create()
{
    GameResultItem* gameResultItem = new GameResultItem();
    gameResultItem->autorelease();
    return gameResultItem;
}

void GameResultItem::initLayout(cocos2d::ui::Layout* layoutResultTem)
{
    _layoutRoot   = (Layout*) layoutResultTem->clone();
    _layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(this->_layoutRoot, "LayoutAvatar"));
    _textUsername = static_cast<Text*>(Helper::seekWidgetByName(this->_layoutRoot, "TextUsername"));
    _imageResult  = static_cast<ImageView*>(Helper::seekWidgetByName(this->_layoutRoot, "ImageResult"));
    _textResult   = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoot, "TextBMResult"));
    for (int i = 0; i < MAX_RESULT_SIZE; i++)
    {
        auto textValue = static_cast<TextBMFont*>(Helper::seekWidgetByName(this->_layoutRoot, StringUtils::format("BMText%d", i + 1)));
        if (s_resolution_ratio <= RESOLUTION_1_6)
            textValue->setScale(textValue->getScale() * SCALE_TEXT_TITLE_IPAD);
        _listTextValue.push_back(textValue);
    }
}

void GameResultItem::loadData(PlayerResultData data)
{
    _data = data;
    this->_avatar = Avatar::create();
    this->_avatar->setPosition(this->_layoutAvatar->getContentSize() / 2);
    this->_avatar->setFrameUrl(_data._avatarLink, this->_layoutAvatar->getContentSize());
    this->_layoutAvatar->addChild(this->_avatar, -2);
    this->_textUsername->setString(_data._userName);
    switch (_data._type)
    {
    case GinResultType::GIN_RESULT_TYPE_WIN_KNOCK:
    case GinResultType::GIN_RESULT_TYPE_WIN_GIN:
    case GinResultType::GIN_RESULT_TYPE_WIN_BIG_GIN:
    {
        this->_imageResult->loadTexture("ui/ImagesCommon/ribbon_win.png", Widget::TextureResType::PLIST);
        this->_textResult->setString(GET_TEXT_BY_ID(TXT_GAME_RESULT_TYPE_GIN));
        this->_textResult->setColor(COLOR_WIN_3);
    }
        break;
    case GinResultType::GIN_RESULT_TYPE_LOSE_KNOCK:
    case GinResultType::GIN_RESULT_TYPE_LOSE_GIN:
    case GinResultType::GIN_RESULT_TYPE_LOSE_BIG_GIN:
    {
        this->_imageResult->loadTexture("ui/ImagesCommon/ribbon_lose.png", Widget::TextureResType::PLIST);
        this->_textResult->setString(GET_TEXT_BY_ID(TXT_GAME_RESULT_TYPE_LOSE));
        this->_textResult->setColor(COLOR_LOSE_3);
    }
        break;
    case GinResultType::GIN_RESULT_TYPE_SCORE:
    {
        if (_data._winOrder == 0)
        {
            this->_imageResult->loadTexture("ui/ImagesCommon/ribbon_win.png", Widget::TextureResType::PLIST);
            this->_textResult->setString(GET_TEXT_BY_ID(TXT_GAME_RESULT_TYPE_WIN));
            this->_textResult->setColor(COLOR_WIN_3);
        }
        else
        {
            this->_imageResult->loadTexture("ui/ImagesCommon/ribbon_lose.png", Widget::TextureResType::PLIST);
            this->_textResult->setString(GET_TEXT_BY_ID(TXT_GAME_RESULT_TYPE_LOSE));
            this->_textResult->setColor(COLOR_LOSE_3);
        }
    }
        break;
    default:
    {
        this->_imageResult->setVisible(false);
        this->_textResult->setString("");
    }
        break;
    }
     for (int i = 0; i < _listTextValue.size(); i++)
    {
        TextBMFont* textValue = _listTextValue.at(i);
        double number = 0;
        switch (i)
        {
        case INDEX_RESULT_SCORE:
        {
            number = _data._resultDetail._handPoints;
        }
            break;
        case INDEX_RESULT_PROFIT:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_NORMAL_WIN:
        {
            number = _data._resultDetail._normalWinMoney;
        }
            break;
        case INDEX_RESULT_GIN:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_SECRET_MELDS:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_BONUS_CARD:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_BURNED_PLAYER:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_CHALLENGER:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_HITPOT:
        {
            number = 0;
        }
            break;
        case INDEX_RESULT_BONUS_WIN:
        {
            number = 0;
        }
            break;
        default:
            break;
        }

        if (i == INDEX_RESULT_SCORE)
        {
            textValue->setColor(Color3B::WHITE);
            textValue->setString(StringUtils::format("%d", _data._resultDetail._handPoints));
        }
        else if (i == INDEX_RESULT_PROFIT)
        {
            if (number > 0)
            {
                textValue->setColor(COLOR_POSITIVE_NUMBER_3);
                textValue->setString(GameUtils::MoneyFormat(number, "+"));
            }
            else if (number < 0)
            {
                textValue->setColor(COLOR_NAGATIVE_NUMBER_3);
                textValue->setString(GameUtils::MoneyFormat(number));
            }
            else
            {
                textValue->setColor(COLOR_MONEY);
                textValue->setString("0");
            }
        }
        else
        {
            if (number > 0)
            {
                textValue->setColor(COLOR_MONEY);
                textValue->setString(GameUtils::MoneyFormat(number, "+"));
            }
            else if (number < 0)
            {
                textValue->setColor(COLOR_MONEY);
                textValue->setString(GameUtils::MoneyFormat(number));
            }
            else
            {
                textValue->setColor(COLOR_MONEY);
                textValue->setString("-");
            }
            
        }
    }
}

cocos2d::ui::Layout* GameResultItem::getLayoutRoot()
{
    return _layoutRoot;
}
