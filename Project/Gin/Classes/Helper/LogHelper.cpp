#include "LogHelper.h"
#include "GameUtils.h"
#include "ConsoleTable.h"
#include "Define/InGameDefine.h"
#include "Helper/JsonHelper.h"
#include "Define/ServerClientMessageDefine.h"

USING_NS_CC;
using namespace std;

string LogHelper::formatCommon(const string& text)
{
    return text.empty() ? StringHelper::EMPTY_STRING : StringUtils::format("[%s]", text.c_str());
}

string LogHelper::formatLevel()
{
    int max = 0;
    for (const auto& it : levels)
    {
        int length = formatCommon(it.second).length();
        if (length > max)
            max = length;
    }
    return StringUtils::format("%%-%ds", max);
}

string LogHelper::formatGroup()
{
    int max = 0;
    for (const auto& group : groups)
    {
        int length = formatCommon(group.second).length();
        if (length > max)
            max = length;
    }
    return StringUtils::format("%%%ds", max);
}

void LogHelper::warnAlignInGroups(Group group)
{
    if (groups.find(group) == groups.end())
        printConsole(WARN, EMPTY, StringUtils::format("Have a new group [%s], please push it to list [LogHelper.h:groups].", groups.at(group).c_str()));
}

vector<string> LogHelper::alignWhenMultiline(const vlogs& texts, const string& separator)
{
    int max = 0;
    for (const auto& it : texts)
        if (it.first.length() > max)
            max = it.first.length();
    vector<string> results;
    for (const auto& it : texts)
    {
        if (it.first.empty() && it.second.empty())
            results.push_back(StringHelper::EMPTY_STRING);
        else
            results.push_back(StringUtils::format(StringUtils::format("%%-%ds%%s %%s", max).c_str(), it.first.c_str(), separator.c_str(), it.second.c_str()));
    }
    return results;
}

vector<string> LogHelper::alignWhenMultiline(const vector<string>& texts)
{
    vector<string> names;
    int            max = 0;
    for (const string& text : texts)
    {
        vector<string> split = StringHelper::split(text, ":");
        if (split[0].length() > max)
            max = split[0].length();
    }
    vector<string>   results;
    for (const string& text : texts)
    {
        if (text.empty())
            results.push_back(StringHelper::EMPTY_STRING);
        else
        {
            vector<string> split = StringHelper::split(text, ":");
            results.push_back(StringUtils::format(StringUtils::format("%%-%ds%%s %%s", max).c_str(), split[0].c_str(), ":", split[1].c_str()));
        }
    }
    return results;
}

string LogHelper::format(Level level, Group group, const string& text)
{
    string time = PRETTY_TIME ? formatCommon(prettyTime()) : StringHelper::EMPTY_STRING;
    return StringUtils::format("%s%s %s: %s",
                               time.c_str(),
                               StringUtils::format(formatLevel().c_str(), formatCommon(levels.at(level)).c_str()).c_str(),
                               StringUtils::format(formatGroup().c_str(), formatCommon(groups.at(group)).c_str()).c_str(),
                               text.c_str());
}

void LogHelper::printDebugView(Level level, Group group, const string& text, const Color3B& color)
{
    const string msg = format(level, group, text);
    GameUtils::log(msg, color);
    if (level == DEBUG || level == INFO)
        warnAlignInGroups(group);
}

void LogHelper::printConsole(Level level, Group group, const string& text)
{
    cocos2d::log("%s", format(level, group, text).c_str());
}

void LogHelper::printAll(Level level, Group group, const string& text, const Color3B& color)
{
    const string msg = format(level, group, text);
    log("%s", msg.c_str());
    GameUtils::log(msg, color);
    if (level == DEBUG || level == INFO)
        warnAlignInGroups(group);
}

string LogHelper::ffl(const string& text, const string& filename, const string& function, int line)
{
    return StringUtils::format("function %s at %s:%s: %s", function.c_str(), filename.c_str(), (line > -1 ? to_string(line) : "").c_str(), text.c_str());
}

string LogHelper::prettyTime()
{
    auto    now   = chrono::system_clock::now();
    auto    ms    = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto    timer = chrono::system_clock::to_time_t(now);
    std::tm bt    = *std::localtime(&timer);

    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void LogHelper::match(const string& name, const string& text, const Color3B& color)
{
    string s;
    s.append(name);
    if (!name.empty() && !text.empty())
        s.append(": ");
    s.append(text);
    printAll(DEBUG, MATCH, s, color);
}

void LogHelper::match(const string& name, const vector<string>& texts, const Color3B& color)
{
    for (const auto& text : texts)
        match(name, text, color);
}

void LogHelper::match(const string& name, const vlogs& texts, const Color3B& color, const string& separator)
{
    for (const auto& text : alignWhenMultiline(texts, separator))
        match(name, text, color);
}

void LogHelper::match(const ConsoleTable& consoleTable)
{
    for (const string& text : ConsoleTable::toList(consoleTable, false))
        printConsole(DEBUG, MATCH, text);
}

void LogHelper::matchEmptyRow()
{
    printAll(DEBUG, MATCH, StringHelper::EMPTY_STRING);
}

void LogHelper::debug(Group group, const string& text, const string& filename, const string& function)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    if (filename.empty() && function.empty())
        printConsole(DEBUG, group, text);
    else
    {
        string filenameWithoutExtension = !filename.empty() ? filename.substr(0, filename.rfind('.')) : StringHelper::EMPTY_STRING;
        if (!filenameWithoutExtension.empty() && !function.empty())
        {
            string ff = StringUtils::format("%s::%s", filenameWithoutExtension.c_str(), function.c_str());
            printConsole(DEBUG, group, StringUtils::format("%-33s %s", ff.c_str(), text.c_str()));
        }
        else if (!filenameWithoutExtension.empty())
            printConsole(DEBUG, group, StringUtils::format("%s %s", filenameWithoutExtension.c_str(), text.c_str()));
        else if (!function.empty())
            printConsole(DEBUG, group, StringUtils::format("%s %s", function.c_str(), text.c_str()));
    }
    warnAlignInGroups(group);
#endif
}

void LogHelper::debugColor(Group group, const string& text, const Color3B& color)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    printAll(DEBUG, group, text, color);
    warnAlignInGroups(group);
#endif
}

void LogHelper::info(Group group, const string& text)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    printConsole(INFO, group, text);
    warnAlignInGroups(group);
#endif
}

void LogHelper::warn(const string& filename, const string& function, int line, const string& text)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    printConsole(WARN, EMPTY, ffl(text, filename, function, line));
#endif
}

void LogHelper::error(const string& filename, const string& function, int line, const string& text)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    printConsole(ERROR, EMPTY, ffl(text, filename, function, line));
#endif
}

void LogHelper::message(const rapidjson::Document& data)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    int maxLength = 0;
    for (const auto& it : MessageNames)
        if (it.second.length() > maxLength)
            maxLength = it.second.length();

    MessageID msgId = (MessageID) JsonHelper::getInt(data, KEY_MESSAGE_ID, (int) MessageID::MESSAGE_UNKNOWN);
    string    msgName;
    if (MessageNames.find(msgId) != MessageNames.end())
        msgName = MessageNames.at(msgId);
    else
    {
        msgName = "???";
        CCLOGWARN("Please add [MessageId:%d] to [MessageNames]", msgId);
    }
    msgName     = StringUtils::format(StringUtils::format("%%-%ds", maxLength).c_str(), msgName.c_str());
    printAll(DEBUG, MESSAGE, StringUtils::format("%3d: %s: %s", msgId, msgName.c_str(), JsonHelper::convertDocumentToString(data).c_str()), Color3B::GRAY);
    if (msgId == MessageID::MESSAGE_UNKNOWN)
        assert(0);
#endif
}

void LogHelper::bot(const string& name, const string& function, const string& text, const Color3B& color)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    string f = StringUtils::format("<%s>", function.c_str());
    string s = StringUtils::format("%s %-24s %s", name.c_str(), f.c_str(), text.c_str());
    printAll(DEBUG, BOT, s, color);
#endif
}

void LogHelper::bot(const string& name, const string& function, const vector<string>& texts, const Color3B& color)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    bot(name, function, StringHelper::joinDelimiter(", ", texts), color);
#endif
}

void LogHelper::debugConsoleTable(const ConsoleTable& consoleTable)
{
#if COCOS2D_DEBUG > 0 || DEBUG_LOG_BOT
    for (const string& text : ConsoleTable::toList(consoleTable, false))
        printConsole(DEBUG, EMPTY, text);
#endif
}

string LogHelper::getVar(const string& name, const std::string& value)
{
    return StringHelper::format("%s:\"%s\"", name.c_str(), value.c_str());
}

string LogHelper::getVar(const string& name, int value)
{
    return StringHelper::format("%s:%d", name.c_str(), value);
}

string LogHelper::getVar(const string& name, long value)
{
    return StringHelper::format("%s:%ld", name.c_str(), value);
}

string LogHelper::getVar(const string& name, float value)
{
    return StringHelper::format("%s:%.2f", name.c_str(), value);
}

string LogHelper::getVar(const string& name, double value)
{
    return StringHelper::format("%s:%.2lf", name.c_str(), value);
}

string LogHelper::getVar(const string& name, bool value)
{
    return StringHelper::format("%s:%s", name.c_str(), StringHelper::toString(value).c_str());
}

string LogHelper::getVar(const string& name, const vector<string>& values)
{
    return StringHelper::format("%s:%s", name.c_str(), StringHelper::joinDelimiterPrefixSuffix(",", "{", "}", values).c_str());
}

string LogHelper::getVar(const string& name, const vector<int>& values)
{
    return StringHelper::format("%s:%s", name.c_str(), StringHelper::joinDelimiterPrefixSuffix(",", "{", "}", values).c_str());
}

string LogHelper::getVar(const string& name, const vector<long>& values)
{
    return StringHelper::format("%s:%s", name.c_str(), StringHelper::joinDelimiterPrefixSuffix(",", "{", "}", values).c_str());
}

string LogHelper::getVar(const string& name, const vector<float>& values)
{
    vector<string> list;
    for (const auto& value : values)
        list.push_back(StringHelper::format("%.2f", value));
    return getVar(name, list);
}

string LogHelper::getVar(const string& name, const vector<double>& values)
{
    vector<string> list;
    for (const double& value : values)
        list.push_back(StringHelper::format("%.2lf", value));
    return getVar(name, list);
}

string LogHelper::getVar(const string& name, const vector<bool>& values)
{
    vector<string> list;
    for (const auto& value : values)
        list.push_back(StringHelper::toString(value));
    return getVar(name, list);
}
