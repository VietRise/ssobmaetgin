#include "TextManager.h"
#include "Helper/JsonHelper.h"
#include "FirebaseGG/RemoteConfig.h"
#include "Define/InGameDefine.h"
#include "UserData/UserData.h"

USING_NS_CC;

static TextManager* _instance = nullptr;

TextManager::TextManager()
{
    this->_currentLanguageType = Language::LANGUAGE_ENGLISH;
    loadLanguageDefault();
}

TextManager::~TextManager()
{
}

TextManager* TextManager::getInstance()
{
    if (_instance == nullptr)
        _instance = new TextManager();
    return _instance;
}

void TextManager::destroyInstance()
{
    if (_instance != nullptr)
        CC_SAFE_DELETE(_instance);
}

void TextManager::loadData(const std::string& filename)
{
    _mapTextData.clear();
    const std::string& dataString = RemoteConfig::getStringData(REMOTECONFIG_FUNCTION_LOCALIZE_TEXT, filename);
    this->_textData = JsonHelper::parseStringToJson(dataString);
    for (int i = 0; i < _textData.Size(); i++)
        _mapTextData.insert(std::make_pair(JsonHelper::getInt(_textData[i], "ID", -1), &_textData[i]));
}

std::string TextManager::getTextById(int id)
{
    if (this->_mapTextData.size() > 0 && this->_mapTextData.find(id) != this->_mapTextData.end())
    {
        const rapidjson::Value* textObject = _mapTextData.at(id);
        if (textObject->IsNull() == false)
        {
            if (this->_currentLanguageType == Language::LANGUAGE_ENGLISH)
                return JsonHelper::getString(*textObject, ENGLISH, "");
            else if (this->_currentLanguageType == Language::LANGUAGE_INDONESIAN)
                return JsonHelper::getString(*textObject, INDONESIAN, "");
            else
                return JsonHelper::getString(*textObject, ENGLISH, "");
        }
    }
    return StringUtils::format("TNF: %d", id);
}

const rapidjson::Value& TextManager::getJsonById(int id, const rapidjson::Value& defaultValue)
{
    if (this->_mapTextData.size() > 0 && this->_mapTextData.find(id) != this->_mapTextData.end())
    {
        const rapidjson::Value* textObject = _mapTextData[id];
        if (textObject->IsNull() == false)
            return *textObject;
    }
    return defaultValue;
}

std::string TextManager::getTextByJson(const rapidjson::Document& data)
{
    if (!data.IsNull() && data.IsObject())
    {
        if (this->_currentLanguageType == Language::LANGUAGE_ENGLISH)
            return JsonHelper::getString(data, ENGLISH, "");
        else if (this->_currentLanguageType == Language::LANGUAGE_INDONESIAN)
            return JsonHelper::getString(data, INDONESIAN, "");
        else
            return JsonHelper::getString(data, ENGLISH, "");
    }
    return "";
}

std::string TextManager::getTextByJson(const rapidjson::Value& data)
{
    if (!data.IsNull() && data.IsObject())
    {
        if (this->_currentLanguageType == Language::LANGUAGE_ENGLISH)
            return JsonHelper::getString(data, ENGLISH, "");
        else if (this->_currentLanguageType == Language::LANGUAGE_INDONESIAN)
            return JsonHelper::getString(data, INDONESIAN, "");
        else
            return JsonHelper::getString(data, ENGLISH, "");
    }
    return "";
}

void TextManager::setLanguageType(Language type)
{
    this->_currentLanguageType = type;
    saveLanguageDefault();
}

Language TextManager::getLanguageType()
{
    return this->_currentLanguageType;
}

void TextManager::saveLanguageDefault()
{
    UserDefault::getInstance()->setIntegerForKey(USERDEFAULT_LANGUAGE, (int) this->_currentLanguageType);
}

void TextManager::loadLanguageDefault()
{
    int lC = UserDefault::getInstance()->getIntegerForKey(USERDEFAULT_LANGUAGE, -1);
    switch (lC)
    {
        case (int) Language::LANGUAGE_ENGLISH:
        case (int) Language::LANGUAGE_INDONESIAN:
            setLanguageType((Language) lC);
            break;
        default:
        {
            setLanguageType(Language::LANGUAGE_ENGLISH);
            break;
        }
    }
}
