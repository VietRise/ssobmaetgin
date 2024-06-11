#include "SvrShop.h"
#include "Helper/JsonHelper.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "ServerOffline/InGameServerOfflineDefines.h"
#include "Helper/GameUtils.h"
#include "Define/ResourcesDefine.h"
#include "ServerOffline/SvrUserProfile.h"
#include "ServerOffline/SvrHelper.h"
#include "Define/ServerClientMessageDefine.h"

std::vector<SvrCity> SvrShop::getCities()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES);
    if (json.empty())
        initCities();
    return parseCities(GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES));
}

std::vector<SvrTable> SvrShop::getTables()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES);
    if (json.empty())
        initTables();
    return parseTables(GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES));
}

void SvrShop::initCities()
{
    GameUtils::setStringForKeyUserDefault(KEY_LIST_PLACES, JsonHelper::convertDocumentToString(JsonHelper::getDocumentFromFilename(Resources::JSON::JSON_FILE_LIST_CITIES)));
}

void SvrShop::initTables()
{
    GameUtils::setStringForKeyUserDefault(KEY_LIST_TABLES, JsonHelper::convertDocumentToString(JsonHelper::getDocumentFromFilename(Resources::JSON::JSON_FILE_LIST_TABLES)));
}

void SvrShop::initUserCities()
{
    GameUtils::setStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES, "{\"list_cities\":[1,2,3]}");
}

void SvrShop::initUserTables()
{
    GameUtils::setStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES, "{\"list_tables\":[1]}");
}

std::vector<SvrCity> SvrShop::parseCities(const std::string& json)
{
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(json);
    std::vector<SvrCity> listCity;
    const rapidjson::Value& cities = doc[JSON_CITY];
    for (int i = 0; i < cities.Size(); i++)
    {
        const rapidjson::Value& cityNode = cities[i];
        SvrCity city;
        city.id     = cityNode[JSON_CITY_ID].GetInt();
        city.status = cityNode[JSON_CITY_STATUS].GetInt();
        city.price  = cityNode[JSON_CITY_PRICE].GetInt();
        city.name   = cityNode[JSON_CITY_NAME].GetString();
        city.numBot = cityNode[JSON_CITY_BOT_NUM].GetInt();

        const auto& cityBonus = cityNode[JSON_CITY_BONUS];
        for (int j = 0; j < cityBonus.Size(); j++)
        {
            const auto& bonusVal = cityBonus[j];
            SvrMultiplyBonus bonus;
            bonus.ratio  = bonusVal[JSON_CITY_BONUS_RATIO].GetDouble();
            bonus.chance = bonusVal[JSON_CITY_BONUS_CHANCE].GetDouble();
            city.bonus.push_back(bonus);
        }

        const auto& condition = cityNode[JSON_CITY_UNLOCK_CONDITION];
        for (int j = 0; j < condition.Size(); j++)
            city.listRequireCity.push_back(condition[j].GetInt());
        city.jackpotRatioBet = cityNode[KEY_PLACE_JACKPOT_RATIO_BET].GetFloat();
        city.requiredMoney = cityNode[KEY_PLACE_REQUIRED_MONEY].GetDouble();
        listCity.push_back(city);
    }
    return listCity;
}

std::vector<SvrTable> SvrShop::parseTables(const std::string& json)
{
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(json);
    std::vector<SvrTable> listTable;
    const rapidjson::Value& tables = doc[JSON_TABLE];
    for (int i = 0; i < tables.Size(); i++)
    {
        const rapidjson::Value& tableNode = tables[i];
        SvrTable table;
        table.id     = tableNode[JSON_TABLE_ID].GetInt();
        table.status = tableNode[JSON_TABLE_STATUS].GetInt();
        table.price  = tableNode[JSON_TABLE_PRICE].GetInt();
        table.name   = tableNode[JSON_TABLE_NAME].GetString();
        table.bonus  = tableNode[JSON_TABLE_BONUS].GetDouble();

        const auto& condition = tableNode[JSON_TABLE_UNLOCK_CONDITION];
        for (int j = 0; j < condition.Size(); j++)
            table.listRequireCity.push_back(condition[j].GetInt());
        listTable.push_back(table);
    }
    return listTable;
}

std::vector<int> SvrShop::getAlreadyBuyCity()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES);
    if (json.empty())
        initUserCities();
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES));
    std::vector<int> list;
    const auto& cities = doc[JSON_CITY];
    for (int i = 0; i < cities.Size(); i++)
        list.push_back(cities[i].GetInt());
    return list;
}

std::vector<int> SvrShop::getAlreadyBuyTable()
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES);
    if (json.empty())
        initUserTables();
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES));
    std::vector<int> list;
    const auto& tables = doc[JSON_TABLE];
    for (int i = 0; i < tables.Size(); i++)
        list.push_back(tables[i].GetInt());
    return list;
}

void SvrShop::applyAlreadyBuyCity(int id)
{
    if (GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES).empty())
        initUserCities();
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES));
    rapidjson::Document newDoc = JsonHelper::copyDocument(doc);
    newDoc[JSON_CITY].PushBack(id, newDoc.GetAllocator());
    GameUtils::setStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_CITIES, JsonHelper::convertDocumentToString(newDoc));
}

void SvrShop::applyAlreadyBuyTable(int id)
{
    if (GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES).empty())
        initUserTables();
    const rapidjson::Document& doc = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES));
    rapidjson::Document newDoc = JsonHelper::copyDocument(doc);
    newDoc[JSON_TABLE].PushBack(id, newDoc.GetAllocator());
    GameUtils::setStringForKeyUserDefault(KEY_LIST_ALREADY_BUY_TABLES, JsonHelper::convertDocumentToString(newDoc));
}

std::pair<int, std::pair<std::vector<SvrCity>, std::vector<SvrTable>>> SvrShop::buyCity(int cityId)
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES);
    if (json.empty())
        initCities();
    rapidjson::Document                                                    doc          = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES));
    std::vector<int>                                                       listUserCity = getAlreadyBuyCity();
    SvrCity                                                                city         = getCityByID(cityId, doc);
    std::pair<int, std::pair<std::vector<SvrCity>, std::vector<SvrTable>>> result;
    if (city.status != STATUS_AVAILABLE)
    {
        result.first = RESULT_BUY_FAIL_UNABLE_TO_BUY;
        return result;
    }
    if (SvrUserProfile::getMoney() < city.price)
    {
        result.first = RESULT_BUY_FAIL_NOT_ENOUGH_MONEY;
        return result;
    }
    if (!isMeetCityCondition(listUserCity, city.listRequireCity))
    {
        result.first = RESULT_BUY_FAIL_REQUIREMENT_NOT_MET;
        return result;
    }
    if (city.status == STATUS_AVAILABLE && SvrUserProfile::getMoney() >= city.price)
    {
        for (int i = 0; i < city.listRequireCity.size(); i++)
        {
            SvrCity prevCity = getCityByID(city.listRequireCity[i], doc);
            if (prevCity.status != STATUS_ALREADY_BUY)
            {
                result.first = RESULT_BUY_FAIL_REQUIREMENT_NOT_MET;
                return result;
            }
        }

        city.status = STATUS_ALREADY_BUY;
        doc = changeCityStatus(city.id, STATUS_ALREADY_BUY, doc);
        applyAlreadyBuyCity(city.id);
        SvrUserProfile::setMoney("BuyCity", -city.price);
        listUserCity.push_back(city.id);

        std::vector<SvrCity> listCity = getCities();
        for (const auto& ct : listCity)
            if (ct.id != city.id)
                if (ct.status == STATUS_LOCK && isMeetCityCondition(listUserCity, ct.listRequireCity))
                    doc = changeCityStatus(ct.id, STATUS_AVAILABLE, doc);

        result.first  = RESULT_BUY_SUCCESS;
        result.second = std::make_pair(getCities(), getTables());
        return result;
    }

    result.first = RESULT_BUY_FAIL;
    return result;
}

std::pair<int, std::vector<SvrTable>> SvrShop::buyTable(int tableId)
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES);
    if (json.empty())
        initTables();
    rapidjson::Document                   doc   = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES));
    SvrTable                              table = getTableByID(tableId, doc);
    std::pair<int, std::vector<SvrTable>> result;
    if (table.status != STATUS_AVAILABLE)
    {
        result.first = RESULT_BUY_FAIL_UNABLE_TO_BUY;
        return result;
    }
    if (SvrUserProfile::getMoney() < table.price)
    {
        result.first = RESULT_BUY_FAIL_NOT_ENOUGH_MONEY;
        return result;
    }
    if (table.status == STATUS_AVAILABLE && SvrUserProfile::getMoney() >= table.price)
    {
        table.status = STATUS_ALREADY_BUY;
        doc = changeTableStatus(table.id, STATUS_ALREADY_BUY, doc);
        applyAlreadyBuyTable(table.id);
        SvrUserProfile::setMoney("BuyTable", -table.price);

        result.first  = RESULT_BUY_SUCCESS;
        result.second = getTables();
        return result;
    }

    result.first = RESULT_BUY_FAIL;
    return result;
}

rapidjson::Document SvrShop::changeCityStatus(int id, int status, const rapidjson::Document& doc)
{
    rapidjson::Document newDoc = JsonHelper::copyDocument(doc);
    auto& cities = newDoc[JSON_CITY];
    for (int i = 0; i < cities.Size(); i++)
    {
        auto& city = cities[i];
        if (city[JSON_CITY_ID].GetInt() == id)
            city[JSON_CITY_STATUS].SetInt(status);
    }
    GameUtils::setStringForKeyUserDefault(KEY_LIST_PLACES, JsonHelper::convertDocumentToString(newDoc));
    return newDoc;
}

rapidjson::Document SvrShop::changeTableStatus(int id, int status, const rapidjson::Document& doc)
{
    rapidjson::Document newDoc = JsonHelper::copyDocument(doc);
    auto& tables = newDoc[JSON_TABLE];
    for (int i = 0; i < tables.Size(); i++)
    {
        auto& table = tables[i];
        if (table[JSON_TABLE_ID].GetInt() == id)
            table[JSON_TABLE_STATUS].SetInt(status);
    }
    GameUtils::setStringForKeyUserDefault(KEY_LIST_TABLES, JsonHelper::convertDocumentToString(newDoc));
    return newDoc;
}

SvrCity SvrShop::getCityByID(int id, const rapidjson::Document& doc)
{
    const auto& cities = doc[JSON_CITY];
    for (int i = 0; i < cities.Size(); i++)
    {
        const rapidjson::Value& cityNode = cities[i];
        if (cityNode[JSON_CITY_ID].GetInt() == id)
        {
            SvrCity city;
            city.id            = cityNode[JSON_CITY_ID].GetInt();
            city.status        = cityNode[JSON_CITY_STATUS].GetInt();
            city.price         = cityNode[JSON_CITY_PRICE].GetInt();
            city.name          = cityNode[JSON_CITY_NAME].GetString();
            city.numBot        = cityNode[JSON_CITY_BOT_NUM].GetInt();
            city.jackpotRatioBet = cityNode[KEY_PLACE_JACKPOT_RATIO_BET].GetFloat();
            city.requiredMoney = cityNode[KEY_PLACE_REQUIRED_MONEY].GetDouble();

            const auto& bonuses = cityNode[JSON_CITY_BONUS];
            for (int j = 0; j < bonuses.Size(); j++)
            {
                const auto& bonusVal = bonuses[j];
                SvrMultiplyBonus bonus;
                bonus.ratio  = bonusVal[JSON_CITY_BONUS_RATIO].GetDouble();
                bonus.chance = bonusVal[JSON_CITY_BONUS_CHANCE].GetDouble();
                city.bonus.push_back(bonus);
            }

            const auto& condition = cityNode[JSON_CITY_UNLOCK_CONDITION];
            for (int j = 0; j < condition.Size(); j++)
                city.listRequireCity.push_back(condition[j].GetInt());
            return city;
        }
    }
    return SvrCity();
}

SvrTable SvrShop::getTableByID(int id, const rapidjson::Document& doc)
{
    const auto& tables = doc[JSON_TABLE];
    for (int i = 0; i < tables.Size(); i++)
    {
        const rapidjson::Value& tableNode = tables[i];
        if (tableNode[JSON_TABLE_ID].GetInt() == id)
        {
            SvrTable table;
            table.id     = tableNode[JSON_TABLE_ID].GetInt();
            table.status = tableNode[JSON_TABLE_STATUS].GetInt();
            table.price  = tableNode[JSON_TABLE_PRICE].GetInt();
            table.name   = tableNode[JSON_TABLE_NAME].GetString();
            table.bonus  = tableNode[JSON_TABLE_BONUS].GetDouble();

            const auto& condition = tableNode[JSON_TABLE_UNLOCK_CONDITION];
            for (int j = 0; j < condition.Size(); j++)
                table.listRequireCity.push_back(condition[j].GetInt());
            return table;
        }
    }
    return SvrTable();
}

SvrCity SvrShop::getCityByID(int id)
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES);
    if (json.empty())
        initCities();
    return getCityByID(id, JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES)));
}

SvrTable SvrShop::getTableByID(int id)
{
    std::string json = GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES);
    if (json.empty())
        initCities();
    return getTableByID(id, JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES)));
}

bool SvrShop::isMeetCityCondition(const std::vector<int>& listCity, const std::vector<int>& listCondition)
{
    for (const auto& condition : listCondition)
        if (std::find(listCity.begin(), listCity.end(), condition) == listCity.end())
            return false;
    return true;
}

bool SvrShop::isMeetTableCondition(int tableId)
{
    if (tableId == 1)
        return true;
    int preTableId = tableId - 1;
    if (preTableId <= 0)
        return false;
    return isBuyTable(preTableId);
}

bool SvrShop::isBuyCity(int cityId)
{
    std::vector<int> list = getAlreadyBuyCity();
    if (std::find(list.begin(), list.end(), cityId) == list.end())
        return false;
    return true;
}

bool SvrShop::isBuyTable(int tableId)
{
    std::vector<int> list = getAlreadyBuyTable();
    if (std::find(list.begin(), list.end(), tableId) == list.end())
        return false;
    return true;
}

int SvrShop::getLastedAlreadyBuyCity()
{
    std::vector<int> list         = getAlreadyBuyCity();
    int              lastedCityId = 1;
    for (const int& cityId : list)
        if (cityId > lastedCityId)
            lastedCityId = cityId;
    return lastedCityId;
}

void SvrShop::updateRawCityData()
{
    SvrShop::initCities();
    std::vector<int>    listAlreadyBuy = getAlreadyBuyCity();
    rapidjson::Document doc            = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_PLACES));
    for (const int& cityId : listAlreadyBuy)
        doc = changeCityStatus(cityId, STATUS_ALREADY_BUY, doc);
    std::vector<SvrCity> listCity = getCities();
    for (const auto& city : listCity)
        if (city.status == STATUS_LOCK && isMeetCityCondition(listAlreadyBuy, city.listRequireCity))
            doc = changeCityStatus(city.id, STATUS_AVAILABLE, doc);
}

void SvrShop::updateRawTableData()
{
    initTables();
    std::vector<int>    listAlreadyBuy = getAlreadyBuyTable();
    rapidjson::Document doc            = JsonHelper::parseStringToJson(GameUtils::getStringForKeyUserDefault(KEY_LIST_TABLES));
    for (const auto& table : listAlreadyBuy)
        doc = changeTableStatus(table, STATUS_ALREADY_BUY, doc);
}
