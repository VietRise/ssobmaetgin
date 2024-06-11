#include "StringHelper.h"

using namespace std;

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const initializer_list<string>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, vector<string>{ elements });
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const initializer_list<int>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, vector<int>{ elements });
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const initializer_list<long>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, vector<long>{ elements });
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const initializer_list<double>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, vector<double>{ elements });
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const vector<string>& elements)
{
    string result;
    result.append(prefix);
    if (!elements.empty())
        result.append(elements.at(0));
    for (int i = 1; i < elements.size(); i++)
        result.append(delimiter).append(elements.at(i));
    result.append(suffix);
    return result;
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const vector<int>& elements)
{
    if (!elements.empty())
    {
        vector<string> list;
        for (const auto& element : elements)
            list.push_back(to_string(element));
        return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, list);
    }
    return EMPTY_STRING;
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const vector<long>& elements)
{
    if (!elements.empty())
    {
        vector<string> list;
        for (const auto& element : elements)
            list.push_back(to_string(element));
        return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, list);
    }
    return EMPTY_STRING;
}

string StringHelper::joinDelimiterPrefixSuffix(const string& delimiter, const string& prefix, const string& suffix, const vector<double>& elements)
{
    if (!elements.empty())
    {
        vector<string> list;
        for (const auto& element : elements)
            list.push_back(to_string(element));
        return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, list);
    }
    return EMPTY_STRING;
}

string StringHelper::joinDelimiter(const string& delimiter, const initializer_list<string>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const initializer_list<int>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const initializer_list<long>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const initializer_list<double>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const vector<string>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const vector<int>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const vector<long>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::joinDelimiter(const string& delimiter, const vector<double>& elements)
{
    return joinDelimiterPrefixSuffix(delimiter, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::join(const initializer_list<string>& elements)
{
    return joinDelimiterPrefixSuffix(EMPTY_STRING, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::join(const vector<string>& elements)
{
    return joinDelimiterPrefixSuffix(EMPTY_STRING, EMPTY_STRING, EMPTY_STRING, elements);
}

string StringHelper::repeat(const string& text, const int& repeats)
{
    string   result;
    for (int i = 0; i < repeats; i++)
        result.append(text);
    return result;
}

string StringHelper::toString(const bool& b)
{
    return b ? "true" : "false";
}

string StringHelper::ltrim(const string& str)
{
    return std::regex_replace(str, std::regex("^\\s+"), StringHelper::EMPTY_STRING);
}

string StringHelper::rtrim(const string& str)
{
    return std::regex_replace(str, std::regex("\\s+$"), StringHelper::EMPTY_STRING);
}

string StringHelper::trim(const string& str)
{
    return ltrim(rtrim(str));
}

vector<string> StringHelper::split(const string& s, const string& delimiter)
{
    vector<string> result;

    int start, end = -1 * delimiter.size();
    do
    {
        start = end + delimiter.size();
        end   = s.find(delimiter, start);
        result.push_back(s.substr(start, end - start));
    }
    while (end != -1);

    return result;
}