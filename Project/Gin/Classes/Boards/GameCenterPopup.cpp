#include "GameCenterPopup.h"
#include "Manager/TextManager.h"
#include "Helper/JsonHelper.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Helper/GameUtils.h"
#include "Native/CallToOtherLanguage.h"
#include "FirebaseGG/Firebase.h"
#include "Manager/DownloadManager.h"
#include "UserData/UserData.h"
#include "Define/GameDefine.h"
#include "Define/ResourcesDefine.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define KEY_GAME_CENTER_VERSION         "version"
#define KEY_GAME_CENTER_GAMES           "games"
#define KEY_GAME_CENTER_NEW_NAME        "nn"
#define KEY_GAME_CENTER_PACKAGE_ID      "pi"
#define KEY_GAME_CENTER_NEW_AVATAR_URL  "nau"
#define KEY_GAME_CENTER_VERSION_CODE    "vc"
#define KEY_GAME_CENTER_PRICE           "p"
#define KEY_GAME_CENTER_RELEASE_TIME    "rt"
#define KEY_GAME_CENTER_HIGHLIGHT       "hl"
#define KEY_GAME_CENTER_LINK            "l"

#define GAME_CENTER_COLUMN 2

#define BUTTON_DOWNLOAD_NORMAL_NAME "ui/ImagesCommon/button_3.png"
#define BUTTON_DOWNLOAD_PRESS_NAME "ui/ImagesCommon/button_3_click.png"
#define BUTTON_PLAY_NOW_NORMAL_NAME "ui/ImagesCommon/button_2.png"
#define BUTTON_PLAY_NOW_PRESS_NAME "ui/ImagesCommon/button_2_click.png"
#define BUTTON_DISABLE_NAME "ui/ImagesCommon/button_4.png"

#define LABEL_PLAY_NOW_COLOR cocos2d::Color3B(75,47,0)

int GameCenterPopup::_versionGameCenter = 0;
std::string                 GameCenterPopup::_dataGameCenterJson = "";
std::vector<GameCenterData> GameCenterPopup::_listGameItemsData;

GameCenterPopup::~GameCenterPopup()
{
    _listGameItems.clear();
}

GameCenterPopup::GameCenterPopup()
{
}

bool GameCenterPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    this->initScreen();
    return true;
}

void GameCenterPopup::onEnter()
{
    InterfacePopupBoard::onEnter();
    UserData::getInstance()->setGameCenterVersion(_versionGameCenter);
}

void GameCenterPopup::initScreen()
{
    this->setContentSize(_screenSize);
    initWithColor(POPUP_BACKGROUND_COLOR);

    Node* node  = CSLoader::createNode("ui/Common/GameCenterPopup.csb");
    Node* child = node->getChildByName("Root");
    child->removeFromParent();
    this->_layoutRoot = static_cast<Layout*>(child);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));

    this->addChild(_layoutRoot, kMiddleground);
    ui::Helper::doLayout(this);

    this->_textTitle = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "TextTitle"));
    this->_textTitle->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_TITILE));

    this->_buttonClose = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonClose"));
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(GameCenterPopup::onCloseButtonClick, this));

    this->_listViewGames = static_cast<ListView*>(Helper::seekWidgetByName(_layoutRoot, "ListViewContent"));
    GameCenterPopup::loadItemsData();
    loadItems();
}

void GameCenterPopup::loadItems()
{
    _listGameItems.clear();
    _listViewGames->removeAllItems();

    Layout* listViewItem = nullptr;
    for (int i = 0; i < _listGameItemsData.size(); i++)
    {
        Node* node  = CSLoader::createNode("ui/Common/GameCenterItem.csb");
        Node* child = node->getChildByName("Root");
        child->removeFromParent();
        Layout* layoutItem = static_cast<Layout*>(child);

        if (i % GAME_CENTER_COLUMN == 0)
        {
            listViewItem = Layout::create();
            listViewItem->setContentSize(Size(_listViewGames->getContentSize().width, layoutItem->getContentSize().height + 10.0f));
            listViewItem->setCascadeOpacityEnabled(true);
            _listViewGames->pushBackCustomItem(listViewItem);
        }

        GameCenterItem* gameCenterItem = GameCenterItem::create(layoutItem, _listGameItemsData.at(i));
        gameCenterItem->setCallback(CC_CALLBACK_1(GameCenterPopup::onGameCenterItemClick, this));
        float posX = (i % GAME_CENTER_COLUMN + 1) * (layoutItem->getContentSize().width + 10.0f) - layoutItem->getContentSize().width / 2;
        float posY = listViewItem->getContentSize().height * 0.5f;
        layoutItem->setPosition(Vec2(posX, posY));
        listViewItem->addChild(layoutItem);
        _listGameItems.pushBack(gameCenterItem);
    }
}

void GameCenterPopup::loadItemsData()
{
    _dataGameCenterJson = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_GAME_CENTER_DATA, Resources::JSON::JSON_FILE_GAME_CENTER);
    if (!_dataGameCenterJson.empty())
    {
        long long   currentTime        = GameUtils::getCurrentTime();
        std::string currentPackageName = CallToOtherLanguage::getInstance()->getMyPackageName();
        _listGameItemsData.clear();
        const rapidjson::Document& dataGameCenterDocument = JsonHelper::parseStringToJson(_dataGameCenterJson);
        _versionGameCenter                                = JsonHelper::getInt(dataGameCenterDocument, KEY_GAME_CENTER_VERSION, 0);
        const rapidjson::Value& dataGames = JsonHelper::getValue(dataGameCenterDocument, KEY_GAME_CENTER_GAMES);
        if (dataGames.IsArray())
        {
            for (auto iterGameItem = dataGames.Begin(); iterGameItem != dataGames.End(); iterGameItem++)
            {
                const rapidjson::Value& gameItemValue = *iterGameItem;
                GameCenterData gameCenterItemData;
                gameCenterItemData._name        = GET_TEXT_BY_JSON(JsonHelper::getValue(gameItemValue, KEY_GAME_CENTER_NEW_NAME));
                gameCenterItemData._packageID   = JsonHelper::getString(gameItemValue, KEY_GAME_CENTER_PACKAGE_ID, "");
                gameCenterItemData._avatarUrl   = GET_TEXT_BY_JSON(JsonHelper::getValue(gameItemValue, KEY_GAME_CENTER_NEW_AVATAR_URL));
                gameCenterItemData._versionCode = JsonHelper::getInt(gameItemValue, KEY_GAME_CENTER_VERSION_CODE, 0);
                gameCenterItemData._price       = JsonHelper::getDouble(gameItemValue, KEY_GAME_CENTER_PRICE, 0);
                gameCenterItemData._releaseTime = JsonHelper::getLong(gameItemValue, KEY_GAME_CENTER_RELEASE_TIME, 0);
                gameCenterItemData._isHighlight = JsonHelper::getInt(gameItemValue, KEY_GAME_CENTER_HIGHLIGHT, 0);
                gameCenterItemData._link        = JsonHelper::getString(gameItemValue, KEY_GAME_CENTER_LINK, "");
                // Game state
                long long passTime = currentTime - gameCenterItemData._releaseTime;
                if (passTime < 0)
                    gameCenterItemData._state = GameCenterItemState::ComingSoon;
                else
                {
                    if (gameCenterItemData._packageID.compare(currentPackageName) == 0)
                        gameCenterItemData._state = GameCenterItemState::Playing;
                    else
                    {
                        bool isAppInstalled = CallToOtherLanguage::getInstance()->isPackageInstalled(gameCenterItemData._packageID);
                        if (isAppInstalled)
                            gameCenterItemData._state = GameCenterItemState::Installed;
                        else
                            gameCenterItemData._state = GameCenterItemState::NotInstall;
                    }
                }
                _listGameItemsData.push_back(gameCenterItemData);
            }
        }
    }
    std::sort(_listGameItemsData.begin(), _listGameItemsData.end(), [](const GameCenterData& i, const GameCenterData& j) {
        return i._state < j._state;
    });
}

bool GameCenterPopup::isNew()
{
    if (UserData::getInstance()->getGameCenterVersion() < _versionGameCenter)
        return true;
    return false;
}

GameCenterData GameCenterPopup::getGameCenterData(const std::string& id)
{
    for (int       i = 0; i < _listGameItemsData.size(); i++)
        if (_listGameItemsData.at(i)._packageID.compare(id) == 0)
            return _listGameItemsData.at(i);
    GameCenterData data;
    data._packageID = id;
    return data;
}

void GameCenterPopup::onCloseButtonClick(Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    this->close();
}

void GameCenterPopup::onGameCenterItemClick(const GameCenterData& gameCenterData)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    Firebase::logGameCenterAction(gameCenterData._packageID, gameCenterData._state);
    if (CallToOtherLanguage::getInstance()->isPackageInstalled(gameCenterData._packageID) || gameCenterData._link.empty())
        CallToOtherLanguage::getInstance()->openAnotherAppInStore(gameCenterData._packageID);
    else
        CallToOtherLanguage::getInstance()->openAnotherAppInStoreByLink(gameCenterData._link);
}

// ------------- GameCenter Item ------------------
GameCenterItem::~GameCenterItem()
{
    CC_SAFE_RELEASE(_layoutRoot);
}

GameCenterItem::GameCenterItem(const GameCenterData& data)
{
    _data = data;
}

void GameCenterItem::initLayout(cocos2d::ui::Layout* layoutRoot)
{
    _layoutRoot = layoutRoot;
    _layoutRoot->retain();

    _layoutAvatar = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, "LayoutAvatar"));
    _spriteAvatar = (Sprite*) _layoutAvatar->getChildByName("SpriteAvatar");

    if (!GameUtils::validateURL(_data._avatarUrl))
    {
        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_data._avatarUrl);
        if (frame == nullptr)
            frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(Resources::PNG::IconGameDefault);
        if (frame != nullptr)
        {
            _spriteAvatar->setSpriteFrame(frame);
            _spriteAvatar->setScaleX(_layoutAvatar->getContentSize().width / _spriteAvatar->getContentSize().width);
            _spriteAvatar->setScaleY(_layoutAvatar->getContentSize().height / _spriteAvatar->getContentSize().height);
        }
    }
    else
    {
        DownloadManager::getInstance()->downloadImage(this->_spriteAvatar, _data._avatarUrl, Resources::PNG::IconGameDefault, _layoutAvatar->getContentSize());
    }

    _textName = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelName"));
    _textName->setString(_data._name);

    _textPrice = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelPrice"));
    if (_data._price == 0)
        _textPrice->setString(GET_TEXT_BY_ID(TXT_FREE));
    else
        _textPrice->setString(StringUtils::format("%.2f", _data._price));
    _btnStatus = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "BtnStatus"));
    _btnStatus->setCascadeOpacityEnabled(true);
    _btnStatus->addClickEventListener(CC_CALLBACK_1(GameCenterItem::onButtonClick, this));

    auto buttonBackground = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, "ButtonListView"));
    buttonBackground->setCascadeOpacityEnabled(true);
    buttonBackground->addClickEventListener(CC_CALLBACK_1(GameCenterItem::onButtonClick, this));

    _textStatus = static_cast<Text*>(Helper::seekWidgetByName(_layoutRoot, "LabelStatus"));

    _imageViewNew = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, "ImageIconNew"));
    _imageViewNew->setVisible(false);

    switch ((GameCenterItemState) _data._state)
    {
        case GameCenterItemState::NotInstall:
        {
            _btnStatus->loadTextures(BUTTON_DOWNLOAD_NORMAL_NAME, BUTTON_DOWNLOAD_PRESS_NAME, BUTTON_DISABLE_NAME, Widget::TextureResType::PLIST);
            _textStatus->setColor(Color3B::WHITE);
            _textStatus->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_ITEM_STATUS_DOWNLOAD));

            _imageViewNew->setVisible(true);
            break;
        }
        case GameCenterItemState::ComingSoon:
        {
            _btnStatus->loadTextures(BUTTON_DOWNLOAD_NORMAL_NAME, BUTTON_DOWNLOAD_PRESS_NAME, BUTTON_DISABLE_NAME, Widget::TextureResType::PLIST);
            _btnStatus->setOpacity(160);
            _btnStatus->setEnabled(false);
            _textStatus->setColor(Color3B::WHITE);
            _textStatus->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_ITEM_STATUS_COMMING_SOON));

            _imageViewNew->setVisible(true);
            break;
        }
        case GameCenterItemState::Installed:
        {
            _btnStatus->loadTextures(BUTTON_PLAY_NOW_NORMAL_NAME, BUTTON_PLAY_NOW_PRESS_NAME, BUTTON_DISABLE_NAME, Widget::TextureResType::PLIST);
            _textStatus->setColor(LABEL_PLAY_NOW_COLOR);
            _textStatus->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_ITEM_STATUS_PLAY_NOW));
            break;
        }
        case GameCenterItemState::Playing:
        {
            _btnStatus->loadTextures(BUTTON_DISABLE_NAME, BUTTON_DISABLE_NAME, BUTTON_DISABLE_NAME, Widget::TextureResType::PLIST);
            _btnStatus->setOpacity(100);
            _btnStatus->setEnabled(false);
            _textStatus->setColor(Color3B::WHITE);
            _textStatus->setString(GET_TEXT_BY_ID(TXT_GAME_CENTER_ITEM_STATUS_PLAYING));
            break;
        }
        default:
            break;
    }
}

void GameCenterItem::onButtonClick(cocos2d::Ref* pSender)
{
    if (_data._state != (int) GameCenterItemState::ComingSoon && _data._state != (int) GameCenterItemState::Playing)
    {
        SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
        if (_callback != nullptr)
            _callback(_data);
    }
}

GameCenterItem* GameCenterItem::create(cocos2d::ui::Layout* layoutRoot, const GameCenterData& data)
{
    GameCenterItem* gameCenterItem = new GameCenterItem(data);
    gameCenterItem->initLayout(layoutRoot);
    gameCenterItem->autorelease();
    return gameCenterItem;
}

