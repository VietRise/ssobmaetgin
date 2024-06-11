#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Define/TextDefine.h"

#define USERDEFAULT_LANGUAGE "language"

#define GET_TEXT_BY_ID(_TEXTID) TextManager::getInstance()->getTextById(_TEXTID).c_str()
#define GET_TEXT_BY_JSON(_JSON) TextManager::getInstance()->getTextByJson(_JSON).c_str()
#define GET_JSON_BY_ID(_TEXTID, _DEFAULT) TextManager::getInstance()->getJsonById(_TEXTID, _DEFAULT)

enum class Language
{
    LANGUAGE_ENGLISH    = 1,
    LANGUAGE_INDONESIAN = 2
};

class TextManager
{
private:
    TextManager();
    rapidjson::Document              _textData;
    std::map<int, rapidjson::Value*> _mapTextData;
    Language                         _currentLanguageType;
protected:
public:
    ~TextManager();

    static TextManager* getInstance();
    static void destroyInstance();

    void loadData(const std::string& filename);

    void setLanguageType(Language type);
    Language getLanguageType();
    std::string getTextById(int id);
    std::string getTextByJson(const rapidjson::Document& data);
    std::string getTextByJson(const rapidjson::Value& data);
    const rapidjson::Value& getJsonById(int id, const rapidjson::Value& defaultValue);

    void saveLanguageDefault();
    void loadLanguageDefault();
};

#endif // __TEXT_MANAGER_H__ 
