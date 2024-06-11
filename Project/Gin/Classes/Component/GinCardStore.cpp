#include "GinCardStore.h"
#include "Helper/JsonHelper.h"

USING_NS_CC;
using namespace std;

static GinCardStore* _instance = nullptr;

GinCardStore::~GinCardStore()
{
}

GinCardStore::GinCardStore()
{
}


GinCardStore* GinCardStore::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new GinCardStore();
        _instance->init();
    }
    return _instance;
}

void GinCardStore::destroyInstance()
{
    if (_instance != nullptr)
    {
        CC_SAFE_DELETE(_instance);
    }
}

void GinCardStore::init()
{

}

void GinCardStore::loadCardDefine(const std::string& filename)
{
    this->_cardDefine = JsonHelper::getDocumentFromFilename(filename);
    /*this->_cardDefine.SetObject();
    std::string jsonString = JsonHelper::getJsonStringFromFilename(filename);
    if (this->_cardDefine.Parse<0>(jsonString.c_str()).HasParseError())
    {
        CCAssert(false, StringUtils::format("Json parse fail: %s", jsonString.c_str()).c_str());
    }*/

    setCardMapIndex();
}

void GinCardStore::setCardMapIndex()
{
    this->_mapCardIndex.clear();

    rapidjson::Value::MemberIterator mIter;
    for (mIter = this->_cardDefine.MemberBegin(); mIter != this->_cardDefine.MemberEnd(); mIter++)
    {
        const char* key = mIter->name.GetString();
        int index = JsonHelper::getInt(mIter->value, "index", 0);
        this->_mapCardIndex.insert(std::pair<int, std::string>(index, key));
    }
}

Sprite* GinCardStore::getCardFront(int index)
{
    string nameCard = this->_mapCardIndex[index];

    Sprite* spriteFront = Sprite::createWithSpriteFrame(getFrame(GIN_CARD_FRONT_SPRITEFRAME_NAME));
    spriteFront->setCascadeColorEnabled(true);
    spriteFront->setCascadeOpacityEnabled(true);
    string skinId = "skin_1";
    //string skinId = "skin_2";
    const rapidjson::Value& value = JsonHelper::getValue(this->_cardDefine, nameCard);
    const rapidjson::Value& skin  = JsonHelper::getValue(value, skinId);
    for (unsigned int i = 0; i < skin.Size(); i++)
    {
        string nameFrame = skin[i].GetString();
        Sprite* sprite = Sprite::createWithSpriteFrame(getFrame(nameFrame));
        sprite->setAnchorPoint(Vec2(0, 0));
        spriteFront->addChild(sprite, 1);
    }
    return spriteFront;
}

cocos2d::Sprite* GinCardStore::getCardFront1(int index)
{
    string nameCard = this->_mapCardIndex[index];

    Sprite* spriteFront = Sprite::createWithSpriteFrame(getFrame(GIN_CARD_FRONT_SMALL_SPRITEFRAME_NAME));
    spriteFront->setCascadeColorEnabled(true);
    spriteFront->setCascadeOpacityEnabled(true);
    //string skinId = "skin_1";
    string skinId = "skin_2";
    const rapidjson::Value& value = JsonHelper::getValue(this->_cardDefine, nameCard);
    const rapidjson::Value& skin  = JsonHelper::getValue(value, skinId);
    for (unsigned int i = 0; i < skin.Size(); i++)
    {
        string nameFrame = skin[i].GetString();
        Sprite* sprite = Sprite::createWithSpriteFrame(getFrame(nameFrame));
        sprite->setAnchorPoint(Vec2(0, 0));
        spriteFront->addChild(sprite, 1);
    }
    return spriteFront;
}

Sprite* GinCardStore::getCardBack(const std::string& spriteFrame)
{
    return Sprite::createWithSpriteFrameName(spriteFrame);
}

SpriteFrame* GinCardStore::getFrame(const std::string& nameFrame)
{
    SpriteFrame* spriteFrame = this->_listFrameCached.at(nameFrame);
    if (spriteFrame != nullptr)
        return spriteFrame->clone();
    spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(nameFrame);
    this->_listFrameCached.insert(nameFrame, spriteFrame);
    return spriteFrame;
}

std::string GinCardStore::getCardBackSpriteNameFromID(int backID)
{
    return GIN_CARD_BACK_SPRITEFRAME_NAME;
}

std::string GinCardStore::getMeldColorSpriteFrameName(MeldColorType meldColor)
{
    switch (meldColor)
    {
        case MeldColorType::COLOR_1:
            return GIN_CARD_FRONT_COLOR_1_SPRITEFRAME_NAME;
        case MeldColorType::COLOR_2:
            return GIN_CARD_FRONT_COLOR_2_SPRITEFRAME_NAME;
        case MeldColorType::COLOR_3:
            return GIN_CARD_FRONT_COLOR_3_SPRITEFRAME_NAME;
    }
    return "";
}

std::string GinCardStore::getMeldColorCircleSpriteFrameName(MeldColorType meldColor)
{
    switch (meldColor)
    {
        case MeldColorType::COLOR_1:
            return GIN_CARD_FRONT_CIRCLE_COLOR_1_SPRITEFRAME_NAME;
        case MeldColorType::COLOR_2:
            return GIN_CARD_FRONT_CIRCLE_COLOR_2_SPRITEFRAME_NAME;
        case MeldColorType::COLOR_3:
            return GIN_CARD_FRONT_CIRCLE_COLOR_3_SPRITEFRAME_NAME;
    }
    return "";
}
