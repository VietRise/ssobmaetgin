#include "SvrLevel.h"
#include "Define/GameDefine.h"
#include "ServerOffline/ServerOfflineDefines.h"
#include "Helper/GameUtils.h"

std::map<int, std::pair<int, int>> SvrLevel::_levelExp;

void SvrLevel::initLevelExp()
{
    int level = SvrLevel::getLevel();
    SvrLevel::getLevelExp(level + 100);
}

const std::pair<int, int>& SvrLevel::getLevelExp(int level)
{
    if (_levelExp.find(level) != _levelExp.end())
        return _levelExp[level];
    if (level <= 1)
    {
        _levelExp[level] = std::make_pair(0, 0);
        return _levelExp[level];
    }
    int preLevelExp = getLevelExp(level - 1).first;
    int levelExp    = std::floor(preLevelExp + 30 + 10 * level * 0.3f);
    _levelExp[level] = std::make_pair(levelExp, levelExp - preLevelExp);
    return _levelExp[level];
}

int SvrLevel::getLevel()
{
    int level = GameUtils::getDoubleForKeyUserDefault(KEY_USER_LEVEL, -1);
    if (level != -1)
        return level;
    int calculateTotalExp = s_winNumber * 10 + s_loseNumber * 2;
    int preLevel          = 1;
    int userlevel         = _levelExp.size() - 1;
    int userlevelexp      = _levelExp[userlevel].first;

    for (int i = 0; i < _levelExp.size(); i++)
    {
        int step = _levelExp[i].first - calculateTotalExp;
        if (step >= 0)
        {
            userlevel    = preLevel;
            userlevelexp = step;
            break;
        }
        preLevel = i;
    }
    setLevel(userlevel);
    setLevelExp(userlevelexp);
    return userlevel;
}

void SvrLevel::setLevel(int level)
{
    GameUtils::setDoubleForKeyUserDefault(KEY_USER_LEVEL, level);
}

int SvrLevel::getLevelExp()
{
    return GameUtils::getDoubleForKeyUserDefault(KEY_USER_LEVELEXP, 0);
}

void SvrLevel::setLevelExp(int levelexp)
{
    GameUtils::setDoubleForKeyUserDefault(KEY_USER_LEVELEXP, levelexp);
}

void SvrLevel::addLevelExp(int exp)
{
    int totalexp = exp + getLevelExp();
    int levelExp = getLevelExp(getLevel() + 1).second;
    while (totalexp - levelExp >= 0)
    {
        totalexp = totalexp - levelExp;
        setLevelUp();
        levelExp = getLevelExp(getLevel() + 1).second;
    }
    setLevelExp(totalexp);
}

void SvrLevel::setLevelUp()
{
    setLevel(getLevel() + 1);
}


