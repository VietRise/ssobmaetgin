#include "JsonHelper.h"
#include "json/writer.h"
#include "json/prettywriter.h"
#include "cocos2d.h"

using namespace rapidjson;
using namespace std;
USING_NS_CC;

rapidjson::Document JsonHelper::getDocumentFromFilename(const string& fileName)
{
    rapidjson::Document     doc;
    string                  str = FileUtils::getInstance()->getStringFromFile(fileName);
    rapidjson::StringStream stream(str.c_str());
    doc.ParseStream<0>(stream);
    if (doc.HasParseError())
        CCLOGERROR("%s parse error!", fileName.c_str());
    return doc;
}

const rapidjson::Value& JsonHelper::getValue(const rapidjson::Document& doc, const string& key)
{
    if (doc.HasMember(key.c_str()))
        return doc[key.c_str()];
    static rapidjson::Value defaultValueNull(kNullType);
    return defaultValueNull;
}

const rapidjson::Value& JsonHelper::getValue(const rapidjson::Document& doc, const string& key, const rapidjson::Value& defaultValue)
{
    return doc.HasMember(key.c_str()) ? doc[key.c_str()] : defaultValue;
}

const ValueType& JsonHelper::getValue(const DocumentType& doc, const string& key)
{
    if (doc.HasMember(key.c_str()))
        return doc[key.c_str()];
    static ValueType defaultValueTypeNull(kNullType);
    return defaultValueTypeNull;
}

const ValueType& JsonHelper::getValue(const DocumentType& doc, const string& key, const ValueType& defaultValue)
{
    return doc.HasMember(key.c_str()) ? doc[key.c_str()] : defaultValue;
}

const rapidjson::Value& JsonHelper::getValue(const rapidjson::Value& val, const string& key)
{
    if (val.HasMember(key.c_str()))
        return val[key.c_str()];
    static rapidjson::Value defaultValueNull(kNullType);
    return defaultValueNull;
}

const rapidjson::Value& JsonHelper::getValue(const rapidjson::Value& val, const string& key, const rapidjson::Value& defaultValue)
{
    return val.HasMember(key.c_str()) ? val[key.c_str()] : defaultValue;
}

const ValueType& JsonHelper::getValue(const ValueType& val, const string& key)
{
    if (val.HasMember(key.c_str()))
        return val[key.c_str()];
    static ValueType defaultValueTypeNull(kNullType);
    return defaultValueTypeNull;
}

const ValueType& JsonHelper::getValue(const ValueType& val, const string& key, const ValueType& defaultValue)
{
    return val.HasMember(key.c_str()) ? val[key.c_str()] : defaultValue;
}

string JsonHelper::getString(const rapidjson::Document& doc, const string& key, const string& defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsString())
        return string(doc[key.c_str()].GetString());
    return defaultValue;
}

string JsonHelper::getString(const DocumentType& doc, const string& key, const string& defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsString())
        return string(doc[key.c_str()].GetString());
    return defaultValue;
}

string JsonHelper::getString(const rapidjson::Value& val, const string& key, const string& defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsString())
        return string(val[key.c_str()].GetString());
    return defaultValue;
}

string JsonHelper::getString(const ValueType& val, const string& key, const string& defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsString())
        return string(val[key.c_str()].GetString());
    return defaultValue;
}

int JsonHelper::getInt(const rapidjson::Document& doc, const string& key, int defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt())
        return doc[key.c_str()].GetInt();
    return defaultValue;
}

int JsonHelper::getInt(const DocumentType& doc, const string& key, int defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt())
        return doc[key.c_str()].GetInt();
    return defaultValue;
}

int JsonHelper::getInt(const rapidjson::Value& val, const string& key, int defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsInt())
        return val[key.c_str()].GetInt();
    return defaultValue;
}

int JsonHelper::getInt(const ValueType& val, const string& key, int defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsInt())
        return val[key.c_str()].GetInt();
    return defaultValue;
}

long long JsonHelper::getLong(const rapidjson::Document& doc, const string& key, long long defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt64())
        return doc[key.c_str()].GetInt64();
    return defaultValue;
}

long long JsonHelper::getLong(const DocumentType& doc, const string& key, long long defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsInt64())
        return doc[key.c_str()].GetInt64();
    return defaultValue;
}

long long JsonHelper::getLong(const rapidjson::Value& val, const string& key, long long defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsInt64())
        return val[key.c_str()].GetInt64();
    return defaultValue;
}

long long JsonHelper::getLong(const ValueType& val, const string& key, long long defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsInt64())
        return val[key.c_str()].GetInt64();
    return defaultValue;
}

double JsonHelper::getDouble(const rapidjson::Document& doc, const string& key, float defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsNumber())
        return doc[key.c_str()].GetDouble();
    return defaultValue;
}

double JsonHelper::getDouble(const DocumentType& doc, const string& key, float defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsNumber())
        return doc[key.c_str()].GetDouble();
    return defaultValue;
}

double JsonHelper::getDouble(const rapidjson::Value& val, const string& key, float defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsNumber())
        return val[key.c_str()].GetDouble();
    return defaultValue;
}

double JsonHelper::getDouble(const ValueType& val, const string& key, float defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsNumber())
        return val[key.c_str()].GetDouble();
    return defaultValue;
}

float JsonHelper::getFloat(const rapidjson::Document& doc, const string& key, float defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsNumber())
        return doc[key.c_str()].GetFloat();
    return defaultValue;
}

float JsonHelper::getFloat(const DocumentType& doc, const string& key, float defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsNumber())
        return doc[key.c_str()].GetFloat();
    return defaultValue;
}

float JsonHelper::getFloat(const rapidjson::Value& val, const string& key, float defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsNumber())
        return val[key.c_str()].GetFloat();
    return defaultValue;
}

float JsonHelper::getFloat(const ValueType& val, const string& key, float defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsNumber())
        return val[key.c_str()].GetFloat();
    return defaultValue;
}

bool JsonHelper::getBool(const rapidjson::Document& doc, const string& key, bool defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsBool())
        return doc[key.c_str()].GetBool();
    return defaultValue;
}

bool JsonHelper::getBool(const DocumentType& doc, const string& key, bool defaultValue)
{
    if (doc.HasMember(key.c_str()) && doc[key.c_str()].IsBool())
        return doc[key.c_str()].GetBool();
    return defaultValue;
}

bool JsonHelper::getBool(const rapidjson::Value& val, const string& key, bool defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsBool())
        return val[key.c_str()].GetBool();
    return defaultValue;
}

bool JsonHelper::getBool(const ValueType& val, const string& key, bool defaultValue)
{
    if (val.HasMember(key.c_str()) && val[key.c_str()].IsBool())
        return val[key.c_str()].GetBool();
    return defaultValue;
}

vector<int> JsonHelper::getVectorInt(const rapidjson::Document& val)
{
    vector<int>  result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetInt());
    return result;
}

vector<int> JsonHelper::getVectorInt(const rapidjson::Value& val)
{
    vector<int>  result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetInt());
    return result;
}

vector<int> JsonHelper::getVectorInt(const rapidjson::Value& val, const string& key)
{
    return val.HasMember(key.c_str()) ? getVectorInt(val[key.c_str()]) : vector<int>();
}

vector<long> JsonHelper::getVectorLong(const rapidjson::Document& val)
{
    vector<long> result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetInt64());
    return result;
}

vector<long> JsonHelper::getVectorLong(const rapidjson::Value& val)
{
    vector<long> result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetInt64());
    return result;
}

vector<double> JsonHelper::getVectorDouble(const rapidjson::Document& val)
{
    vector<double> result;
    if (val.IsArray())
        for (int   i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetDouble());
    return result;
}

vector<double> JsonHelper::getVectorDouble(const rapidjson::Value& val)
{
    vector<double> result;
    if (val.IsArray())
        for (int   i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetDouble());
    return result;
}

vector<float> JsonHelper::getVectorFloat(const rapidjson::Document& val)
{
    vector<float> result;
    if (val.IsArray())
        for (int  i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetFloat());
    return result;
}

vector<float> JsonHelper::getVectorFloat(const rapidjson::Value& val)
{
    vector<float> result;
    if (val.IsArray())
        for (int  i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetFloat());
    return result;
}

vector<bool> JsonHelper::getVectorBool(const rapidjson::Document& val)
{
    vector<bool> result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetBool());
    return result;
}

vector<bool> JsonHelper::getVectorBool(const rapidjson::Value& val)
{
    vector<bool> result;
    if (val.IsArray())
        for (int i = 0; i < val.Size(); i++)
            result.push_back(val[i].GetBool());
    return result;
}

vector<string> JsonHelper::getVectorString(const rapidjson::Document& val)
{
    vector<string> result;
    if (val.IsArray())
        for (int   i = 0; i < val.Size(); i++)
            result.push_back(string(val[i].GetString()));
    return result;
}

vector<string> JsonHelper::getVectorString(const rapidjson::Value& val)
{
    vector<string> result;
    if (val.IsArray())
        for (int   i = 0; i < val.Size(); i++)
            result.push_back(string(val[i].GetString()));
    return result;
}

vector<vector<int>> JsonHelper::getVectorVectorInt(const rapidjson::Document& val)
{
    vector<vector<int>> result;
    if (val.IsArray())
        for (int        i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorInt(val[i]));
    return result;
}

vector<vector<int>> JsonHelper::getVectorVectorInt(const rapidjson::Document& val, const string& key)
{
    return val.HasMember(key.c_str()) ? getVectorVectorInt(val[key.c_str()]) : vector<vector<int>>();
}

vector<vector<int>> JsonHelper::getVectorVectorInt(const rapidjson::Value& val)
{
    vector<vector<int>> result;
    if (val.IsArray())
        for (int        i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorInt(val[i]));
    return result;
}

vector<vector<int>> JsonHelper::getVectorVectorInt(const rapidjson::Value& val, const string& key)
{
    return val.HasMember(key.c_str()) ? getVectorVectorInt(val[key.c_str()]) : vector<vector<int>>();
}

vector<vector<long>> JsonHelper::getVectorVectorLong(const rapidjson::Document& val)
{
    vector<vector<long>> result;
    if (val.IsArray())
        for (int         i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorLong(val[i]));
    return result;
}

vector<vector<long>> JsonHelper::getVectorVectorLong(const rapidjson::Value& val)
{
    vector<vector<long>> result;
    if (val.IsArray())
        for (int         i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorLong(val[i]));
    return result;
}

vector<vector<double>> JsonHelper::getVectorVectorDouble(const rapidjson::Document& val)
{
    vector<vector<double>> result;
    if (val.IsArray())
        for (int           i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorDouble(val[i]));
    return result;
}

vector<vector<double>> JsonHelper::getVectorVectorDouble(const rapidjson::Value& val)
{
    vector<vector<double>> result;
    if (val.IsArray())
        for (int           i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorDouble(val[i]));
    return result;
}

vector<vector<float>> JsonHelper::getVectorVectorFloat(const rapidjson::Document& val)
{
    vector<vector<float>> result;
    if (val.IsArray())
        for (int          i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorFloat(val[i]));
    return result;
}

vector<vector<float>> JsonHelper::getVectorVectorFloat(const rapidjson::Value& val)
{
    vector<vector<float>> result;
    if (val.IsArray())
        for (int          i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorFloat(val[i]));
    return result;
}

vector<vector<bool>> JsonHelper::getVectorVectorBool(const rapidjson::Document& val)
{
    vector<vector<bool>> result;
    if (val.IsArray())
        for (int         i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorBool(val[i]));
    return result;
}

vector<vector<bool>> JsonHelper::getVectorVectorBool(const rapidjson::Value& val)
{
    vector<vector<bool>> result;
    if (val.IsArray())
        for (int         i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorBool(val[i]));
    return result;
}

vector<vector<string>> JsonHelper::getVectorVectorString(const rapidjson::Document& val)
{
    vector<vector<string>> result;
    if (val.IsArray())
        for (int           i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorString(val[i]));
    return result;
}

vector<vector<string>> JsonHelper::getVectorVectorString(const rapidjson::Value& val)
{
    vector<vector<string>> result;
    if (val.IsArray())
        for (int           i = 0; i < val.Size(); i++)
            result.push_back(JsonHelper::getVectorString(val[i]));
    return result;
}

rapidjson::Value JsonHelper::makeValueArrayInt(rapidjson::Document& doc, const vector<int>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(value, allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayLong(rapidjson::Document& doc, const vector<long>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(rapidjson::Value(rapidjson::kNumberType).SetInt64(value), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayDouble(rapidjson::Document& doc, const vector<double>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(value, allocator);
    return array;
}
rapidjson::Value JsonHelper::makeValueArrayFloat(rapidjson::Document& doc, const vector<float>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(value, allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayString(rapidjson::Document& doc, const vector<string>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(rapidjson::Value(rapidjson::kStringType).SetString(value.c_str(), doc.GetAllocator()), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayArrayInt(rapidjson::Document& doc, const vector<vector<int>>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(makeValueArrayInt(doc, value), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayArrayLong(rapidjson::Document& doc, const vector<vector<long>>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(makeValueArrayLong(doc, value), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayArrayDouble(rapidjson::Document& doc, const vector<vector<double>>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(makeValueArrayDouble(doc, value), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayArrayFloat(rapidjson::Document& doc, const vector<vector<float>>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(makeValueArrayFloat(doc, value), allocator);
    return array;
}

rapidjson::Value JsonHelper::makeValueArrayArrayString(rapidjson::Document& doc, const vector<vector<string>>& list)
{
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (const auto& value : list)
        array.PushBack(makeValueArrayString(doc, value), allocator);
    return array;
}

string JsonHelper::convertDocumentToString(const rapidjson::Document& doc, bool pretty)
{
    rapidjson::StringBuffer buffer;
    if (pretty)
    {
        rapidjson::PrettyWriter<StringBuffer> writer(buffer);
        writer.SetIndent(' ', 2);
        doc.Accept(writer);
    }
    else
    {
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
    }
    return buffer.GetString();
}

string JsonHelper::convertDocumentToString(const DocumentType& doc)
{
    rapidjson::StringBuffer strbuf;
    Writer<StringBuffer>    writer(strbuf);
    doc.Accept(writer);
    return string(strbuf.GetString());
}

string JsonHelper::convertValueToString(const rapidjson::Value& value)
{
    if (!value.IsNull())
    {
        rapidjson::StringBuffer strbuf;
        Writer<StringBuffer>    writer(strbuf);
        value.Accept(writer);
        return string(strbuf.GetString());
    }
    return "";
}

rapidjson::Document JsonHelper::parseStringToJson(const string& json, bool object)
{
    Document document;
    if (object)
        document.SetObject();
    else
        document.SetArray();
    if (!json.empty() && document.Parse<0>(json.c_str()).HasParseError())
                CCAssert (false, StringUtils::format("Json parse fail: %s", json.c_str()).c_str());
    return document;
}

DocumentType JsonHelper::parseStringToDocumentType(const string& json)
{
    DocumentType document;
    document.SetObject();
    if (!json.empty() && document.Parse<0>(json.c_str()).HasParseError())
                CCAssert (false, StringUtils::format("Json parse fail: %s", json.c_str()).c_str());
    return document;
}

rapidjson::Document JsonHelper::copyDocument(const rapidjson::Document& doc)
{
    vector<rapidjson::Document> list;
    rapidjson::Document         document;
    document.SetObject();
    document.CopyFrom(doc, document.GetAllocator());
    return document;
}
