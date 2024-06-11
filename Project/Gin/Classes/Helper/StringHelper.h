#ifndef __STRING_HELPER_H__
#define __STRING_HELPER_H__

#include <string>
#include <vector>
#include <cstdarg>
#include <sstream>
#include <regex>

class StringHelper
{
public:
    inline static const std::string EMPTY_STRING;
    inline static const std::string NOTFOUND_STRING     = "NOTFOUND";
    inline static const std::string SPACE_STRING        = " ";
    inline static const std::string NEWLINE_STRING      = "\n";
    inline static const std::string TAB_STRING          = "    ";
    inline static const std::string COLON_STRING        = ":";
    inline static const std::string SEMICOLON_STRING    = ";";
    inline static const std::string EQUAL_SIGN_STRING   = "=";
    inline static const std::string PLUS_SIGN_STRING    = "+";
    inline static const std::string VERTICAL_BAR_STRING = "|";
    inline static const std::string HYPHEN_STRING       = "-";
    inline static const std::string COMMA_STRING        = ",";
    inline static const std::string DOT_STRING          = ".";
    inline static const std::string UNDERSCORE_STRING   = "_";

    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::initializer_list<std::string>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::initializer_list<int>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::initializer_list<long>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::initializer_list<double>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::vector<std::string>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::vector<int>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::vector<long>& elements);
    static std::string joinDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const std::vector<double>& elements);

    static std::string joinDelimiter(const std::string& delimiter, const std::initializer_list<std::string>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::initializer_list<int>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::initializer_list<long>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::initializer_list<double>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::vector<std::string>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::vector<int>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::vector<long>& elements);
    static std::string joinDelimiter(const std::string& delimiter, const std::vector<double>& elements);

    static std::string join(const std::initializer_list<std::string>& elements);
    static std::string join(const std::vector<std::string>& elements);

    static std::string repeat(const std::string& text, const int& repeats);

    static std::string toString(const bool& b);

    static std::string ltrim(const std::string& str);
    static std::string rtrim(const std::string& str);
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& s, const std::string& delimiter);

    template<typename... Args>
    static std::string format(const std::string& format, Args&& ... args)
    {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...) + 1;

        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), std::forward<Args>(args)...);
        return { buf.get(), buf.get() + size - 1 };
    }

    template<typename T>
    static std::string typeToString(const T& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    };

    template<typename T>
    static T stringToType(const std::string& s)
    {
        std::stringstream ss(s);

        T t;
        ss >> t;
        return t;
    };

    // START: stringify
    template<typename T>
    static std::string stringify(const T& t)
    {
        std::string s = typeToString(t);
        if (!s.empty())
        {
            if (typeid(t) == typeid(float))
                return format("%.2f", std::stof(s));
            else if (typeid(t) == typeid(double))
                return format("%.2lf", std::stod(s));
        }
        return s;
    }

    template<typename T, typename ... Args>
    static std::string stringify(const T& t, const Args& ... args)
    {
        return stringify(t) + stringify(args...);
    }
    // END: stringify

    // START: list
    template<typename T>
    static std::vector<std::string> list(const T& t)
    {
        return { stringify(t) };
    }

    template<typename T, typename ... Args>
    static std::vector<std::string> list(const T& t, const Args& ... args)
    {
        std::vector<std::string> v = list(t);
        std::vector<std::string> l = list(args...);
        v.insert(v.end(), l.begin(), l.end());
        return v;
    }
    // END: list

    // START: joinArgs
    template<typename ... Args>
    static std::string joinArgsDelimiterPrefixSuffix(const std::string& delimiter, const std::string& prefix, const std::string& suffix, const Args& ... args)
    {
        return joinDelimiterPrefixSuffix(delimiter, prefix, suffix, list(args...));
    }

    template<typename ... Args>
    static std::string joinArgsDelimiter(const std::string& delimiter, const Args& ... args)
    {
        return joinDelimiter(delimiter, list(args...));
    }

    template<typename ... Args>
    static std::string joinArgs(const Args& ... args)
    {
        return join(list(args...));
    }
    // END: joinArgs
};

#endif // __STRING_HELPER_H__
