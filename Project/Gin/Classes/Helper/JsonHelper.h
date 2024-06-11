#ifndef __JSON_HELPER_H__
#define __JSON_HELPER_H__

#include <string>
#include "json/rapidjson.h"
#include "json/document.h"

typedef rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> DocumentType;
typedef rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::CrtAllocator>    ValueType;

class JsonHelper
{
private:
protected:
public:
    static rapidjson::Document getDocumentFromFilename(const std::string& fileName);
    static const rapidjson::Value& getValue(const rapidjson::Document& doc, const std::string& key);
    static const rapidjson::Value& getValue(const rapidjson::Document& val, const std::string& key, const rapidjson::Value& defaultValue);

    static const ValueType& getValue(const DocumentType& doc, const std::string& key);
    static const ValueType& getValue(const DocumentType& doc, const std::string& key, const ValueType& defaultValue);

    static const rapidjson::Value& getValue(const rapidjson::Value& val, const std::string& key);
    static const rapidjson::Value& getValue(const rapidjson::Value& val, const std::string& key, const rapidjson::Value& defaultValue);

    static const ValueType& getValue(const ValueType& val, const std::string& key);
    static const ValueType& getValue(const ValueType& val, const std::string& key, const ValueType& defaultValue);

    static std::string getString(const rapidjson::Document& doc, const std::string& key, const std::string& defaultValue);
    static std::string getString(const DocumentType& doc, const std::string& key, const std::string& defaultValue);

    static int getInt(const rapidjson::Document& doc, const std::string& key, int defaultValue);
    static int getInt(const DocumentType& doc, const std::string& key, int defaultValue);

    static long long getLong(const rapidjson::Document& doc, const std::string& key, long long defaultValue);
    static long long getLong(const DocumentType& doc, const std::string& key, long long defaultValue);

    static double getDouble(const rapidjson::Document& doc, const std::string& key, float defaultValue);
    static double getDouble(const DocumentType& doc, const std::string& key, float defaultValue);

    static float getFloat(const rapidjson::Document& doc, const std::string& key, float defaultValue);
    static float getFloat(const DocumentType& doc, const std::string& key, float defaultValue);

    static bool getBool(const rapidjson::Document& doc, const std::string& key, bool defaultValue);
    static bool getBool(const DocumentType& doc, const std::string& key, bool defaultValue);

    static std::string getString(const rapidjson::Value& val, const std::string& key, const std::string& defaultValue);
    static std::string getString(const ValueType& val, const std::string& key, const std::string& defaultValue);

    static int getInt(const rapidjson::Value& val, const std::string& key, int defaultValue);
    static int getInt(const ValueType& val, const std::string& key, int defaultValue);

    static long long getLong(const rapidjson::Value& val, const std::string& key, long long defaultValue);
    static long long getLong(const ValueType& val, const std::string& key, long long defaultValue);

    static double getDouble(const rapidjson::Value& val, const std::string& key, float defaultValue);
    static double getDouble(const ValueType& val, const std::string& key, float defaultValue);

    static float getFloat(const rapidjson::Value& val, const std::string& key, float defaultValue);
    static float getFloat(const ValueType& val, const std::string& key, float defaultValue);

    static bool getBool(const rapidjson::Value& val, const std::string& key, bool defaultValue);
    static bool getBool(const ValueType& val, const std::string& key, bool defaultValue);

    static std::vector<int> getVectorInt(const rapidjson::Document& val);
    static std::vector<int> getVectorInt(const rapidjson::Value& val);
    static std::vector<int> getVectorInt(const rapidjson::Value& val, const std::string& key);

    static std::vector<long> getVectorLong(const rapidjson::Document& val);
    static std::vector<long> getVectorLong(const rapidjson::Value& val);

    static std::vector<double> getVectorDouble(const rapidjson::Document& val);
    static std::vector<double> getVectorDouble(const rapidjson::Value& val);

    static std::vector<float> getVectorFloat(const rapidjson::Document& val);
    static std::vector<float> getVectorFloat(const rapidjson::Value& val);

    static std::vector<bool> getVectorBool(const rapidjson::Document& val);
    static std::vector<bool> getVectorBool(const rapidjson::Value& val);

    static std::vector<std::string> getVectorString(const rapidjson::Document& val);
    static std::vector<std::string> getVectorString(const rapidjson::Value& val);

    static std::vector<std::vector<int>> getVectorVectorInt(const rapidjson::Document& val);
    static std::vector<std::vector<int>> getVectorVectorInt(const rapidjson::Document& val, const std::string& key);
    static std::vector<std::vector<int>> getVectorVectorInt(const rapidjson::Value& val);
    static std::vector<std::vector<int>> getVectorVectorInt(const rapidjson::Value& val, const std::string& key);

    static std::vector<std::vector<long>> getVectorVectorLong(const rapidjson::Document& val);
    static std::vector<std::vector<long>> getVectorVectorLong(const rapidjson::Value& val);

    static std::vector<std::vector<double>> getVectorVectorDouble(const rapidjson::Document& val);
    static std::vector<std::vector<double>> getVectorVectorDouble(const rapidjson::Value& val);

    static std::vector<std::vector<float>> getVectorVectorFloat(const rapidjson::Document& val);
    static std::vector<std::vector<float>> getVectorVectorFloat(const rapidjson::Value& val);

    static std::vector<std::vector<bool>> getVectorVectorBool(const rapidjson::Document& val);
    static std::vector<std::vector<bool>> getVectorVectorBool(const rapidjson::Value& val);

    static std::vector<std::vector<std::string>> getVectorVectorString(const rapidjson::Document& val);
    static std::vector<std::vector<std::string>> getVectorVectorString(const rapidjson::Value& val);

    static rapidjson::Value makeValueArrayInt(rapidjson::Document& doc, const std::vector<int>& list);
    static rapidjson::Value makeValueArrayLong(rapidjson::Document& doc, const std::vector<long>& list);
    static rapidjson::Value makeValueArrayDouble(rapidjson::Document& doc, const std::vector<double>& list);
    static rapidjson::Value makeValueArrayFloat(rapidjson::Document& doc, const std::vector<float>& list);
    static rapidjson::Value makeValueArrayString(rapidjson::Document& doc, const std::vector<std::string>& list);

    static rapidjson::Value makeValueArrayArrayInt(rapidjson::Document& doc, const std::vector<std::vector<int>>& list);
    static rapidjson::Value makeValueArrayArrayLong(rapidjson::Document& doc, const std::vector<std::vector<long>>& list);
    static rapidjson::Value makeValueArrayArrayDouble(rapidjson::Document& doc, const std::vector<std::vector<double>>& list);
    static rapidjson::Value makeValueArrayArrayFloat(rapidjson::Document& doc, const std::vector<std::vector<float>>& list);
    static rapidjson::Value makeValueArrayArrayString(rapidjson::Document& doc, const std::vector<std::vector<std::string>>& list);

    static std::string convertDocumentToString(const rapidjson::Document& doc, bool pretty = false);
    static std::string convertDocumentToString(const DocumentType& doc);
    static std::string convertValueToString(const rapidjson::Value& value);

    static rapidjson::Document parseStringToJson(const std::string& json, bool object = true);
    static DocumentType parseStringToDocumentType(const std::string& json);

    static rapidjson::Document copyDocument(const rapidjson::Document& doc);
};

#endif //__JSON_HELPER_H__