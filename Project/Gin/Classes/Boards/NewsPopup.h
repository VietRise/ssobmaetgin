#ifndef __NEWS_POPUP_H__
#define __NEWS_POPUP_H__

#include "cocos2d.h"
#include "InterfacePopupBoard.h"
#include <functional>
#include "Helper/JsonHelper.h"

#define ACTION_BUTTON_CALLBACK_SELECTOR(_SELECTOR)  std::bind(&_SELECTOR, this)
typedef std::function<void()> ActionButtonClickCallback;

class NewsItem : public cocos2d::ui::Layout
{
private:
protected:
public:
    NewsItem();
    virtual ~NewsItem();
    virtual bool init() override;
    CREATE_FUNC(NewsItem);
};

class NewsAdsItem : public NewsItem
{
private:
    cocos2d::ui::Layout   * _root;
    cocos2d::ui::ImageView* _imageAds;
    cocos2d::ui::ImageView* _imageGameIcon;
    cocos2d::ui::Text     * _textGameName;
    cocos2d::ui::Text     * _textPrice;
    cocos2d::ui::Button   * _buttonAction;
    cocos2d::ui::Text     * _textAction;
    cocos2d::Sprite       * _spriteAds;
    cocos2d::Sprite       * _spriteIcon;

    ActionButtonClickCallback _actionButtonCallback;

    std::string _bundle_id;
    std::string _firebase_log_string;
    std::string _link;
protected:
public:

    NewsAdsItem();
    ~NewsAdsItem();

    virtual bool init() override;

    void onButtonClick(cocos2d::Ref* sender);
    void initWithData(const std::string& adsImage,
                      const std::string& gameIcon,
                      const std::string& gameName,
                      const std::string& bundleId,
                      const std::string& price,
                      const std::string& firebaseLogString,
                      const std::string& link,
                      const ActionButtonClickCallback& callback);
    static NewsAdsItem* createNode(cocos2d::ui::Layout* templateRoot,
                                   const std::string& adsImage,
                                   const std::string& gameIcon,
                                   const std::string& gameName,
                                   const std::string& bundleId,
                                   const std::string& price,
                                   const std::string& firebaseLogString,
                                   const std::string& link,
                                   const ActionButtonClickCallback& callback);

    CREATE_FUNC(NewsAdsItem);
};

enum NewsItemType
{
    TYPE_NONE = -1,
    TYPE_ADS
};

class NewsPopup : public InterfacePopupBoard
{
private:
    cocos2d::ui::Layout   * _root;
    cocos2d::ui::Button   * _buttonClose;
    cocos2d::ui::Layout   * _panel_content;
    cocos2d::ui::Layout   * _panel_item_content_app_ads;
    cocos2d::ui::Layout   * _panel_indicator;
    cocos2d::ui::ImageView* _image_indicator;
    cocos2d::ui::PageView * _page_view_content;

    cocos2d::FiniteTimeAction* createShowAction(float delay = 0);

    void onButtonClick(cocos2d::Ref* sender);
    void onPageViewEventCallback(Ref* sender, cocos2d::ui::PageView::EventType eventType);
    void updateIndicator();
    void onNewsItemAdsActionButtonCallback();
protected:
public:
    NewsPopup();
    ~NewsPopup();

    virtual bool init() override;
    void initScreen();
    virtual void open() override;
    bool loadNews(const rapidjson::Document& doc);
    CREATE_FUNC(NewsPopup);

    static bool showNewsPopup(cocos2d::Node* parent, bool isNewDay);
};

#endif //__NEWS_POPUP_H__