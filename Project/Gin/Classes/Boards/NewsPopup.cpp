#include "NewsPopup.h"
#include "Manager/MyActionsManager.h"
#include "Helper/GameUtils.h"
#include "Manager/TextManager.h"
#include "UserData/UserData.h"
#include "Native/CallToOtherLanguage.h"
#include "Manager/BoardManager.h"
#include "FirebaseGG/Firebase.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Manager/DownloadManager.h"
#include "Define/ResourcesDefine.h"
#include "Helper/fmt/format.h"

#define PATH                            "ui/Common/NewsPopup.csb"
#define ID_ROOT                         "Root"
#define PANEL_ITEM_APP_ADS              "panel_content_app_ads"
#define BUTTON_CLOSE                    "ButtonClose"
#define PANEL_CONTENT                   "panel_content"
#define PAGE_VIEW_CONTENT               "page_view_content"

#define PANEL_INDICATOR                 "panel_indicator"
#define IMAGE_INDICATOR                 "image_indicator"

#define ITEM_ADS_IMAGE_ADS              "image_ads"
#define ITEM_ADS_IMAGE_GAME_ICON        "image_icon"
#define ITEM_ADS_TEXT_GAME_NAME         "text_name"
#define ITEM_ADS_TEXT_PRICE             "text_price"
#define ITEM_ADS_BUTTON_ACTION          "ButtonAction"
#define ITEM_ADS_TEXT_ACTION            "text_action"
#define ITEM_ADS_LEFT_PANEL             "panel_left"
#define ITEM_ADS_RIGHT_PANEL            "panel_right"

#define JSON_KEY_DATA                               "data"
#define JSON_KEY_ITEM_TYPE                          "type"
#define JSON_KEY_ITEM_ADS_IMAGE_LOCAL_PATH          "ads_image_local_path"
#define JSON_KEY_ITEM_ICON_IMAGE_LOCAL_PATH         "icon_image_local_path"
#define JSON_KEY_ITEM_ADS_IMAGE_PATH                "ads_image_path"
#define JSON_KEY_ITEM_ICON_IMAGE_PATH               "icon_image_path"
#define JSON_KEY_ITEM_GAME_NAME                     "game_name"
#define JSON_KEY_ITEM_BUNDLE_ID                     "bundle_id"
#define JSON_KEY_ITEM_PRICE                         "price"
#define JSON_KEY_ITEM_FIREBASE_LOG_STRING           "firebase_log_string"
#define JSON_KEY_ITEM_APPEAR_RATIO                  "appear_ratio"
#define JSON_KEY_ITEM_LINK                          "link"

#define PADDING_INDICATOR            2

USING_NS_CC;
using namespace ui;
using namespace std;

NewsItem::NewsItem()
{
}

NewsItem::~NewsItem()
{
}

bool NewsItem::init()
{
    if (!Layout::init())
        return false;
    return true;
}

NewsAdsItem::NewsAdsItem()
{
}

NewsAdsItem::~NewsAdsItem()
{
}

bool NewsAdsItem::init()
{
    if (!NewsItem::init())
        return false;
    return true;
}

void NewsAdsItem::initWithData(const std::string& adsImage,
                               const std::string& gameIcon,
                               const std::string& gameName,
                               const std::string& bundleId,
                               const std::string& price,
                               const std::string& firebaseLogString,
                               const std::string& link,
                               const ActionButtonClickCallback& callback)
{
    Layout* panelLeft  = static_cast<Layout*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_LEFT_PANEL));
    Layout* panelRight = static_cast<Layout*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_RIGHT_PANEL));

    this->_imageAds      = static_cast<ImageView*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_IMAGE_ADS));
    this->_imageGameIcon = static_cast<ImageView*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_IMAGE_GAME_ICON));
    this->_textGameName  = static_cast<Text*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_TEXT_GAME_NAME));
    this->_textPrice     = static_cast<Text*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_TEXT_PRICE));
    this->_buttonAction  = static_cast<Button*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_BUTTON_ACTION));
    this->_buttonAction->addClickEventListener(CC_CALLBACK_1(NewsAdsItem::onButtonClick, this));
    this->_textAction = static_cast<Text*>(Helper::seekWidgetByName(this->_root, ITEM_ADS_TEXT_ACTION));
    this->_spriteAds  = nullptr;
    this->_spriteIcon = nullptr;

    auto labelAds = static_cast<Text*>(Helper::seekWidgetByName(this->_root, "text_studio_name"));
    labelAds->setString(fmt::format(GET_TEXT_BY_ID(TXT_ADS_BY), GameUtils::getDeveloperName()));

    this->_bundle_id           = bundleId;
    this->_firebase_log_string = firebaseLogString;
    this->_link                = link;

    this->_textGameName->setString(gameName);
    if (GameUtils::validateURL(adsImage))
    {
        this->_spriteAds = Sprite::create();
        this->_spriteAds->setAnchorPoint(this->_imageAds->getAnchorPoint());
        this->_spriteAds->setPosition(this->_imageAds->getPosition());
        panelLeft->addChild(this->_spriteAds);
        this->_imageAds->setVisible(false);
        DownloadManager::getInstance()->downloadImage(this->_spriteAds, adsImage, Resources::PNG::IconGameDefault, this->_imageAds->getContentSize());
    }
    else
    {
        this->_imageAds->setVisible(true);
        this->_imageAds->loadTexture(adsImage);
    }

    if (GameUtils::validateURL(gameIcon))
    {
        this->_spriteIcon = Sprite::create();
        this->_spriteIcon->setAnchorPoint(this->_imageGameIcon->getAnchorPoint());
        this->_spriteIcon->setPosition(this->_imageGameIcon->getPosition());
        panelRight->addChild(this->_spriteIcon);

        this->_imageGameIcon->setVisible(false);
        DownloadManager::getInstance()->downloadImage(this->_spriteIcon, gameIcon, Resources::PNG::IconGameDefault, this->_imageGameIcon->getContentSize());
    }
    else
    {
        this->_imageGameIcon->setVisible(true);
        this->_imageGameIcon->loadTexture(gameIcon);
    }

    if (price == "")
        this->_textPrice->setString(GET_TEXT_BY_ID(TXT_FREE));
    else
        this->_textPrice->setString(price);
    this->_textAction->setString(GET_TEXT_BY_ID(TXT_DOWNLOAD));
    this->_actionButtonCallback = callback;
}

NewsAdsItem* NewsAdsItem::createNode(cocos2d::ui::Layout* templateRoot,
                                     const std::string& adsImage,
                                     const std::string& gameIcon,
                                     const std::string& gameName,
                                     const std::string& bundleId,
                                     const std::string& price,
                                     const std::string& firebaseLogString,
                                     const std::string& link,
                                     const ActionButtonClickCallback& callback)
{
    NewsAdsItem* item = NewsAdsItem::create();
    item->_root = static_cast<Layout*>(templateRoot->clone());
    item->initWithData(adsImage, gameIcon, gameName, bundleId, price, firebaseLogString, link, callback);
    item->_root->setVisible(true);

    item->setAnchorPoint(Vec2(0.5f, 0.5f));
    item->setContentSize(item->_root->getContentSize());
    item->addChild(item->_root);
    item->_root->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
    ui::Helper::doLayout(item->_root);
    return item;
}

void NewsAdsItem::onButtonClick(cocos2d::Ref* sender)
{
    Button* button = (Button*) sender;
    if (button == _buttonAction)
    {
        if (this->_actionButtonCallback != nullptr)
            this->_actionButtonCallback();
        Firebase::logOpenApp(this->_firebase_log_string);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        CallToOtherLanguage::getInstance()->openAnotherAppInStore(this->_bundle_id);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CallToOtherLanguage::getInstance()->openAnotherAppInStoreByLink(this->_link);
#endif
    }
}

NewsPopup::NewsPopup()
{
    this->_applyTouchOutClose = false;
    this->_applyBackKey       = false;
}

NewsPopup::~NewsPopup()
{
    CC_SAFE_RELEASE_NULL(this->_panel_item_content_app_ads);
    CC_SAFE_RELEASE_NULL(this->_image_indicator);
}

bool NewsPopup::init()
{
    if (!InterfacePopupBoard::init())
        return false;
    initScreen();
    return true;
}

void NewsPopup::initScreen()
{
    initWithColor(POPUP_BACKGROUND_COLOR);
    Node* node = CSLoader::createNode(PATH);
    Node* root = node->getChildByName(ID_ROOT);
    root->removeFromParent();

    this->_layoutRoot = static_cast<Layout*>(root);
    this->_layoutRoot->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->_layoutRoot->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(_layoutRoot, kAboveForeground);
    ui::Helper::doLayout(this);

    Size  rootSize     = this->_layoutRoot->getContentSize();
    float contentScale = 1.0f;
    if (rootSize.width < 960)
        contentScale = rootSize.width / 960;

    this->_panel_content = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_CONTENT));
    this->_panel_content->setScale(contentScale);

    this->_buttonClose = static_cast<Button*>(Helper::seekWidgetByName(_layoutRoot, BUTTON_CLOSE));
    this->_buttonClose->setPressedActionEnabled(true);
    this->_buttonClose->addClickEventListener(CC_CALLBACK_1(NewsPopup::onButtonClick, this));

    this->_page_view_content = static_cast<PageView*>(Helper::seekWidgetByName(_layoutRoot, PAGE_VIEW_CONTENT));
    this->_page_view_content->addEventListener(CC_CALLBACK_2(NewsPopup::onPageViewEventCallback, this));

    this->_panel_indicator = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_INDICATOR));

    this->_image_indicator = static_cast<ImageView*>(Helper::seekWidgetByName(_layoutRoot, IMAGE_INDICATOR));
    this->_image_indicator->setVisible(false);
    this->_image_indicator->removeFromParent();
    this->_image_indicator->retain();

    this->_panel_item_content_app_ads = static_cast<Layout*>(Helper::seekWidgetByName(_layoutRoot, PANEL_ITEM_APP_ADS));
    this->_panel_item_content_app_ads->setVisible(false);
    this->_panel_item_content_app_ads->removeFromParent();
    this->_panel_item_content_app_ads->retain();
}

cocos2d::FiniteTimeAction* NewsPopup::createShowAction(float delay)
{
    ScaleTo    * scaleUp = ScaleTo::create(0.25f, 1.0f);
    EaseBackOut* action  = EaseBackOut::create(scaleUp);
    return Sequence::create(DelayTime::create(delay), action, nullptr);
}

void NewsPopup::open()
{
    this->setScale(0);
    FiniteTimeAction* popupAction = createShowAction(0.75f);
    this->runAction(popupAction);
    this->_buttonClose->setVisible(false);
    this->_buttonClose->setOpacity(0);
    this->_buttonClose->setTouchEnabled(false);
    Sequence* closeButtonActions = Sequence::create(DelayTime::create(4.0f), CallFunc::create([=] { this->_buttonClose->setVisible(true); }), FadeIn::create(0.25f), CallFunc::create([=] {
        this->_buttonClose->setTouchEnabled(true);
        this->_applyBackKey       = true;
        this->_applyTouchOutClose = true;
    }), nullptr);
    this->_buttonClose->runAction(closeButtonActions);
    _state = State::OPENED;
}

void NewsPopup::onButtonClick(cocos2d::Ref* sender)
{
    SoundManager::playSoundEffect(SoundEvent::CLICK_NORMAL_BUTTON);
    if (sender == _buttonClose)
        this->close();
}

void NewsPopup::onPageViewEventCallback(Ref* sender, cocos2d::ui::PageView::EventType eventType)
{
    if (sender == this->_page_view_content && eventType == PageView::EventType::TURNING)
        updateIndicator();
}

bool NewsPopup::loadNews(const rapidjson::Document& doc)
{
    const rapidjson::Value& data = JsonHelper::getValue(doc, JSON_KEY_DATA);
    int      index     = 0;
    double   randomNum = GameUtils::randomDoubleInRange(0, 100);
    double   lastBound = 0;
    for (int i         = 0; i < data.Size(); i++)
    {
        const rapidjson::Value& item = data[i];
        double ratio = JsonHelper::getDouble(item, JSON_KEY_ITEM_APPEAR_RATIO, 1.0f) * 100.0f;
        if (lastBound <= randomNum && randomNum <= lastBound + ratio)
        {
            int type = JsonHelper::getInt(item, JSON_KEY_ITEM_TYPE, NewsItemType::TYPE_NONE);
            if (type == (int) NewsItemType::TYPE_ADS)
            {
                string adsImgLocalPath  = JsonHelper::getString(item, JSON_KEY_ITEM_ADS_IMAGE_LOCAL_PATH, "");
                string iconImgLocalPath = JsonHelper::getString(item, JSON_KEY_ITEM_ICON_IMAGE_LOCAL_PATH, "");
                string adsImgPath       = JsonHelper::getString(item, JSON_KEY_ITEM_ADS_IMAGE_PATH, "");
                string iconImgPath      = JsonHelper::getString(item, JSON_KEY_ITEM_ICON_IMAGE_PATH, "");
                string gameName         = JsonHelper::getString(item, JSON_KEY_ITEM_GAME_NAME, "");
                string bundleId         = JsonHelper::getString(item, JSON_KEY_ITEM_BUNDLE_ID, "");
                string price            = JsonHelper::getString(item, JSON_KEY_ITEM_PRICE, "");
                string logString        = JsonHelper::getString(item, JSON_KEY_ITEM_FIREBASE_LOG_STRING, "");
                string link             = JsonHelper::getString(item, JSON_KEY_ITEM_LINK, "");

                if (adsImgLocalPath != "")
                    adsImgPath  = adsImgLocalPath;
                if (iconImgLocalPath != "")
                    iconImgPath = iconImgLocalPath;

                bool isAdsImgPathValid  = ((GameUtils::validateURL(adsImgPath) == true) || (FileUtils::getInstance()->isFileExist(adsImgPath) == true));
                bool isIconImgPathValid = ((GameUtils::validateURL(iconImgPath) == true) || (FileUtils::getInstance()->isFileExist(iconImgPath) == true));

                if (adsImgPath != "" && iconImgPath != "" && gameName != "" && bundleId != "" && isAdsImgPathValid == true && isIconImgPathValid == true)
                {
                    NewsAdsItem* newsnode = NewsAdsItem::createNode(this->_panel_item_content_app_ads,
                                                                    adsImgPath,
                                                                    iconImgPath,
                                                                    gameName,
                                                                    bundleId,
                                                                    price,
                                                                    logString,
                                                                    link,
                                                                    ACTION_BUTTON_CALLBACK_SELECTOR(NewsPopup::onNewsItemAdsActionButtonCallback));
                    newsnode->setTag(index);
                    this->_page_view_content->addPage(newsnode);
                    ImageView* image = static_cast<ImageView*>(this->_image_indicator->clone());
                    image->setVisible(true);
                    image->setTag(index);
                    this->_panel_indicator->addChild(image);
                    index++;
                }
            }
        }
        lastBound += ratio;
    }
    Vector<Node*> children = this->_panel_indicator->getChildren();
    float         posX     = 0;
    for (int      i        = 0; i < children.size(); i++)
    {
        Size size = children.at(i)->getContentSize();
        if (i == 0)
            posX += 0;
        else
            posX += PADDING_INDICATOR + size.width;
        Vec2 pos = Vec2(posX, size.width / 2);
        children.at(i)->setPosition(pos);
    }
    this->_panel_indicator->setContentSize(Size(posX, this->_panel_indicator->getContentSize().height));
    updateIndicator();
    return (index > 0);
}

bool NewsPopup::showNewsPopup(cocos2d::Node* parent, bool isNewDay)
{
    if (!UserData::getInstance()->isEnableInterferingAds())
        return false;
    bool show = false;
    UserData::getInstance()->updateNewsFunctionVersion();
    if (isNewDay == true)
    {
        UserData::getInstance()->resetNewsShowCounter();
        if (UserData::getInstance()->shouldShowDailyNews(s_versionCode))
        {
            std::string dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_NEWS_POPUP_DATA, Resources::JSON::JSON_FILE_NEWS);
            const rapidjson::Document& doc  = JsonHelper::parseStringToJson(dataString);
            const rapidjson::Value   & data = JsonHelper::getValue(doc, JSON_KEY_DATA);
            if (data.Size() > 0)
            {
                NewsPopup* popup = BoardManager::createNewsPopup();
                bool hasNew = popup->loadNews(doc);
                if (hasNew)
                    popup->attach(parent, kAboveForeground);
                show = hasNew;
            }
        }
        UserData::getInstance()->updateNewsShowCounter(s_versionCode);
    }
    else
    {
        if (UserData::getInstance()->shouldShowDailyNews(s_versionCode))
        {
            std::string dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_NEWS_POPUP_DATA, Resources::JSON::JSON_FILE_NEWS);
            const rapidjson::Document& doc  = JsonHelper::parseStringToJson(dataString);
            const rapidjson::Value   & data = JsonHelper::getValue(doc, JSON_KEY_DATA);
            if (data.Size() > 0)
            {
                NewsPopup* popup = BoardManager::createNewsPopup();
                bool hasNew = popup->loadNews(doc);
                if (hasNew)
                    popup->attach(parent, kAboveForeground);
                show = hasNew;
            }
        }
    }
    return show;
}

void NewsPopup::updateIndicator()
{
    Vector<Node*> children = this->_panel_indicator->getChildren();
    if (children.size() > 1)
    {
        this->_panel_indicator->setVisible(true);
        int pageIndex = this->_page_view_content->getCurrentPageIndex();
        Widget* selectedPage = this->_page_view_content->getItem(pageIndex);
        if (selectedPage != nullptr)
        {
            int      selectedIndex = selectedPage->getTag();
            for (int i             = 0; i < children.size(); i++)
            {
                Node* child = children.at(i);
                if (child->getTag() == selectedIndex)
                    child->setOpacity(255);
                else
                    child->setOpacity(100);
            }
        }
        else
            this->_panel_indicator->setVisible(false);
    }
    else
        this->_panel_indicator->setVisible(false);
}

void NewsPopup::onNewsItemAdsActionButtonCallback()
{
    this->close();
}
