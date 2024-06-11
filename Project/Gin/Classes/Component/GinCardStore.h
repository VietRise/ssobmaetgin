#ifndef __GIN_CARD_STORE_H__
#define __GIN_CARD_STORE_H__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <functional>

#define GIN_CARD_BACK_SPRITEFRAME_NAME "plist/ImageCard/back1.png"
#define GIN_CARD_FRONT_SPRITEFRAME_NAME "plist/ImageCard/front.png"
#define GIN_CARD_FRONT_SMALL_SPRITEFRAME_NAME "plist/ImageCard/front_small.png"
#define GIN_CARD_FRONT_SPECIAL_SPRITEFRAME_NAME "plist/ImageCard/front_special.png"
#define GIN_CARD_FRONT_SPECIAL_SMALL_SPRITEFRAME_NAME "plist/ImageCard/front_special_small.png"
#define GIN_CARD_FRONT_SPECIAL_2_SMALL_SPRITEFRAME_NAME "plist/ImageCard/front_special1.png"
#define GIN_CARD_FRONT_BONUS_CARD_SMALL_SPRITEFRAME_NAME "plist/ImageCard/bonus_mark_1.png"
#define GIN_CARD_FRONT_BONUS_CARD_SPRITEFRAME_NAME "plist/ImageCard/bonus_mark.png"

#define GIN_CARD_FRONT_COLOR_1_SPRITEFRAME_NAME "plist/ImageCard/front_color_1.png"
#define GIN_CARD_FRONT_COLOR_2_SPRITEFRAME_NAME "plist/ImageCard/front_color_2.png"
#define GIN_CARD_FRONT_COLOR_3_SPRITEFRAME_NAME "plist/ImageCard/front_color_3.png"

#define GIN_CARD_FRONT_CIRCLE_COLOR_1_SPRITEFRAME_NAME "plist/ImageCard/card_circle_color_1.png"
#define GIN_CARD_FRONT_CIRCLE_COLOR_2_SPRITEFRAME_NAME "plist/ImageCard/card_circle_color_2.png"
#define GIN_CARD_FRONT_CIRCLE_COLOR_3_SPRITEFRAME_NAME "plist/ImageCard/card_circle_color_3.png"

enum class MeldColorType
{
    UNKNOWN,
    COLOR_1,
    COLOR_2,
    COLOR_3
};

class GinCardStore
{
private:
    rapidjson::Document                              _cardDefine;
    std::map<int, std::string>                       _mapCardIndex;
    cocos2d::Map<std::string, cocos2d::SpriteFrame*> _listFrameCached;

protected:
public:
    ~GinCardStore();
    GinCardStore();

    static GinCardStore* getInstance();
    static void destroyInstance();

    void init();

    void loadCardDefine(const std::string& filename);
    void setCardMapIndex();

    cocos2d::Sprite* getCardFront(int index);
    cocos2d::Sprite* getCardFront1(int index);
    cocos2d::Sprite* getCardBack(const std::string& spriteFrame);

    cocos2d::SpriteFrame* getFrame(const std::string& nameFrame);
    static std::string getCardBackSpriteNameFromID(int backID);

    static std::string getMeldColorSpriteFrameName(MeldColorType meldColor);
    static std::string getMeldColorCircleSpriteFrameName(MeldColorType meldColor);
};

#endif // __GIN_CARD_STORE_H__

