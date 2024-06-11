#ifndef __LOG_HELPER_H__
#define __LOG_HELPER_H__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include <string>
#include <cstdarg>
#include <iostream>
#include <utility>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "StringHelper.h"
#include "ConsoleTable.h"

class LogHelper
{
public:
    enum Level
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    }; // Level

    inline static const std::map<Level, std::string> levels{{ Level::DEBUG, "DEBUG" },
                                                            { Level::INFO,  "INFO" },
                                                            { Level::WARN,  "WARN" },
                                                            { Level::ERROR, "ERROR" }};

    enum Group
    {
        EMPTY,
        FIREBASE,
        MESSAGE,
        MATCH,
        BOT
    }; // Group

    inline static const std::map<Group, std::string> groups{{ Group::EMPTY,    "" },
                                                            { Group::FIREBASE, "FIREBASE" },
                                                            { Group::MESSAGE,  "MESSAGE" },
                                                            { Group::MATCH,    "MATCH" },
                                                            { Group::BOT,      "BOT" }};

    typedef std::vector<std::pair<std::string, std::string>> vlogs;

private:
    static const bool PRETTY_TIME = false;

    static std::string formatCommon(const std::string& text);
    static std::string formatLevel();
    static std::string formatGroup();

    static void warnAlignInGroups(Group group);
    static std::vector<std::string> alignWhenMultiline(const vlogs& texts, const std::string& separator);
    static std::vector<std::string> alignWhenMultiline(const std::vector<std::string>& texts);

    static void printDebugView(Level level, Group group, const std::string& text, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    static void printConsole(Level level, Group group, const std::string& text);
    static void printAll(Level level, Group group, const std::string& text, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);

    static std::string format(Level level, Group group, const std::string& text);
    static std::string ffl(const std::string& text, const std::string& filename = "", const std::string& function = "", int line = -1);

    static std::string prettyTime();

public:
    static void debug(Group group, const std::string& text, const std::string& filename = "", const std::string& function = "");
    static void debugColor(Group group, const std::string& text, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    static void info(Group group, const std::string& text);
    static void warn(const std::string& filename, const std::string& function, int line, const std::string& text);
    static void error(const std::string& filename, const std::string& function, int line, const std::string& text);

    static void message(const rapidjson::Document& data);

    static void match(const std::string& name, const std::string& text = "", const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    static void match(const std::string& name, const std::vector<std::string>& texts, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    static void match(const std::string& name, const vlogs& texts, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE, const std::string& separator = ":");
    static void match(const ConsoleTable& consoleTable);
    static void matchEmptyRow();

    static void bot(const std::string& name, const std::string& function, const std::string& text = "", const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    static void bot(const std::string& name, const std::string& function, const std::vector<std::string>& texts, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);

    static void debugConsoleTable(const ConsoleTable& consoleTable);

    // START: getVar
    static std::string getVar(const std::string& name, const std::string& value);
    static std::string getVar(const std::string& name, int value);
    static std::string getVar(const std::string& name, long value);
    static std::string getVar(const std::string& name, float value);
    static std::string getVar(const std::string& name, double value);
    static std::string getVar(const std::string& name, bool value);
    static std::string getVar(const std::string& name, const std::vector<std::string>& values);
    static std::string getVar(const std::string& name, const std::vector<int>& values);
    static std::string getVar(const std::string& name, const std::vector<long>& values);
    static std::string getVar(const std::string& name, const std::vector<float>& values);
    static std::string getVar(const std::string& name, const std::vector<double>& values);
    static std::string getVar(const std::string& name, const std::vector<bool>& values);
    // END: getVar

    // START: list
    template<typename T>
    static inline std::vector<std::string> list(const T& t)
    {
        return { t };
    }

    template<typename T, typename ... Args>
    static inline std::vector<std::string> list(const T& t, const Args& ... args)
    {
        std::vector<std::string> v = list(t);
        std::vector<std::string> l = list(args...);
        v.insert(v.end(), l.begin(), l.end());
        return v;
    }
    // END: list

    // START: listPairs
    template<typename K, typename V>
    static vlogs listPairs(const K& key, const V& value)
    {
        return { std::make_pair(StringHelper::stringify(key), StringHelper::stringify(value)) };
    }

    template<typename K, typename V, typename... KVargs>
    static vlogs listPairs(const K& key, const V& value, const KVargs& ... args)
    {
        vlogs result = listPairs(key, value);
        vlogs list   = listPairs(args...);
        result.insert(result.end(), list.begin(), list.end());
        return result;
    }
    // END: listPairs

    template<typename ... Args>
    static void matchArgs(const std::string& name, const Args& ... args)
    {
        match(name, list(args...));
    }

    template<typename ... Args>
    static void botArgs(const std::string& name, const std::string& function, const Args& ... args)
    {
        bot(name, function, list(args...));
    }

    template<typename MyGroup, typename MyFile, typename MyFunction, typename MyValue, typename ...Args>
    static void debugArgs(MyGroup&& myGroup, MyFile&& myFile, MyFunction&& myFunction, MyValue&& myValue, Args&& ...args)
    {
        debug(std::forward<decltype(myGroup)>(myGroup), StringHelper::format(std::forward<decltype(myValue)>(myValue), std::forward<Args>(args)...), std::forward<decltype(myFile)>(myFile), std::forward<decltype(myFunction)>(myFunction));
    }

    template<typename MyGroup, typename MyColor, typename MyValue, typename ...Args>
    static void debugColorArgs(MyGroup&& myGroup, MyColor&& myColor, MyValue&& myValue, Args&& ...args)
    {
        debugColor(std::forward<decltype(myGroup)>(myGroup), StringHelper::format(std::forward<decltype(myValue)>(myValue), std::forward<Args>(args)...), std::forward<decltype(myColor)>(myColor));
    }
};

#endif // __LOG_HELPER_H__
