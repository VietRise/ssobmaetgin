#ifndef __SVR_LEVEL_H__
#define __SVR_LEVEL_H__

#include <map>

class SvrLevel
{
private:
    static std::map<int, std::pair<int, int>> _levelExp;

    static void setLevel(int level);
    static void setLevelExp(int levelexp);
    static void setLevelUp();
protected:
public:
    static void initLevelExp();
    static int getLevel();
    static int getLevelExp();
    static const std::pair<int, int>& getLevelExp(int level);
    static void addLevelExp(int exp);
};

#endif //__SVR_LEVEL_H__
