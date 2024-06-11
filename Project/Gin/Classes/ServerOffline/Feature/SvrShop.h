#ifndef __SVR_SHOP_H__
#define __SVR_SHOP_H__

#include <string>
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#include "ServerOffline/SvrHelper.h"

class SvrCity
{
public:
    int                           id;
    int                           price;
    int                           status;
    int                           numBot;
    std::vector<SvrMultiplyBonus> bonus;
    std::string                   name;
    std::vector<int>              listRequireCity;
    float                         jackpotRatioBet;
    double                        requiredMoney;
};

class SvrTable
{
public:
    int              id;
    int              price;
    int              status;
    std::string      name;
    double           bonus;
    std::vector<int> listRequireCity;
};

class SvrShop
{
private:
    static std::vector<SvrCity> parseCities(const std::string& json);
    static std::vector<SvrTable> parseTables(const std::string& json);
    static rapidjson::Document changeCityStatus(int id, int status, const rapidjson::Document& doc);
    static rapidjson::Document changeTableStatus(int id, int status, const rapidjson::Document& doc);
    static SvrCity getCityByID(int id, const rapidjson::Document& doc);
    static SvrTable getTableByID(int id, const rapidjson::Document& doc);
    static void applyAlreadyBuyCity(int id);
    static void applyAlreadyBuyTable(int id);
    static bool isMeetCityCondition(const std::vector<int>& listCity, const std::vector<int>& listCondition);
    static std::vector<int> getAlreadyBuyCity();
    static std::vector<int> getAlreadyBuyTable();
protected:
public:
    static void initCities();
    static void initTables();
    static void initUserCities();
    static void initUserTables();
    static void updateRawCityData();
    static void updateRawTableData();
    static int getLastedAlreadyBuyCity();
    static void unlockAllCities();
    static std::pair<int, std::pair<std::vector<SvrCity>, std::vector<SvrTable>>> buyCity(int cityId);
    static std::vector<SvrCity> getCities();
    static std::vector<SvrTable> getTables();
    static bool isMeetTableCondition(int tableId);
    static std::pair<int, std::vector<SvrTable>> buyTable(int tableId);
    static bool isBuyCity(int cityId);
    static bool isBuyTable(int tableId);
    static SvrCity getCityByID(int id);
    static SvrTable getTableByID(int id);
};

#endif //__SVR_SHOP_H__
